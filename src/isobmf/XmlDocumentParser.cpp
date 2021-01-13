
#include "XmlDocumentParser.h"
#include "utils/Base64.h"
#include "utils/StringConvert.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace isobmf {

namespace {

const char tagSmpteImage[] = "smpte:image"; ///< Element that holds image body according to SMPTE-TT
const char tagBase64[] = "base64";

/**
 * @brief Simplify access to XML tag attributes
 * Attributes are passed as an array of name->value string pairs
 */
class XmlAttrHelper
{
public:
    explicit XmlAttrHelper(const XML_Char** atts)
        : m_atts{atts}
    {
        // Number of attributes must be even
        while (m_atts[m_numAtts]) m_numAtts++;
        if (m_numAtts % 2) m_numAtts--;
    }

    /**
     * @brief Find attribute and return its value
     * @param name Attribute name
     * @return Value if attribute found or none if not
     */
    std::optional<const XmlStringView> getAttr(const XmlStringView& name) const
    {
        for (std::size_t pos = 0; pos < m_numAtts; pos += 2) {
            if (m_atts[pos] == name) {
                return m_atts[pos + 1];
            }
        }
        return {};
    }

private:
    const XML_Char** m_atts = nullptr;
    std::size_t m_numAtts = 0;
};

} // namespace

XmlDocumentParser::~XmlDocumentParser()
{
    if (m_xmlParser) {
        XML_ParserFree(m_xmlParser);
    }
}

void XmlDocumentParser::startParse()
{
    m_xmlParser = XML_ParserCreate(nullptr);
    if (!m_xmlParser) {
        throw std::runtime_error("Failed to create XML parser");
    }

    XML_SetUserData(m_xmlParser, this);
    XML_SetElementHandler(m_xmlParser, startElementHandler, endElementHandler);
    XML_SetCharacterDataHandler(m_xmlParser, characterDataHandler);

    m_state = State::WaitImage;
    m_imageNumber = 1;
}

bool XmlDocumentParser::parseChar(std::uint8_t ch)
{
    // Feed char to parser
    if (m_xmlParser) {
        char s[2];
        s[0] = ch;
        if (XML_Parse(m_xmlParser, s, 1, 0) == XML_STATUS_OK) {
            return true;
        }
    }
    return false;
}

bool XmlDocumentParser::endParse()
{
    if (m_xmlParser) {
        // Notify parser about last char
        return XML_Parse(m_xmlParser, nullptr, 0, 1) == XML_STATUS_OK;
    }
    return true;
}

std::ostream& XmlDocumentParser::printDetails(std::ostream& os) const
{
    // Dump info about found images
    auto count = m_foundImages.size();
    os << "images: ";
    if (!count) {
        os << "none";
    } else {
        for (const auto& s : m_foundImages) {
            os << "{" << s << "}";
            if (--count) os << ", ";
        }
    }
    return os;
}

void XmlDocumentParser::handleImage()
{
    // Decode image body if we know encoding method
    if (m_imageEncoding == tagBase64) {
        m_imageBody = utils::Base64Decode(m_imageBody);
    }

    std::string fileName = "image" + std::to_string(m_imageNumber);
    if (m_imageType == "png") { // Note: Looks like SMPTE-TT supports PNG images only
        fileName += ".png";
    } else {
        fileName += ".dat";
    }

    // Write image body to file
    std::ofstream ofs(fileName, ofs.binary | ofs.trunc);
    if (!ofs.is_open()) {
        std::cerr << "Error: Failed to open image file '" << fileName << "'" << std::endl;
    }  else {
        ofs.write(m_imageBody.data(), m_imageBody.size());
        ofs.flush();
    }

    // Remember found image info to report it later
    std::ostringstream os;
    os << "type: " << m_imageType << " size: " << m_imageBody.size() << " file: " << fileName;
    m_foundImages.push_back(os.str());

    m_imageNumber++;
}

void XmlDocumentParser::handleElementStart(const XmlStringView& tagName, const XML_Char** tagAtts)
{
    // Catchup only SMPTE image tags
    switch (m_state) {
    case State::WaitImage:
        if (tagName == tagSmpteImage) {
            // Try to detect image parameters
            XmlAttrHelper attrParser(tagAtts);
            if (const auto value = attrParser.getAttr("imagetype")) {
                m_imageType = utils::toLowerCase(*value);
            }
            if (const auto value = attrParser.getAttr("encoding")) {
                m_imageEncoding = utils::toLowerCase(*value);
            }
            m_state = State::ParseImage; // Start to accept character data
            m_imageBody.clear();
        }
        break;
    case State::ParseImage:
        // TODO: Nested element inside image? That should not happen.
        // But if it does it signals about error. Take care about it.
        break;
    }
}

void XmlDocumentParser::handleElementEnd(const XmlStringView& tagName)
{
    switch (m_state) {
    case State::WaitImage:
        break;
    case State::ParseImage:
        if (tagName == tagSmpteImage) {
            handleImage();
            m_state = State::WaitImage;
            m_imageBody.clear();
        }
        break;
    }
}

void XmlDocumentParser::handleCharacterData(const XmlStringView& s)
{
    if (m_state == State::ParseImage) {
        if (m_imageEncoding == tagBase64) {
            // Strip control chars and spaces (they break our decoder)
            std::copy_if(s.begin(), s.end(), std::back_inserter(m_imageBody), [](auto ch)
            {
                return ch > ' ';
            });
        } else {
            m_imageBody += s;
        }
    }
}

} // namespace
