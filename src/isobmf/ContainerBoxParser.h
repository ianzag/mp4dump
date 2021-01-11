
#pragma once

#include "BaseBoxParser.h"

#include <memory>

namespace isobmf {

/**
 * @brief Parse Container Box

4.2 Object Structure
aligned(8) class Box (unsigned int(32) boxtype, optional unsigned int(8)[16] extended_type) {
    unsigned int(32) size;
    unsigned int(32) type = boxtype;
    if (size==1) {
        unsigned int(64) largesize;
    } else if (size==0) {
        // box extends to end of file
    }
    if (boxtype==‘uuid’) {
        unsigned int(8)[16] usertype = extended_type;
    }
}
 */
class ContainerBoxParser : public BaseBoxParser
{
public:
    explicit ContainerBoxParser(const ParserFactory& boxFactory)
        : m_boxFactory{boxFactory}
    {}

    ContainerBoxParser(std::string boxName,
                       BoxSize boxSize,
                       const BoxParser* parentBox,
                       const ParserFactory& boxFactory)
        : BaseBoxParser(std::move(boxName), boxSize, parentBox)
        , m_boxFactory{boxFactory}
    {}

    void startParse() override;
    void parseChar(std::uint8_t ch) override;
    void endParse() override;

private:
    enum class State
    {
        CompactSize,    ///< Box compact size (normal)
        ExtendedSize,   ///< Box extended size (optional)
        Type,           ///< Box type
        Data,           ///< Box body
    };

    const ParserFactory& m_boxFactory;

    State m_state = State::CompactSize;

    BoxSize m_childBoxSize;
    std::unique_ptr<BoxParser> m_childBoxParser;
    BoxSize m_dataLeft;
};

} // namespace
