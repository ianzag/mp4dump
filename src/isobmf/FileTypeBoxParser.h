
#pragma once

#include "FullBoxParser.h"
#include "utils/BinaryParser.h"

#include <vector>

namespace isobmf {

/*
4.3 File Type Box
4.3.1 Definition
Box Type:   `ftyp’
Container:  File
Mandatory:  Yes
Quantity:   Exactly one (but see below)

4.3.2 Syntax
aligned(8) class FileTypeBox extends Box(‘ftyp’) {
    unsigned int(32) major_brand;
    unsigned int(32) minor_version;
    unsigned int(32) compatible_brands[];
    // to end of the box
}
*/
class FileTypeBoxParser final : public FullBoxParser
{
public:
    FileTypeBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("File Type", boxSize, parentBox) {}

    void startParse() override;
    void parseChar(std::uint8_t ch) override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    using BrandNumber   = std::uint32_t;
    using VersionNumber = std::uint32_t;

    enum class State
    {
        MajorBrand,
        MinorVersion,
        CompatibleBrands,
    };

    void switchState(State newState) noexcept
    {
        m_state = newState;
        m_parser.reset();
    }

    utils::BinaryParser<4> m_parser;
    State m_state;
    BrandNumber m_majorBrand = 0;
    VersionNumber m_minorVersion = 0;
    std::vector<BrandNumber> m_compatibleBrands;
};

} // namespace
