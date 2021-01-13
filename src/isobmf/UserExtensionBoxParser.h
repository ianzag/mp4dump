
#pragma once

#include "FullBoxParser.h"
#include "utils/BinaryParser.h"

namespace isobmf {

class UserExtensionBoxParser final : public FullBoxParser
{
public:
    UserExtensionBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("User Extension", boxSize, parentBox) {}

    void parseChar(std::uint8_t ch) override;
    std::ostream& printDetails(std::ostream& os) const override;

private:
    enum class State
    {
        UUID,
        Data,
    };

    utils::BinaryParser<16> m_parser;
    State m_state = State::UUID;
    std::array<std::uint8_t, 16> m_uuid;
    std::size_t m_dataSize = 0;
};

} // namespace
