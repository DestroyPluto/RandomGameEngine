cmake_minimum_required(VERSION 3.19)

project(Core)

file (GLOB CORE_SOURCES "*.cpp")

add_library(Core STATIC ${CORE_SOURCES})

target_include_directories(Core
	PUBLIC
		${CMAKE_SOURCE_DIR}/Include/Core
)