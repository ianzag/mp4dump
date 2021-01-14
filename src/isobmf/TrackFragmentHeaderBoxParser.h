
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

    void startParse() override;
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

    void switchState(State newState) noexcept
    {
        m_state = newState;
        m_parser.reset();
    }

    using TfFlags                   = std::uint32_t;
    using TrackId                   = std::uint32_t;
    using BaseDataOffset            = std::uint64_t;
    using SampleDescriptionIndex    = std::uint32_t;
    using DefaultSampleDuration     = std::uint32_t;
    using DefaultSampleSize         = std::uint32_t;
    using DefaultSampleFlags        = std::uint32_t;

    enum : TfFlags
    {
        TfFlag_BaseDataOffset           = (1 << 0),
        TfFlag_SampleDescriptionIndex   = (1 << 1),
        TfFlag_DefaultSampleDuration    = (1 << 3),
        TfFlag_DefaultSampleSize        = (1 << 4),
        TfFlag_DefaultSampleFlags       = (1 << 5),
    };

    utils::BinaryParser<8> m_parser;
    State m_state;
    TfFlags                                 m_tfFlags;
    TrackId                                 m_trackId;
    std::optional<BaseDataOffset>           m_baseDataOffset;
    std::optional<SampleDescriptionIndex>   m_sampleDescriptionIndex;
    std::optional<DefaultSampleDuration>    m_defaultSampleDuration;
    std::optional<DefaultSampleSize>        m_defaultSampleSize;
    std::optional<DefaultSampleFlags>       m_defaultSampleFlags;
};

} // namespace
