cmake_minimum_required(VERSION 3.19)

project(RenderingGL)

file (GLOB RENDERING_GL_SOURCES "*.cpp")
#file (GLOB RENDERING_GL_INCLUDES "../../Include/RenderingGL/*.h")
add_library(RenderingGL SHARED ${RENDERING_GL_SOURCES})

target_include_directories(RenderingGL
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Include/RenderingGL
)
