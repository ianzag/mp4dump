
#include "XmlDocumentParser.h"

#include <iostream>
#include <stdexcept>

namespace isobmf {

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
}

bool XmlDocumentParser::parseChar(std::uint8_t ch)
{
    if (m_xmlParser) {
        char s[2];
        s[0] = ch;
        if (XML_Parse(m_xmlParser, s, 1, 0) == XML_STATUS_OK) {
            return true;
        }
    }
    return false;
}

void XmlDocumentParser::endParse()
{
    if (m_xmlParser) {
        // Notify parser about last char
        XML_Parse(m_xmlParser, nullptr, 0, 1);
    }
}

std::ostream& XmlDocumentParser::printDetails(std::ostream& os) const
{
    // XXX Print details
    return os;
}

void XmlDocumentParser::handleElementStart(const XML_Char* name, const XML_Char** atts)
{
    std::cout << "start '" << name << "'" << std::endl;
    // XXX Handle element
}

void XmlDocumentParser::handleElementEnd(const XML_Char* name)
{
    std::cout << "end '" << name << "'" << std::endl;
    // XXX Handle element
}

void XmlDocumentParser::handleCharacterData(const XML_Char* s, int len)
{
    // XXX Handle data
}

} // namespace
