
#include "TrackFragmentHeaderBoxParser.h"

namespace isobmf {

void TrackFragmentHeaderBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::TfFlags:
        if (putChar(ch) == 4) {
            m_tfFlags = getAs<std::uint32_t>();
            m_state = State::TrackId;
            m_pos = 0;
        }
        break;
    case State::TrackId:
        if (putChar(ch) == 4) {
            m_trackId = getAs<std::uint32_t>();
            if (m_tfFlags & TfFlag_BaseDataOffset) {
                m_state = State::BaseDataOffset;
            } else if (m_tfFlags & TfFlag_SampleDescriptionIndex) {
                m_state = State::SampleDescriptionIndex;
            } else if (m_tfFlags & TfFlag_DefaultSampleDuration) {
                m_state = State::DefaultSampleDuration;
            } else if (m_tfFlags & TfFlag_DefaultSampleSize) {
                m_state = State::DefaultSampleSize;
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                m_state = State::DefaultSampleFlags;
            } else {
                m_state = State::Done;
            }
            m_pos = 0;
        }
        break;
    case State::BaseDataOffset:
        if (putChar(ch) == 8) {
            m_baseDataOffset = getAs<std::uint64_t>();
            if (m_tfFlags & TfFlag_SampleDescriptionIndex) {
                m_state = State::SampleDescriptionIndex;
            } else if (m_tfFlags & TfFlag_DefaultSampleDuration) {
                m_state = State::DefaultSampleDuration;
            } else if (m_tfFlags & TfFlag_DefaultSampleSize) {
                m_state = State::DefaultSampleSize;
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                m_state = State::DefaultSampleFlags;
            } else {
                m_state = State::Done;
            }
            m_pos = 0;
        }
        break;
    case State::SampleDescriptionIndex:
        if (putChar(ch) == 4) {
            m_sampleDescriptionIndex = getAs<std::uint32_t>();
            if (m_tfFlags & TfFlag_DefaultSampleDuration) {
                m_state = State::DefaultSampleDuration;
            } else if (m_tfFlags & TfFlag_DefaultSampleSize) {
                m_state = State::DefaultSampleSize;
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                m_state = State::DefaultSampleFlags;
            } else {
                m_state = State::Done;
            }
            m_pos = 0;
        }
        break;
    case State::DefaultSampleDuration:
        if (putChar(ch) == 4) {
            m_defaultSampleDuration = getAs<std::uint32_t>();
            if (m_tfFlags & TfFlag_DefaultSampleSize) {
                m_state = State::DefaultSampleSize;
            } else if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                m_state = State::DefaultSampleFlags;
            } else {
                m_state = State::Done;
            }
            m_pos = 0;
        }
        break;
    case State::DefaultSampleSize:
        if (putChar(ch) == 4) {
            m_defaultSampleSize = getAs<std::uint32_t>();
            if (m_tfFlags & TfFlag_DefaultSampleFlags) {
                m_state = State::DefaultSampleFlags;
            } else {
                m_state = State::Done;
            }
            m_pos = 0;
        }
        break;
    case State::DefaultSampleFlags:
        if (putChar(ch) == 4) {
            m_defaultSampleFlags = getAs<std::uint32_t>();
            m_state = State::Done;
        }
        break;
    case State::Done:
        break;
    }
}

std::ostream& TrackFragmentHeaderBoxParser::printDetails(std::ostream& os) const
{
    os << "tfFlags: 0x" << std::hex << m_tfFlags
       << " trackId: " << std::dec << m_trackId;
    if (m_baseDataOffset) os << " baseDataOffset: " << std::dec << *m_baseDataOffset;
    if (m_sampleDescriptionIndex) os << " sampleDescriptionIndex: " << std::dec << *m_sampleDescriptionIndex;
    if (m_defaultSampleDuration) os << " defaultSampleDuration: " << std::dec << *m_defaultSampleDuration;
    if (m_defaultSampleSize) os << " defaultSampleSize: " << std::dec << *m_defaultSampleSize;
    if (m_defaultSampleFlags) os << " defaultSampleFlags: 0x" << std::hex << *m_defaultSampleFlags;
    return os;
}

} // namespace
