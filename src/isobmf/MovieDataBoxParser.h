
#pragma once

#include "FullBoxParser.h"
#include "FileFormatDetector.h"

#include <memory>
#include <string>

namespace isobmf {

/*
8.1.1 Media Data Box
8.1.1.1 Definition
Box Type:   ‘mdat’
Container:  File
Mandatory:  No
Quantity:   Zero or more

8.1.1.2 Syntax
aligned(8) class MediaDataBox extends Box(‘mdat’) {
    bit(8) data[];
}
*/
class MovieDataBoxParser final : public FullBoxParser
{
public:
    using Parent = FullBoxParser;

    MovieDataBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Movie Data", boxSize, parentBox) {}

    void parseChar(std::uint8_t ch) override;
    void endParse() override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    enum class State
    {
        DetectFormat,
        UnknownFormat,
        ParseDocument,
        ParseError,
    };

    State m_state = State::DetectFormat;
    FileFormatDetector<64> m_formatDetector;
    std::string m_buffer;
    std::unique_ptr<DocumentParser> m_documentParser;
};

} // namespace
