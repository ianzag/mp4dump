
#pragma once

#include "IsoBmfTypes.h"

#include <ostream>
#include <memory>

namespace isobmf {

/**
 * @brief Interface for box parsers
 */
class BoxParser
{
public:
    virtual ~BoxParser() = default;

    /**
     * @brief Get descriptive box name
     * @return Box name
     */
    virtual const std::string& getName() const noexcept = 0;

    /**
     * @brief Get box size including header
     * @return Box size, bytes
     */
    virtual BoxSize getSize() const noexcept = 0;

    /**
     * @brief Get box level inside tree of boxes
     * @return Box level
     */
    virtual unsigned int getLevel() const noexcept = 0;

    /**
     * @brief Called on parser startup
     */
    virtual void startParse() = 0;

    /**
     * @brief Feed input data to parser
     * @param ch Data to feed
     */
    virtual void parseChar(std::uint8_t ch) = 0;

    /**
     * @brief Called when there's no more data to parse
     */
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

/**
 * @brief Interface to parse documents
 */
class DocumentParser
{
public:
    virtual ~DocumentParser() = default;

    /**
     * @brief Called on parser startup
     */
    virtual void startParse() = 0;

    /**
     * @brief Feed input data to parser
     * @param ch Data to feed
     */
    virtual bool parseChar(std::uint8_t ch) = 0;

    /**
     * @brief Called when there's no more data to parse
     */
    virtual bool endParse() = 0;

    /**
     * @brief Print parsed document information into output stream
     * @param os Output stream
     * @return Output stream
     */
    virtual std::ostream& printDetails(std::ostream& os) const = 0;
};

} // namespace
