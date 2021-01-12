
#include "UserExtensionBoxParser.h"

#include <iomanip>
#include <iostream>

namespace isobmf {

void UserExtensionBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::UUID:
        if (m_parser.putChar(ch) == 16) {
            std::copy(m_parser.begin(), m_parser.end(), m_uuid.begin());
            // TODO: Create an extended user data parser for known UUIDs
            m_state = State::Data;
            m_parser.reset();
        }
        break;
    case State::Data:
        m_dataSize++;
        break;
    }
}

std::ostream& UserExtensionBoxParser::printDetails(std::ostream& os) const
{
    os << "uuid: " << std::hex << std::setfill('0');
    auto it = m_uuid.begin();
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << ":";
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << ":";
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << ":";
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << ":";
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << std::setw(2) << static_cast<unsigned int>(*it++);
    os << " dataSize: " << std::dec << m_dataSize;
    return os;
}

} // namespace
