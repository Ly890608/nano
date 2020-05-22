cmake_minimum_required(VERSION 3.1...3.17)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# ==================================================================================================
# Include custom cmake modules
# ==================================================================================================
include(cmake_module/custom.cmake)

# ==================================================================================================
# Project
# ==================================================================================================
set(PROJECT_NAME nano_project)
project(${PROJECT_NAME})

# ==================================================================================================
# Options
# ==================================================================================================
option(NANO_BUILD_SHARED_LIBS "Build as a shared library" OFF)
option(NANO_BUILD_APP "Build app" ON)
option(NANO_ENABLE_POSITION_INDEPENDENT_CODE "Enable Position independent code for SHARED or MODULE libraries" ON)
option(NANO_ENABLE_CCACHE "Enable Ccache Program if supported by the compiler" ON)
option(NANO_ENABLE_LTO "Enable link-time optimizations if supported by the compiler" ON)

if(WIN32)
    option(NANO_USE_STATIC_CRT "Link against the static runtime libraries" ON)
endif()

# ==================================================================================================
# Build Type
# ==================================================================================================
set(NANO_DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${NANO_DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${NANO_DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ==================================================================================================
# Position independent code for SHARED or MODULE libraries
# ==================================================================================================
if(NANO_ENABLE_POSITION_INDEPENDENT_CODE)
    message(STATUS "Position independent code is enabled.")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# ==================================================================================================
# Support for ccache
# ==================================================================================================
if(NANO_ENABLE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "CCache is enabled.")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    endif()
endif()

# ==================================================================================================
# Link time optimizations (LTO)
# ==================================================================================================
if(NANO_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORT)
    if(IPO_SUPPORT)
        message(STATUS "LTO support is enabled.")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# ==================================================================================================
# IDE Friendly
# ==================================================================================================
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ==================================================================================================
# Set output directories
# ==================================================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output/)

# ==================================================================================================
# Set runtime path
# ==================================================================================================
set(CMAKE_SKIP_BUILD_RPATH            FALSE) # Add absolute path to all dependencies for BUILD
set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE) # Use CMAKE_INSTALL_RPATH for INSTALL
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # Do NOT add path to dependencies for INSTALL

# ==================================================================================================
# Sub-projects
# ==================================================================================================
add_subdirectory(nano)

if(NANO_BUILD_APP)
    add_subdirectory(app)
endif()
