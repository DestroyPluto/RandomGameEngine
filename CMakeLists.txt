cmake_minimum_required(VERSION 3.19)

project(Mercury)

#TODO: implement this properly
file(GLOB SOURCES "Source/*.cpp"
    "Source/*.c"
    )

#set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(GLFW_BUILD_WAYLAND false)
endif(UNIX)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Include
        ${PROJECT_SOURCE_DIR}/Include/glad
        ${PROJECT_SOURCE_DIR}/Include/KHR
        ${CMAKE_SOURCE_DIR}/Include/RenderingGL
	    ${CMAKE_SOURCE_DIR}/Include/Core
	    ${CMAKE_SOURCE_DIR}/Include/Math
        ${CMAKE_SOURCE_DIR}/Include/IO
        

)

# Specify the path to GLFW
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_DIR})

add_subdirectory("${CMAKE_SOURCE_DIR}/Source/RenderingGL")
add_subdirectory("${CMAKE_SOURCE_DIR}/Source/Core")
add_subdirectory("${CMAKE_SOURCE_DIR}/Source/Math")
add_subdirectory("${CMAKE_SOURCE_DIR}/Source/IO")


find_package( OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} 
glfw
RenderingGL
Core
Math
IO
)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
