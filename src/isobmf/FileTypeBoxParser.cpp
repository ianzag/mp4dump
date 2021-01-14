
#include "FileTypeBoxParser.h"
#include "IsoBmfUtils.h"

namespace isobmf {

void FileTypeBoxParser::startParse()
{
    switchState(State::MajorBrand);
}

void FileTypeBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::MajorBrand:
        if (m_parser.putChar(ch) == sizeof(BrandNumber)) {
            m_majorBrand = m_parser.getAs<BrandNumber>();
            switchState(State::MinorVersion);
        }
        break;
    case State::MinorVersion:
        if (m_parser.putChar(ch) == sizeof(VersionNumber)) {
            m_minorVersion = m_parser.getAs<VersionNumber>();
            switchState(State::CompatibleBrands);
        }
        break;
    case State::CompatibleBrands:
        if (m_parser.putChar(ch) == sizeof(BrandNumber)) {
            const auto brandCode = m_parser.getAs<BrandNumber>();
            m_compatibleBrands.push_back(brandCode);
            switchState(State::CompatibleBrands);
        }
        break;
    }
}

std::ostream& FileTypeBoxParser::printDetails(std::ostream& os) const
{
    os << "majorBrand: " << makeBrandName(m_majorBrand) << " ";
    os << "minorVersion: "  << std::dec << m_minorVersion << " ";

    auto numBrands = m_compatibleBrands.size();
    os << "compatibleBrands: {";
    for (const auto brandName : m_compatibleBrands) {
          os << makeBoxName(brandName);
          if (--numBrands) os << " ";
    }
    os << "}";

    return os;
}

} // namespace
