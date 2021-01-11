
#include "ContainerBoxParser.h"

#include <iostream>
#include <sstream>

namespace isobmf {

void ContainerBoxParser::startParse()
{
    printIndent(std::cout) << "Box type: '" << getName() << "' size: " << std::dec << getSize() << " {" << std::endl;
}

void ContainerBoxParser::endParse()
{
    if (m_childBoxParser) {
        m_childBoxParser->endParse();
        m_childBoxParser.reset();
    }

    printIndent(std::cout) << "}" << std::endl;
}

void ContainerBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::CompactSize:
        if (putChar(ch) == 4) {
            m_childBoxSize = getAs<std::uint32_t>();
            if (m_childBoxSize == 1) { // Large size is following
                m_state = State::ExtendedSize;
            } else if (m_childBoxSize == 0) {
                m_state = State::Type;
                m_dataLeft = 0; // Data spans till the end of file
            } else if (m_childBoxSize >= 8) {
                m_state = State::Type;
                m_dataLeft = m_childBoxSize - 8;
            } else {
                std::ostringstream os;
                os << "Failed to parse box (Invalid box size " << m_childBoxSize << ")";
                throw std::runtime_error(os.str());
            }
            m_pos = 0;
        }
        break;
    case State::ExtendedSize:
        if (putChar(ch) == 8) {
            m_childBoxSize = getAs<std::uint64_t>();
            if (m_childBoxSize == 0) {
                m_state = State::Type;
                m_dataLeft = 0; // Data spans till the end of file
            } else if (m_childBoxSize >= 16) {
                m_state = State::Type;
                m_dataLeft = m_childBoxSize - 16;
            } else {
                std::ostringstream os;
                os << "Failed to parse box (Invalid box size " << m_childBoxSize << ")";
                throw std::runtime_error(os.str());
            }
            m_pos = 0;
        }
        break;
    case State::Type:
        if (putChar(ch) == 4) {
            const auto boxType = getAs<std::uint32_t>();
            m_childBoxParser = m_boxFactory.createParser(boxType, m_childBoxSize, this);
            if (!m_childBoxParser) {
                m_childBoxParser = m_boxFactory.createUnknownParser(boxType, m_childBoxSize, this);
            }
            m_childBoxParser->startParse();
            if (!m_childBoxSize || m_dataLeft) {
                m_state = State::Data;
            } else { // No data for this child box
                m_childBoxParser->endParse();
                m_childBoxParser.reset();
                m_state = State::CompactSize;
            }
            m_pos = 0;
        }
        break;
    case State::Data:
        // Container should pass all data to its children boxes
        if (m_childBoxParser) {
            m_childBoxParser->parseChar(ch);
        }
        if (m_childBoxSize && --m_dataLeft == 0) { // No more data for this child box
            m_childBoxParser->endParse();
            m_childBoxParser.reset();
            m_state = State::CompactSize;
            m_pos = 0;
        }
        break;
    }
}

} // namespace
