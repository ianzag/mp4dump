
#pragma once

#include "IsoBmfInterfaces.h"

namespace isobmf {

/**
 * @brief Create boxes for ISO Base Media File Format
 */
class BaseParserFactory : public ParserFactory
{
public:
    std::unique_ptr<BoxParser> createParser(
            BoxType boxType,
            BoxSize boxSize,
            const BoxParser* parentBox) const override;

    std::unique_ptr<BoxParser> createUnknownParser(
            BoxType boxType,
            BoxSize boxSize,
            const BoxParser* parentBox) const override;
};

} // namespace
