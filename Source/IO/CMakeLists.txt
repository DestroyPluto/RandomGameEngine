cmake_minimum_required(VERSION 3.19)

project(IO)

# Find the PNG package 
find_package(PNG REQUIRED)

file (GLOB IO_SOURCES "*.cpp")

add_library(IO STATIC ${IO_SOURCES})

target_include_directories(IO
	PUBLIC
		${CMAKE_SOURCE_DIR}/Include/Core
		${CMAKE_SOURCE_DIR}/Include/Math
        ${CMAKE_SOURCE_DIR}/Include/IO
)
