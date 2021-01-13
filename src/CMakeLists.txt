
set(APP_NAME mp4dump)

set(SOURCE_FILES
    main.cpp
    Application.h
    Application.cpp
    StreamDownloader.h
    DownloaderFactory.h
    DownloaderFactory.cpp
    FileDownloader.h
    FileDownloader.cpp
    HttpDownloader.h
    HttpDownloader.cpp
    utils/BinaryParser.h
    utils/Base64.h
    utils/Base64.cpp
    utils/StringConvert.h
    isobmf/IsoBmfTypes.h
    isobmf/IsoBmfInterfaces.h
    isobmf/IsoBmfUtils.h
    isobmf/BaseParserFactory.h
    isobmf/BaseParserFactory.cpp
    isobmf/BaseBoxParser.h
    isobmf/ContainerBoxParser.h
    isobmf/ContainerBoxParser.cpp
    isobmf/FullBoxParser.h
    isobmf/FullBoxParser.cpp
    isobmf/FileTypeBoxParser.h
    isobmf/FileTypeBoxParser.cpp
    isobmf/FreeSpaceBoxParser.h
    isobmf/MovieDataBoxParser.h
    isobmf/MovieDataBoxParser.cpp
    isobmf/MovieHeaderBoxParser.h
    isobmf/MovieHeaderBoxParser.cpp
    isobmf/MovieFragmentHeaderBoxParser.h
    isobmf/MovieFragmentHeaderBoxParser.cpp
    isobmf/TrackHeaderBoxParser.h
    isobmf/TrackHeaderBoxParser.cpp
    isobmf/TrackFragmentHeaderBoxParser.h
    isobmf/TrackFragmentHeaderBoxParser.cpp
    isobmf/TrackFragmentRunBoxParser.h
    isobmf/TrackFragmentRunBoxParser.cpp
    isobmf/UserExtensionBoxParser.h
    isobmf/UserExtensionBoxParser.cpp
    isobmf/FileFormatDetector.h
    isobmf/XmlDocumentParser.h
    isobmf/XmlDocumentParser.cpp
)

find_library(CURL curl)
find_library(EXPAT expat)

set(LIBRARY_FILES
    ${CURL}
    ${EXPAT}
)

add_executable(${APP_NAME} ${SOURCE_FILES})
target_link_libraries(${APP_NAME} ${LIBRARY_FILES})
