cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(HUNTER_PACKAGES minizip)
include(FetchContent)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

set(This Zip)
project(${This} LANGUAGES C CXX)

find_package(minizip CONFIG REQUIRED)

set(Headers
        ${PROJECT_SOURCE_DIR}/include/Zip.hpp)
set(Sources
        ${PROJECT_SOURCE_DIR}/src/Zip.cpp)
        
add_library(${This} ${Headers} ${Sources})
target_link_libraries(${This} PUBLIC minizip::minizip)
target_include_directories(${This} PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
        target_compile_definitions(${This} PUBLIC BUILD _CRT_SECURE_NO_WARNINGS)
endif()