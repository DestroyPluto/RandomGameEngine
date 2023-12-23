cmake_minimum_required(VERSION 3.19)

# Tell cmake we want it to automate generating an export stub for the dll
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(RenderingGL)

file (GLOB RENDERING_GL_SOURCES "*.cpp")

add_library(RenderingGL SHARED ${RENDERING_GL_SOURCES})

target_include_directories(RenderingGL
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/RenderingGL
)
