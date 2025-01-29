cmake_minimum_required(VERSION 3.0)
project(scrabble_game C)
set(CMAKE_C_STANDARD 99)

# Include FetchContent module
include(FetchContent)

# Disable examples and games in raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# Fetch raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
)
FetchContent_MakeAvailable(raylib)

# Fetch raygui
FetchContent_Declare(
    raygui
    GIT_REPOSITORY "https://github.com/raysan5/raygui.git"
    GIT_TAG "master"
)
FetchContent_MakeAvailable(raygui)

# Set project source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Add executable
add_executable(${PROJECT_NAME}
        src/arbol_diccionario.h)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Include raygui header
target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR}/src)

# Define RAYGUI_IMPLEMENTATION
target_compile_definitions(${PROJECT_NAME} PRIVATE RAYGUI_IMPLEMENTATION)

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_PATH="./assets/")

# Copy assets and palabras.txt to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/palabras.txt"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/palabras.txt"
)
