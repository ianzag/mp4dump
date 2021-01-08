
set(APP_NAME mp4dump)

set(SOURCE_FILES
    mp4dump.cpp
    Application.h
    Application.cpp
    DataView.h
    StreamDownloader.h
    StreamDownloader.cpp
)

find_library(CURL curl)

set(LIBRARY_FILES
    ${CURL}
)

add_executable(${APP_NAME} ${SOURCE_FILES})
target_link_libraries(${APP_NAME} ${LIBRARY_FILES})
