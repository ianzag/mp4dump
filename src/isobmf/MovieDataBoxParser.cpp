
#include "MovieDataBoxParser.h"
#include "XmlDocumentParser.h"

namespace isobmf {

void MovieDataBoxParser::parseChar(std::uint8_t ch)
{
    switch (m_state) {
    case State::DetectFormat:
        // Keep track of received chars to pass it later into file handler
        m_buffer += ch;
        // Feed char into format detector and see what happens
        switch (m_formatDetector.putChar(ch)) {
        case FileFormat::InProgress:
            break;
        case FileFormat::Unknown:
            m_state = State::Done;
            break;
        case FileFormat::XML:
            m_documentParser = std::make_unique<XmlDocumentParser>();
            m_documentParser->startParse();
            for (const auto ch : m_buffer) {
                m_documentParser->parseChar(ch);
            }
            m_state = State::ParseDocument;
            break;
        }
        break;
    case State::ParseDocument:
        if (m_documentParser) {
            if (!m_documentParser->parseChar(ch)) {
                m_state = State::Done;
            }
        }
        break;
    case State::Done:
        break;
    }
}

void MovieDataBoxParser::endParse()
{
    // Notify document parser we've finished
    if (m_documentParser) {
        m_documentParser->endParse();
    }
    Parent::endParse();
}

std::ostream& MovieDataBoxParser::printDetails(std::ostream& os) const
{
    // Print document specific details
    if (m_documentParser) {
        m_documentParser->printDetails(os);
    }
    return os;
}

} // namespace
