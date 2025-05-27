cmake_minimum_required(VERSION 3.19)

project(Core)

file(GLOB CORE_SOURCES "*.cpp")

add_library(Core STATIC ${CORE_SOURCES})

target_include_directories(Core
	PUBLIC
		${CMAKE_SOURCE_DIR}/Include/Core
		${CMAKE_SOURCE_DIR}/Include/Math
		${CMAKE_SOURCE_DIR}/Include/IO
)

#file(GLOB CORE_TEST_SOURCES "Test/*.cpp")
include(GoogleTest)
include(CTest)
enable_testing()

add_executable(
     core_test
#     CORE_TEST_SOURCES
    Test/Entity_Test.cpp
)

target_link_libraries (
    core_test
    GTest::gtest_main
    Core
    Math
    IO
)

gtest_discover_tests(core_test)
