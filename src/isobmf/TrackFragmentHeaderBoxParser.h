
#pragma once

#include "FullBoxParser.h"
#include "utils/BinaryParser.h"

#include <optional>

namespace isobmf {

/*
8.8.7  Track Fragment Header Box
8.8.7.1  Definition
Box Type:   ‘tfhd’
Container:  Track Fragment Box ('traf')
Mandatory:  Yes
Quantity:   Exactly one

8.8.7.2  Syntax
aligned(8) class TrackFragmentHeaderBox extends FullBox(‘tfhd’, 0, tf_flags){
    unsigned int(32) track_ID;
    // all the following are optional fields
    unsigned int(64) base_data_offset;
    unsigned int(32) sample_description_index;
    unsigned int(32) default_sample_duration;
    unsigned int(32) default_sample_size;
    unsigned int(32) default_sample_flags
}
*/
class TrackFragmentHeaderBoxParser final : public FullBoxParser
{
public:
    TrackFragmentHeaderBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Track Fragment Header", boxSize, parentBox) {}

    void parseChar(std::uint8_t ch) override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    enum class State
    {
        TfFlags,
        TrackId,
        BaseDataOffset,
        SampleDescriptionIndex,
        DefaultSampleDuration,
        DefaultSampleSize,
        DefaultSampleFlags,
        Done,
    };

    enum : std::uint32_t
    {
        TfFlag_BaseDataOffset           = (1 << 0),
        TfFlag_SampleDescriptionIndex   = (1 << 1),
        TfFlag_DefaultSampleDuration    = (1 << 3),
        TfFlag_DefaultSampleSize        = (1 << 4),
        TfFlag_DefaultSampleFlags       = (1 << 5),
    };

    utils::BinaryParser<8> m_parser;
    State m_state = State::TfFlags;
    std::uint32_t m_tfFlags = 0;
    std::uint32_t m_trackId = 0;
    std::optional<std::uint64_t> m_baseDataOffset;
    std::optional<std::uint32_t> m_sampleDescriptionIndex;
    std::optional<std::uint32_t> m_defaultSampleDuration;
    std::optional<std::uint32_t> m_defaultSampleSize;
    std::optional<std::uint32_t> m_defaultSampleFlags;
};

} // namespace
