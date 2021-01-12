
#include "FileTypeBoxParser.h"
#include "IsoBmfUtils.h"

namespace isobmf {

void FileTypeBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::MajorBrand:
        if (m_parser.putChar(ch) == 4) {
            m_majorBrand = m_parser.getAs<std::uint32_t>();
            m_state = State::MinorVersion;
            m_parser.reset();
        }
        break;
    case State::MinorVersion:
        if (m_parser.putChar(ch) == 4) {
            m_minorVersion = m_parser.getAs<std::uint32_t>();
            m_state = State::CompatibleBrands;
            m_parser.reset();
        }
        break;
    case State::CompatibleBrands:
        if (m_parser.putChar(ch) == 4) {
            const auto brandCode = m_parser.getAs<std::uint32_t>();
            m_compatibleBrands.push_back(brandCode);
            m_parser.reset();
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
