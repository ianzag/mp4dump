
#pragma once

#include "IsoBmfTypes.h"

#include <memory>

namespace isobmf {

/**
 * @brief Interface for box parsers
 */
class BoxParser
{
public:
    virtual ~BoxParser() = default;

    virtual const std::string& getName() const noexcept = 0;
    virtual BoxSize getSize() const noexcept = 0;
    virtual unsigned int getLevel() const noexcept = 0;

    virtual void startParse() = 0;
    virtual void parseChar(std::uint8_t ch) = 0;
    virtual void endParse() = 0;
};

/**
 * @brief Interface to create box parsers
 */
class ParserFactory
{
public:
    virtual ~ParserFactory() = default;

    /**
     * @brief Create box parser for given box type
     * @param boxType Box type
     * @param boxSize Box size, bytes
     * @param parentBox Parent box (can be null)
     * @return New parser instance on success or null if box type is unknown
     */
    virtual std::unique_ptr<BoxParser> createParser(
            BoxType boxType,
            BoxSize boxSize,
            const BoxParser* parentBox) const = 0;

    /**
     * @brief Create default parser for unknown box type
     * @param boxType Box type
     * @param boxSize Box size, bytes
     * @param parentBox Parent box (can be null)
     * @return New parser instance (always succeeded)
     */
    virtual std::unique_ptr<BoxParser> createUnknownParser(
            BoxType boxType,
            BoxSize boxSize,
            const BoxParser* parentBox) const = 0;
};

} // namespace
