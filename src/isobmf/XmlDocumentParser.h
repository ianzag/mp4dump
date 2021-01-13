
#pragma once

#include "IsoBmfInterfaces.h"

#include <string>
#include <string_view>
#include <vector>
#include <expat.h>

namespace isobmf {

using XmlString = std::basic_string<XML_Char>;
using XmlStringView = std::basic_string_view<XML_Char>;

/**
 * @brief Parse XML document stream
 *
 * @note For a real document parser we should process namespace declarations,
 * handle found tags according to found namespaces and so on.
 * Course this is a simple and very specific parser whose sole purpose is to
 * locate images in an explicitly conformant SMPTE-TT document keep it
 * as simple as possible.
 *
 * The real wild-world parser of courst should be much more sophysticated.
 */
class XmlDocumentParser final : public DocumentParser
{
public:
    ~XmlDocumentParser() override;

    void startParse() override;
    bool parseChar(std::uint8_t ch) override;
    bool endParse() override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    /**
     * @brief Handle parsed image body text
     */
    void handleImage();

    void handleElementStart(const XmlStringView& tagName, const XML_Char** tagAtts);
    void handleElementEnd(const XmlStringView& tagName);
    void handleCharacterData(const XmlStringView& s);

    static void startElementHandler(void* userData, const XML_Char* tagName, const XML_Char** tagAtts)
    {
        static_cast<XmlDocumentParser*>(userData)->handleElementStart(tagName, tagAtts);
    }

    static void endElementHandler(void* userData, const XML_Char* tagName)
    {
        static_cast<XmlDocumentParser*>(userData)->handleElementEnd(tagName);
    }

    static void characterDataHandler(void* userData, const XML_Char* s, int len)
    {
        static_cast<XmlDocumentParser*>(userData)->handleCharacterData(XmlStringView(s , len));
    }

    enum class State
    {
        WaitImage,
        ParseImage,
    };

    XML_Parser m_xmlParser = nullptr;
    State m_state;
    XmlString m_imageBody;
    XmlString m_imageType;
    XmlString m_imageEncoding;
    unsigned int m_imageNumber;
    std::vector<std::string> m_foundImages;
};

} // namespace
