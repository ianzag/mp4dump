
#include "MovieDataBoxParser.h"
#include "XmlDocumentParser.h"

namespace isobmf {

void MovieDataBoxParser::startParse()
{
    m_state = State::DetectFormat;
    m_formatDetector.reset();
}

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
            m_state = State::ParseError;
            break;
        case FileFormat::XML:
            m_documentParser = std::make_unique<XmlDocumentParser>();
            m_documentParser->startParse();
            for (const auto ch : m_buffer) {
                if (!m_documentParser->parseChar(ch)) {
                    m_state = State::ParseError;
                    break;
                }
            }
            m_state = State::ParseDocument;
            break;
        }
        break;
    case State::ParseDocument:
        if (m_documentParser && !m_documentParser->parseChar(ch)) {
            m_state = State::ParseError;
        }
        break;
    case State::UnknownFormat:
    case State::ParseError:
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
    if (m_documentParser && m_state == State::ParseDocument) {
        switch (m_state) {
        case State::DetectFormat:
            os << "error: Insufficient data to detect document format";
            break;
        case State::UnknownFormat:
            os << "error: Unknown document format";
            break;
        case State::ParseDocument:
            m_documentParser->printDetails(os);
            break;
        case State::ParseError:
            os << "error: Failed to parse document";
            break;
        }
    }
    return os;
}

} // namespace
