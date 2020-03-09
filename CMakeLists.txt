cmake_minimum_required(VERSION 3.1...3.17)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# ==================================================================================================
# Project
# ==================================================================================================
project(nano_project)

# ==================================================================================================
# Options
# ==================================================================================================
option(NANO_BUILD_SHARED_LIBS "Build as a shared library" OFF)
option(NANO_ENABLE_LTO "Enable link-time optimizations if supported by the compiler" OFF)

if(WIN32)
    option(NANO_USE_STATIC_CRT "Link against the static runtime libraries" ON)
endif()

if(APPLE)
    option(NANO_SUPPORTS_METAL "Support Metal" ON)
endif()

if(WEBGL)
    option(NANO_USE_WEBGL2 "Use WebGL2.0" ON)
endif()

# ==================================================================================================
# Build Type
# ==================================================================================================
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as no build type was specified")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type (Debug/Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

# ==================================================================================================
# Support for ccache
# ==================================================================================================
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(C_LAUNCHER   "${CCACHE_PROGRAM}")
    set(CXX_LAUNCHER "${CCACHE_PROGRAM}")

    configure_file(build/launch-c.in   launch-c)
    configure_file(build/launch-cxx.in launch-cxx)

    execute_process(COMMAND chmod a+rx
        "${CMAKE_BINARY_DIR}/launch-c"
        "${CMAKE_BINARY_DIR}/launch-cxx"
    )

    if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(CMAKE_XCODE_ATTRIBUTE_CC         "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_XCODE_ATTRIBUTE_CXX        "${CMAKE_BINARY_DIR}/launch-cxx")
        set(CMAKE_XCODE_ATTRIBUTE_LD         "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS "${CMAKE_BINARY_DIR}/launch-cxx")
    else()
        set(CMAKE_C_COMPILER_LAUNCHER        "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_CXX_COMPILER_LAUNCHER      "${CMAKE_BINARY_DIR}/launch-cxx")
    endif()
endif()

# ==================================================================================================
# Link time optimizations (LTO)
# ==================================================================================================
if(NANO_ENABLE_LTO)
    include(CheckIPOSupported)

    check_ipo_supported(RESULT IPO_SUPPORT)

    if(IPO_SUPPORT)
        message(STATUS "LTO support is enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# ==================================================================================================
# OS specific
# ==================================================================================================
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    if(${NANO_USE_STATIC_CRT})
        set(CRT_FLAGS_RELEASE "/MT")
        set(CRT_FLAGS_DEBUG "/MTd")
    else()
        set(CRT_FLAGS_RELEASE "/MD")
        set(CRT_FLAGS_DEBUG "/MDd")
    endif()

    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CRT_FLAGS_RELEASE} /Z7")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CRT_FLAGS_RELEASE} /Z7")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CRT_FLAGS_RELEASE} /Zi")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CRT_FLAGS_RELEASE} /Zi")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CRT_FLAGS_DEBUG} /Z7")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CRT_FLAGS_DEBUG} /Z7")
endif()

# ==================================================================================================
# General compiler flags
# ==================================================================================================
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
        set(CLANG_CL true)
    endif()
elseif(MSVC)
    set(MSVC_NATIVE true)
endif()

set(CXX_STANDARD "-std=c++17")
if(WIN32)
    set(CXX_STANDARD "/std:c++17")
endif()

if(MSVC_NATIVE)
    set(CXX_STANDARD "/std:c++latest")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD} /W0 /Zc:__cplusplus")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD} -fstrict-aliasing -Wno-unknown-pragmas -Wno-unused-function")
endif()

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES=1")
endif()

if(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    link_libraries("-static-libgcc -static-libstdc++")
    link_libraries(libc++.a)
    link_libraries(libc++abi.a)
endif()

if(CYGWIN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
endif()

if(CLANG_CL OR MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE")
endif()

if(UNIX AND CMAKE_GENERATOR STREQUAL "Ninja")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif()

# ==================================================================================================
# Release compiler flags
# ==================================================================================================
if(NOT CLANG_CL AND NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer -ffunction-sections -fdata-sections")
endif()

if(ANDROID OR WEBGL)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti")
endif()

if(WEBGL)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")
endif()

# ==================================================================================================
# Debug compiler flags
# ==================================================================================================
if(NOT CLANG_CL AND NOT MSVC AND NOT WEBGL)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fstack-protector")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_SANITIZE_OPTIONS}")

if(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -fno-stack-check")
endif()

# ==================================================================================================
# Linker flags
# ==================================================================================================
if(NOT WEBGL)
    set(GC_SECTIONS "-Wl,--gc-sections")
endif()
set(B_SYMBOLIC_FUNCTIONS "-Wl,-Bsymbolic-functions")

if(APPLE)
    set(GC_SECTIONS "-Wl,-dead_strip")
    set(B_SYMBOLIC_FUNCTIONS "")
    set(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
    set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
    set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> qc -S <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qc -S <TARGET> <LINK_FLAGS> <OBJECTS>")
elseif (CLANG_CL)
    set(GC_SECTIONS "")
    set(B_SYMBOLIC_FUNCTIONS "")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GC_SECTIONS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GC_SECTIONS} ${B_SYMBOLIC_FUNCTIONS}")

if(WEBGL)
    if(${NANO_USE_WEBGL2})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1")
    endif()
endif()

# ==================================================================================================
# Sub-projects
# ==================================================================================================
add_subdirectory(nano)

if(WIN32)
    add_subdirectory(app)
endif()