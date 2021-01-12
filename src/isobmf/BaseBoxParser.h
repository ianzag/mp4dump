
#pragma once

#include "IsoBmfInterfaces.h"

#include <iostream>
#include <string>

namespace isobmf {

/**
 * @brief Base class for all box parsers
 *
 * Handle common box properties like name, size, parent etc.
*/
class BaseBoxParser : public BoxParser
{
public:
    BaseBoxParser() = default;

    BaseBoxParser(std::string boxName, BoxSize boxSize, const BoxParser* parentBox)
        : m_boxName{std::move(boxName)}
        , m_boxSize{boxSize}
        , m_parentBox{parentBox}
    {}

    const std::string& getName() const noexcept override
    {
        return m_boxName;
    }

    BoxSize getSize() const noexcept override
    {
        return m_boxSize;
    }

    unsigned int getLevel() const noexcept override
    {
        return m_parentBox ? m_parentBox->getLevel() + 1 : 0;
    }

    /**
     * @brief Print indent string depending on box position inside box tree
     * @param os Output stream
     * @return Output stream
     */
    std::ostream& printIndent(std::ostream& os) const
    {
        auto level = getLevel();
        for (; level > 0; --level) {
            os << "  ";
        }
        return os;
    }

private:
    std::string m_boxName;
    BoxSize m_boxSize = 0;
    const BoxParser* m_parentBox = nullptr;
};

} // namespace
