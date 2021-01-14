
#include "MovieFragmentHeaderBoxParser.h"

namespace isobmf {

void MovieFragmentHeaderBoxParser::startParse()
{
    switchState(State::ReservedField);
}

void MovieFragmentHeaderBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::ReservedField:
        if (m_parser.putChar(ch) == sizeof(ReservedField)) {
            switchState(State::SequenceNumber);
        }
        break;
    case State::SequenceNumber:
        if (m_parser.putChar(ch) == sizeof(SequenceNumber)) {
            m_sequenceNumber = m_parser.getAs<SequenceNumber>();
            switchState(State::Done);
        }
        break;
    case State::Done:
        break;
    }
}

std::ostream& MovieFragmentHeaderBoxParser::printDetails(std::ostream& os) const
{
    if (m_state != State::Done) {
        os << "parseError: Incompleted data";
    } else {
        os << "sequenceNumber: " << std::dec << m_sequenceNumber;
    }
    return os;
}

} // namespace
