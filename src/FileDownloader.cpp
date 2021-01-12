
#include "FileDownloader.h"

#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace mp4dump {

FileDownloader::FileDownloader(const std::string& path)
    : m_fp{std::fopen(path.c_str(), "r")}
{
    if (!m_fp) {
        const auto msg = std::strerror(errno);
        std::ostringstream os;
        os << "Failed to open input file '" << path << "' (" << msg << ")";
        throw std::runtime_error(os.str());
    }
}

FileDownloader::~FileDownloader()
{
    if (m_fp) {
        std::fclose(m_fp);
    }
}

void FileDownloader::downloadStream(DataCallback dataCallback)
{
    std::vector<std::uint8_t> buffer(4096); // TODO: Make it configureable
    std::rewind(m_fp);
    while (true) {
        const auto rc = std::fread(buffer.data(), 1, buffer.size(), m_fp);
        if (rc > 0) {
            if (dataCallback) {
                dataCallback(DataView(buffer.data(), rc));
            }
        } else if (feof(m_fp)) {
            break;
        } else if (ferror(m_fp)) {
            const auto msg = std::strerror(errno);
            std::ostringstream os;
            os << "Failed to read input file (" << msg << ")";
            throw std::runtime_error(os.str());
        }
    }
}

} // namespace
