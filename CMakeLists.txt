cmake_minimum_required(VERSION 3.20)
project(
    SDL_Test
    VERSION 1.0.0
    DESCRIPTION "SDL Test"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(GLEW_PATH "glew-2.1.0")
set(GLEW_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/${GLEW_PATH}/include)
set(GLEW_LIBRARIES ${PROJECT_SOURCE_DIR}/${GLEW_PATH}/lib/Release/x64/glew32.lib)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})

add_executable(SDL_Test main.cpp)

target_link_libraries(SDL_Test ${SDL2_LIBRARY} ${GLEW_LIBRARIES} OpenGL32 glu32)

if(WIN OR WIN32)
    add_custom_command(
        TARGET SDL_Test
        COMMENT "Copy SDL2.dll"
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${SDL2_PATH}/bin/SDL2.dll ${PROJECT_BINARY_DIR}
        VERBATIM
    )
    add_custom_command(
        TARGET SDL_Test
        COMMENT "Copy glew32.dll"
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${GLEW_PATH}/bin/Release/x64/glew32.dll ${PROJECT_BINARY_DIR}
        VERBATIM
    )
ENDIF()

install(TARGETS SDL_Test)
