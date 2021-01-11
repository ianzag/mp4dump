
#include "UserExtensionBoxParser.h"

#include <iomanip>
#include <iostream>

namespace isobmf {

void UserExtensionBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::UUID:
        if (putChar(ch) == 16) {
            std::copy(m_buf.begin(), m_buf.end(), m_uuid.begin());
            // TODO: Create a extended user box parser for known UUIDs
            m_state = State::Data;
            m_pos = 0;
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
