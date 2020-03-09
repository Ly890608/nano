cmake_minimum_required(VERSION 3.1...3.17)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# ==================================================================================================
# Project
# ==================================================================================================
set(TARGET nano)

project(${TARGET} 
		VERSION 1.0
		LANGUAGES CXX
		DESCRIPTION "nano" 
)

# ==================================================================================================
# Target definition
# ==================================================================================================
if(NANO_BUILD_SHARED_LIBS)
    add_library(${TARGET} SHARED)
	target_compile_definitions(${TARGET} PRIVATE NANO_EXPORT)
else()
    add_library(${TARGET} STATIC)
	target_compile_definitions(${TARGET} PUBLIC NANO_STATIC)
endif()

add_library(nano::${TARGET} ALIAS ${TARGET})

# ==================================================================================================
# Sources and headers
# ==================================================================================================
target_sources(${TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano.cpp
)

target_include_directories(${TARGET}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ==================================================================================================
# Dependencies
# ==================================================================================================
if(ANDROID)
    target_link_libraries(${TARGET} PUBLIC GLESv3 EGL android)
endif()

if(APPLE AND NOT IOS)
    target_link_libraries(${TARGET} PRIVATE "-framework Cocoa")
endif()

if(NANO_SUPPORTS_METAL)
    target_link_libraries(${TARGET} PUBLIC "-framework Metal -framework CoreVideo")
endif()

if(LINUX)
    target_link_libraries(${TARGET} PRIVATE dl)
endif()

# ==================================================================================================
# Compiler flags
# ==================================================================================================
target_compile_features(${TARGET} PRIVATE cxx_std_17)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC OR CLANG_CL)
    set(OPTIMIZATION_FLAGS
        /fp:fast
    )
elseif(WEBGL)
    set(OPTIMIZATION_FLAGS -fvisibility=hidden -fvisibility-inlines-hidden)
else()
    set(OPTIMIZATION_FLAGS
        -ffast-math
        -ffp-contract=fast
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fstrict-vtable-pointers
    )
endif()

set(LINUX_LINKER_OPTIMIZATION_FLAGS
        -Wl,--exclude-libs
)

set(LINUX_COMPILER_FLAGS
        -fPIC
)

if(MSVC_NATIVE)
    set(NANO_WARNINGS -W0)
else()
    set(NANO_WARNINGS
            -Wextra -Wno-unused-parameter
            -Wextra-semi -Wnewline-eof -Wdeprecated -Wundef
            -Wgnu-conditional-omitted-operand
            -Wweak-vtables -Wnon-virtual-dtor -Wclass-varargs -Wimplicit-fallthrough
            -Wover-aligned
    )
endif()

if(CLANG_CL)
    set(NANO_WARNINGS /W4 ${NANO_WARNINGS})
else()
    set(NANO_WARNINGS -Wall ${NANO_WARNINGS})
endif()

target_compile_options(${TARGET} PRIVATE
        ${NANO_WARNINGS}
        $<$<CONFIG:Release>:${OPTIMIZATION_FLAGS}>
        $<$<AND:$<PLATFORM_ID:Darwin>,$<CONFIG:Release>>:${DARWIN_OPTIMIZATION_FLAGS}>
        $<$<PLATFORM_ID:Linux>:${LINUX_COMPILER_FLAGS}>
)

target_link_libraries(${TARGET} PRIVATE
        $<$<AND:$<PLATFORM_ID:Linux>,$<CONFIG:Release>>:${LINUX_LINKER_OPTIMIZATION_FLAGS}>
)

# ==================================================================================================
# Installation
# ==================================================================================================
install(TARGETS ${TARGET}
        EXPORT ${TARGET}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )