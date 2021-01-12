
#pragma once

#include "StreamDownloader.h"

#include <cstdio>
#include <string>

namespace mp4dump {

class FileDownloader final : public StreamDownloader
{
public:
    explicit FileDownloader(const std::string& path);
    ~FileDownloader() override;

    void downloadStream(DataCallback dataCallback) override;

private:
    FILE *m_fp = nullptr;
};

} // namespace
