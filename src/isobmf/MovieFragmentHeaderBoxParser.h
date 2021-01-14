
#pragma once

#include "FullBoxParser.h"
#include "utils/BinaryParser.h"

namespace isobmf {

/*
8.8.5  Movie Fragment Header Box
8.8.5.1  Definition
Box Type:   ‘mfhd’
Container:  Movie Fragment Box ('moof')
Mandatory:  Yes
Quantity:   Exactly one

8.8.5.2 Syntax
aligned(8) class MovieFragmentHeaderBox extends FullBox(‘mfhd’, 0, 0){
    unsigned int(32) sequence_number;
}
*/
class MovieFragmentHeaderBoxParser final : public FullBoxParser
{
public:
    MovieFragmentHeaderBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Movie Fragment Header", boxSize, parentBox) {}

    void startParse() override;
    void parseChar(std::uint8_t ch) override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    using ReservedField  = std::uint32_t;
    using SequenceNumber = std::uint32_t;

    enum class State
    {
        ReservedField,
        SequenceNumber,
        Done,
    };

    void switchState(State newState) noexcept
    {
        m_state = newState;
        m_parser.reset();
    }

    utils::BinaryParser<4> m_parser;
    State m_state = State::ReservedField;
    SequenceNumber m_sequenceNumber = 0;
};

} // namespace
