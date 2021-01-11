
set(APP_NAME mp4dump)

set(SOURCE_FILES
    main.cpp
    Application.h
    Application.cpp
    DataView.h
    StreamDownloader.h
    StreamDownloader.cpp
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
)

find_library(CURL curl)

set(LIBRARY_FILES
    ${CURL}
)

add_executable(${APP_NAME} ${SOURCE_FILES})
target_link_libraries(${APP_NAME} ${LIBRARY_FILES})
