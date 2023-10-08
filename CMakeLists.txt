cmake_minimum_required(VERSION 3.26)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
add_definitions(-DCMAKE_OSX_ARCHITECTURES=x86_64)

project(format_factory)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/liyang/Qt/5.15.2/clang_64/lib/cmake")
set(Qt5_DIR "${CMAKE_PREFIX_PATH}/Qt5")

set(FFMPEG_DIR "/usr/local/Cellar/ffmpeg/6.0_1")

file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")


include_directories("${FFMPEG_DIR}/include/")


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)



# 创建可执行文件并链接依赖库
add_executable(format_factory main.cpp ${HEADER_FILES} ${SOURCE_FILES})

target_link_directories(format_factory PUBLIC "${FFMPEG_DIR}/lib/")

target_link_libraries(format_factory
        PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        -lavcodec
        -lavdevice
        -lavfilter
        -lavformat
        -lavutil
        -lswresample
        -lswscale
)

