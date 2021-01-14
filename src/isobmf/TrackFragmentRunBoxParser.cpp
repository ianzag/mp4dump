
#include "TrackFragmentRunBoxParser.h"

namespace isobmf {

void TrackFragmentRunBoxParser::startParse()
{
    m_state = State::TrFlags;
    m_parser.reset();
}

void TrackFragmentRunBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::TrFlags:
        if (m_parser.putChar(ch) == sizeof(TrFlags)) {
            m_trFlags = m_parser.getAs<TrFlags>();
            m_state = State::SampleCount;
            m_parser.reset();
        }
        break;
    case State::SampleCount:
        if (m_parser.putChar(ch) == sizeof(SampleCount)) {
            m_sampleCount = m_parser.getAs<SampleCount>();
            if (m_trFlags & TrFlag_DataOffset) {
                m_state = State::DataOffset;
            } else if (m_trFlags & TrFlag_FirstSampleFlags) {
                m_state = State::FirstSampleFlags;
            } else {
                // TODO: Parse array of samples
                m_state = State::Done;
            }
            m_parser.reset();
        }
        break;
    case State::DataOffset:
        if (m_parser.putChar(ch) == sizeof(DataOffset)) {
            m_dataOffset = m_parser.getAs<DataOffset>();
            if (m_trFlags & TrFlag_FirstSampleFlags) {
                m_state = State::FirstSampleFlags;
            } else {
                // TODO: Parse array of samples
                m_state = State::Done;
            }
            m_parser.reset();
        }
        break;
    case State::FirstSampleFlags:
        if (m_parser.putChar(ch) == sizeof(FirstSampleFlags)) {
            m_firstSampleFlags = m_parser.getAs<FirstSampleFlags>();
            // TODO: Parse array of samples
            m_state = State::Done;
            m_parser.reset();
        }
        break;
    case State::Done:
        break;
    }
}

std::ostream& TrackFragmentRunBoxParser::printDetails(std::ostream& os) const
{
    if (m_state != State::Done) {
        os << "parseError: Imcomplete data";
    } else {
        os << "trFlags: 0x" << std::hex << m_trFlags
           << " sampleCount: " << std::dec << m_sampleCount;
        if (m_dataOffset) os << " dataOffset: " << std::dec << *m_dataOffset;
        if (m_firstSampleFlags) os << " firstSampleFlags: " << std::dec << *m_firstSampleFlags;
    }
    return os;
}

} // namespace
