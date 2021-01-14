
#pragma once

#include "FullBoxParser.h"
#include "utils/BinaryParser.h"

#include <optional>

namespace isobmf {

/*
8.8.8  Track Fragment Run Box
8.8.8.1  Definition
Box Type:   ‘trun’
Container:  Track Fragment Box ('traf')
Mandatory:  No
Quantity:   Zero or more

8.8.8.2  Syntax
aligned(8) class TrackRunBox extends FullBox(‘trun’, version, tr_flags) {
    unsigned int(32) sample_count;
    // the following are optional fields
    signed int(32) data_offset;
    unsigned int(32) first_sample_flags;
    // all fields in the following array are optional
    {
        unsigned int(32) sample_duration;
        unsigned int(32) sample_size;
        unsigned int(32) sample_flags
        if (version == 0) {
            unsigned int(32) sample_composition_time_offset;
        } else {
            signed int(32) sample_composition_time_offset;
        }
    }[ sample_count ]
}
*/
class TrackFragmentRunBoxParser final : public FullBoxParser
{
public:
    TrackFragmentRunBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Track Fragment Run", boxSize, parentBox) {}

    void startParse() override;
    void parseChar(std::uint8_t ch) override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    enum class State
    {
        TrFlags,
        SampleCount,
        DataOffset,
        FirstSampleFlags,
        Done,
    };

    using TrFlags          = std::uint32_t;
    using SampleCount      = std::uint32_t;
    using DataOffset       = std::int32_t;
    using FirstSampleFlags = std::uint32_t;

    enum : TrFlags
    {
        TrFlag_DataOffset       = (1 << 0),
        TrFlag_FirstSampleFlags = (1 << 2),
    };

    State                       m_state;
    utils::BinaryParser<4>      m_parser;
    TrFlags                     m_trFlags;
    SampleCount                 m_sampleCount;
    std::optional<DataOffset>   m_dataOffset;
    std::optional<FirstSampleFlags> m_firstSampleFlags;
};

} // namespace
