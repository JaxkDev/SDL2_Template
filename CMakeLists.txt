set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

target_compile_options(project_warnings
        INTERFACE
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2)

# some GCC specific warnings. These flags are added only if the used compiler is GCC.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(project_warnings
            INTERFACE
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wlogical-op
            -Wuseless-cast
    )
    target_link_libraries(project_options INTERFACE stdc++fs)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options (project_options INTERFACE -fcolor-diagnostics)
endif ()

cmake_minimum_required(VERSION 3.20)
project(
    SDL_Test
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "SDL Test"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


set(ALL_SOURCES
        src/main.cpp
)

set(ALL_LIBRARIES
    project_warnings
    project_options
)

set(ALL_DLLS
)


### === Libraries ===
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## SDL2
set(SDL2_PATH ${CMAKE_SOURCE_DIR}/extern/SDL2-2.30.3/x86_64-w64-mingw32)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
list(APPEND ALL_LIBRARIES ${SDL2_LIBRARY})
if(WIN32)
    list(APPEND ALL_DLLS ${SDL2_PATH}/bin/SDL2.dll)
endif()

## OpenGL
find_package(OpenGL 3.3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
list(APPEND ALL_LIBRARIES ${OPENGL_LIBRARIES})

## GLM
set(GLM_PATH ${CMAKE_SOURCE_DIR}/extern/glm-1.0.1-light)
add_library(glm INTERFACE)
target_include_directories(glm PUBLIC INTERFACE ${GLM_PATH})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(glm PROPERTIES COMPILE_FLAGS "-w")
endif()

list(APPEND ALL_LIBRARIES glm)

## GLAD
set(GLAD_PATH ${CMAKE_SOURCE_DIR}/extern/glad)
add_library(glad ${GLAD_PATH}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_PATH}/include)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(glad PROPERTIES COMPILE_FLAGS "-w")
endif()

list(APPEND ALL_LIBRARIES glad)

####################

## Create executable
add_executable(SDL_Test ${ALL_SOURCES})

## Link all libraries
target_link_libraries(SDL_Test PRIVATE ${ALL_LIBRARIES})

## Copy all DLLs
if(WIN32)
    foreach(DLL ${ALL_DLLS})
        add_custom_command(TARGET SDL_Test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:SDL_Test>)
    endforeach()
endif()

message("#########################################")
message("                Summary                  ")
message("#########################################")
message("Build type:       \t ${CMAKE_BUILD_TYPE}")
message("C++ Compiler:     \t ${CMAKE_CXX_COMPILER_ID}")
message("C++ Compiler Path:\t ${CMAKE_CXX_COMPILER}")
message("C++ Compiler Flags:\t ${CMAKE_CXX_FLAGS}")
message("C++ Standard:     \t ${CMAKE_CXX_STANDARD}")
message("CMake Version:    \t ${CMAKE_VERSION}")
message("System:           \t ${CMAKE_SYSTEM} (${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR})")
message("Source Dir:       \t ${CMAKE_SOURCE_DIR}")
message("Binary Dir:       \t ${CMAKE_BINARY_DIR}")
message("------------------------------------------")
message("Sources:          \t ${ALL_SOURCES}")
message("Libraries:        \t ${ALL_LIBRARIES}")
message("DLLs:             \t ${ALL_DLLS}")
message("------------------------------------------")
message("Version:          \t ${PROJECT_VERSION}")
message("########################################")
