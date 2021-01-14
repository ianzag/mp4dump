
#include "ContainerBoxParser.h"

#include <iostream>
#include <sstream>

namespace isobmf {

void ContainerBoxParser::startParse()
{
    printIndent(std::cout) << "Box type: '" << getName() << "' size: " << std::dec << getSize() << " {" << std::endl;

    switchState(State::CompactSize);
}

void ContainerBoxParser::endParse()
{
    // Notify child parser we've finished
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
        if (m_parser.putChar(ch) == sizeof(CompactSize)) {
            m_childBoxSize = m_parser.getAs<CompactSize>();
            if (m_childBoxSize == 1) { // Large size is following
                switchState(State::ExtendedSize);
            } else if (m_childBoxSize == 0) {
                switchState(State::BoxType);
                m_dataLeft = 0; // Data spans till the end of file
            } else if (m_childBoxSize >= sizeof(CompactSize) + sizeof(BoxType)) {
                switchState(State::BoxType);
                m_dataLeft = m_childBoxSize - sizeof(CompactSize) - sizeof(BoxType);
            } else {
                std::ostringstream os;
                os << "Failed to parse box (Invalid box size " << m_childBoxSize << ")";
                throw std::runtime_error(os.str());
            }
        }
        break;
    case State::ExtendedSize:
        if (m_parser.putChar(ch) == sizeof(ExtendedSize)) {
            m_childBoxSize = m_parser.getAs<ExtendedSize>();
            if (m_childBoxSize == 0) {
                switchState(State::BoxType);
                m_dataLeft = 0; // Data spans till the end of file
            } else if (m_childBoxSize >= sizeof(CompactSize) + sizeof(ExtendedSize) + sizeof(BoxType)) {
                switchState(State::BoxType);
                m_dataLeft = m_childBoxSize - sizeof(CompactSize) - sizeof(ExtendedSize) - sizeof(BoxType);
            } else {
                std::ostringstream os;
                os << "Failed to parse box (Invalid box size " << m_childBoxSize << ")";
                throw std::runtime_error(os.str());
            }
        }
        break;
    case State::BoxType:
        if (m_parser.putChar(ch) == sizeof(BoxType)) {
            const auto boxType = m_parser.getAs<BoxType>();
            m_childBoxParser = m_boxFactory.createParser(boxType, m_childBoxSize, this);
            if (!m_childBoxParser) {
                m_childBoxParser = m_boxFactory.createUnknownParser(boxType, m_childBoxSize, this);
            }
            m_childBoxParser->startParse();
            if (!m_childBoxSize || m_dataLeft) {
                switchState(State::BoxData);
            } else { // No data for this child box
                m_childBoxParser->endParse();
                m_childBoxParser.reset();
                switchState(State::CompactSize);
            }
        }
        break;
    case State::BoxData:
        // Container should pass all data to its children boxes
        if (m_childBoxParser) {
            m_childBoxParser->parseChar(ch);
        }
        if (m_childBoxSize && --m_dataLeft == 0) { // No more data for this child box
            m_childBoxParser->endParse();
            m_childBoxParser.reset();
            switchState(State::CompactSize);
        }
        break;
    }
}

} // namespace
