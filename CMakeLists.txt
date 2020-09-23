cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(HUNTER_PACKAGES minizip)
include(FetchContent)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

function(InstallVendor project_name git_repo)
        IF (NOT ${project_name})
                FetchContent_Declare(
                        ${project_name}
                        GIT_REPOSITORY ${git_repo}
                        SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/${project_name}"
                        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
                        SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}-sub"
                )
                FetchContent_MakeAvailable(${project_name})
                set(${project_name} CACHE INTERNAL "")
                message(STATUS "Found ${project_name}")
        endif()
endfunction()

set(This Zip)
project(${This} LANGUAGES C CXX)

InstallVendor(Core https://CorrupD:hiddenhand@git.nipqer.nl/CorrupD/Core.git)
find_package(minizip CONFIG REQUIRED)

set(Headers
        ${PROJECT_SOURCE_DIR}/include/Zip.hpp)
set(Sources
        ${PROJECT_SOURCE_DIR}/src/Zip.cpp)
        
add_library(${This} ${Headers} ${Sources})
target_link_libraries(${This} PUBLIC Core minizip::minizip)
target_include_directories(${This} PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
        target_compile_definitions(${This} PUBLIC BUILD _CRT_SECURE_NO_WARNINGS)
endif()