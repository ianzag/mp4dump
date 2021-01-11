
#pragma once

#include "BaseBoxParser.h"

namespace isobmf {

/**
 * @brief Parse Full Box (generic implementation)
 */
class FullBoxParser : public BaseBoxParser
{
public:
    FullBoxParser(std::string boxName,
                  BoxSize boxSize,
                  const BoxParser* parentBox)
        : BaseBoxParser(std::move(boxName), boxSize, parentBox)
    {}

    void startParse() override {}
    void parseChar(std::uint8_t ch) override {}
    void endParse() override;

    virtual std::ostream& printDetails(std::ostream& os) const
    {
        return os;
    }
};

} // namespace
