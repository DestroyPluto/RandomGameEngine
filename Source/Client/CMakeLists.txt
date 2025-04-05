cmake_minimum_required(VERSION 3.19)

project(Client)

file (GLOB CLIENT_SOURCES "*.cpp")

add_library(Client STATIC ${CLIENT_SOURCES})

target_include_directories(Client
PUBLIC
    ${CMAKE_SOURCE_DIR}/Include
    ${CMAKE_SOURCE_DIR}/Include/Client
    ${CMAKE_SOURCE_DIR}/Include/Core
    ${CMAKE_SOURCE_DIR}/Include/Math

)

target_link_libraries(${PROJECT_NAME} 
	Core
)