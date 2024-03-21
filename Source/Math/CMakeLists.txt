cmake_minimum_required(VERSION 3.19)

project(Math)

file (GLOB MATH_SOURCES "*.cpp")

add_library(Math STATIC ${MATH_SOURCES})

target_include_directories(Math
	PUBLIC
		${CMAKE_SOURCE_DIR}/Include/Math
)