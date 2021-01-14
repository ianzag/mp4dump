
#include "TrackFragmentHeaderBoxParser.h"

namespace isobmf {

void TrackFragmentHeaderBoxParser::startParse()
{
    switchState(State::TfFlags);
}

void TrackFragmentHeaderBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::TfFlags:
        if (m_parser.putChar(ch) == sizeof(TfFlags)) {
            m_tfFlags = m_parser.getAs<TfFlags>();
            switchState(State::TrackId);
        }
        break;
    case State::TrackId:
        if (m_parser.putChar(ch) == sizeof(TrackId)) {
            m_trackId = m_parser.getAs<TrackId>();
            if (m_tfFlags & TfFlag_BaseDataOffset) {
                switchState(State::BaseDataOffset);
            } else if (m_tfFlags & TfFlag_SampleDescriptionIndex) {
                switchState(State::SampleDescriptionIndex);
            } else if (m_tfFlags & TfFlag_DefaultSampleDuration) {
                switchState(State::DefaultSampleDuration);
            } else if (m_tfFlags & TfFlag_DefaultSampleSize) {
                switchState(State::DefaultSampleSize);
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                switchState(State::DefaultSampleFlags);
            } else {
                switchState(State::Done);
            }
        }
        break;
    case State::BaseDataOffset:
        if (m_parser.putChar(ch) == sizeof(BaseDataOffset)) {
            m_baseDataOffset = m_parser.getAs<BaseDataOffset>();
            if (m_tfFlags & TfFlag_SampleDescriptionIndex) {
                switchState(State::SampleDescriptionIndex);
            } else if (m_tfFlags & TfFlag_DefaultSampleDuration) {
                switchState(State::DefaultSampleDuration);
            } else if (m_tfFlags & TfFlag_DefaultSampleSize) {
                switchState(State::DefaultSampleSize);
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                switchState(State::DefaultSampleFlags);
            } else {
                switchState(State::Done);
            }
        }
        break;
    case State::SampleDescriptionIndex:
        if (m_parser.putChar(ch) == sizeof(SampleDescriptionIndex)) {
            m_sampleDescriptionIndex = m_parser.getAs<SampleDescriptionIndex>();
            if (m_tfFlags & TfFlag_DefaultSampleDuration) {
                switchState(State::DefaultSampleDuration);
            } else if (m_tfFlags & TfFlag_DefaultSampleSize) {
                switchState(State::DefaultSampleSize);
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                switchState(State::DefaultSampleFlags);
            } else {
                switchState(State::Done);
            }
        }
        break;
    case State::DefaultSampleDuration:
        if (m_parser.putChar(ch) == sizeof(DefaultSampleDuration)) {
            m_defaultSampleDuration = m_parser.getAs<DefaultSampleDuration>();
            if (m_tfFlags & TfFlag_DefaultSampleSize) {
                switchState(State::DefaultSampleSize);
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                switchState(State::DefaultSampleFlags);
            } else {
                switchState(State::Done);
            }
        }
        break;
    case State::DefaultSampleSize:
        if (m_parser.putChar(ch) == sizeof(DefaultSampleSize)) {
            m_defaultSampleSize = m_parser.getAs<DefaultSampleSize>();
            if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                switchState(State::DefaultSampleFlags);
            } else {
                switchState(State::Done);
            }
        }
        break;
    case State::DefaultSampleFlags:
        if (m_parser.putChar(ch) == sizeof(DefaultSampleFlags)) {
            m_defaultSampleFlags = m_parser.getAs<DefaultSampleFlags>();
            switchState(State::Done);
        }
        break;
    case State::Done:
        break;
    }
}

std::ostream& TrackFragmentHeaderBoxParser::printDetails(std::ostream& os) const
{
    if (m_state != State::Done) {
        os << "parseError: Incomplete data";
    } else {
        os << "tfFlags: 0x" << std::hex << m_tfFlags
           << " trackId: " << std::dec << m_trackId;
        if (m_baseDataOffset) os << " baseDataOffset: " << std::dec << *m_baseDataOffset;
        if (m_sampleDescriptionIndex) os << " sampleDescriptionIndex: " << std::dec << *m_sampleDescriptionIndex;
        if (m_defaultSampleDuration) os << " defaultSampleDuration: " << std::dec << *m_defaultSampleDuration;
        if (m_defaultSampleSize) os << " defaultSampleSize: " << std::dec << *m_defaultSampleSize;
        if (m_defaultSampleFlags) os << " defaultSampleFlags: 0x" << std::hex << *m_defaultSampleFlags;
    }
    return os;
}

} // namespace
