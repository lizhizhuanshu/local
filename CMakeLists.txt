cmake_minimum_required(VERSION 3.13) # CMake install : https://cmake.org/download/
project(local LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/home/lizhi/Qt/6.8.1/gcc_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Widgets Gui  OpenGL OpenGLWidgets  REQUIRED) # Qt COMPONENTS
aux_source_directory(./src srcs)


find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)


# 包含 FFmpeg 头文件路径
include_directories(${FFMPEG_INCLUDE_DIRS})

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs} 
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets 
  Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets  ${FFMPEG_LIBRARIES}) # Qt5 Shared Library
