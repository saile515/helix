cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

include(cmake/Vcpkg.cmake) # needs to happen before project()

project(
    "helix"
    VERSION 0.0.1
    LANGUAGES CXX
)

include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Git.cmake)

# below are options which should be changed

### SETTINGS ###

# add all headers (.h, .hpp) to this
set(PRJ_HEADERS include/helix/Window.h)
# add all source files (.cpp) to this, except the one with main()
set(PRJ_SOURCES src/Window.cpp)
# set include paths not part of libraries
set(PRJ_INCLUDE_DIRS include/helix)
# set compile features (e.g. standard version)
set(PRJ_COMPILE_FEATURES cxx_std_20)
# set #defines (test enable/disable not included here)
set(PRJ_DEFINITIONS )
# add all libraries used by the project (WARNING: also set them in vcpkg.json!)
set(PRJ_LIBRARIES 
    fmt::fmt
    glfw
    GLEW::GLEW
    Threads::Threads
)

# add dependency find_package calls and similar here
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
# to enable multithreading and the Threads::Threads dependency
include(FindThreads)

### END SETTINGS ###

# DONT change anything beyond this point unless you've read the cmake bible and 
# swore on it not to bonk up the ci/cd pipelines with your changes.

####################


# enables compile_commands.json for clang-related tools (such as the clang LS)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# build debug builds by default (if not specified otherwise)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(UNIX)
    # this will allow to use same _DEBUG macro available in both Linux as well as Windows - MSCV environment. Easy to put Debug specific code.
    add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")
endif(UNIX)

include(cmake/CompilerWarnings.cmake)

# set MT library for msvc - this is required (says documentation)
# linux/mac/etc should simply ignore this by default.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set(PRJ_DEFINITIONS ${PRJ_DEFINITIONS}
    PRJ_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PRJ_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    PRJ_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    PRJ_GIT_HASH="${PRJ_GIT_HASH}"
)


add_library(${PROJECT_NAME} SHARED ${PRJ_HEADERS} ${PRJ_SOURCES} ${PRJ_MAIN})
target_include_directories(${PROJECT_NAME} PUBLIC ${PRJ_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PRJ_LIBRARIES})
target_compile_features(${PROJECT_NAME} PRIVATE ${PRJ_COMPILE_FEATURES})

# setup all warnings (from cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})