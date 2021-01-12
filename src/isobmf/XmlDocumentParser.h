
#pragma once

#include "IsoBmfInterfaces.h"

#include <expat.h>

namespace isobmf {

/**
 * @brief Parse XML document stream
 */
class XmlDocumentParser final : public DocumentParser
{
public:
    ~XmlDocumentParser() override;

    void startParse() override;
    bool parseChar(std::uint8_t ch) override;
    void endParse() override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    void handleElementStart(const XML_Char* name, const XML_Char** atts);
    void handleElementEnd(const XML_Char* name);
    void handleCharacterData(const XML_Char* s, int len);

    static void startElementHandler(void* userData, const XML_Char* name, const XML_Char** atts)
    {
        static_cast<XmlDocumentParser*>(userData)->handleElementStart(name, atts);
    }

    static void endElementHandler(void* userData, const XML_Char* name)
    {
        static_cast<XmlDocumentParser*>(userData)->handleElementEnd(name);
    }

    static void characterDataHandler(void* userData, const XML_Char* s, int len)
    {
        static_cast<XmlDocumentParser*>(userData)->handleCharacterData(s, len);
    }

    XML_Parser m_xmlParser = nullptr;
};

} // namespace
