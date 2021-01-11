
#include "MovieFragmentHeaderBoxParser.h"

namespace isobmf {

void MovieFragmentHeaderBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::Reserved:
        if (putChar(ch) == 4) {
            m_state = State::SequenceNumber;
            m_pos = 0;
        }
        break;
    case State::SequenceNumber:
        if (putChar(ch) == 4) {
            m_sequenceNumber = getAs<std::uint32_t>();
            m_state = State::Done;
            m_pos = 0;
        }
        break;
    case State::Done:
        break;
    }
}

std::ostream& MovieFragmentHeaderBoxParser::printDetails(std::ostream& os) const
{
    os << "sequenceNumber: " << std::dec << m_sequenceNumber;
    return os;
}

} // namespace
