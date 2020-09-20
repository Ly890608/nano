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
set(NANO_GRAPHICS_NOOP 0)
set(NANO_GRAPHICS_D3D12 1)

if(${NANO_BUILD_NOOP_GRAPHICS})
    message(STATUS "Nano Graphcis Type is Noop.")
    set(NANO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Graphics/Noop/GraphicsCore.cpp)
    set(NANO_GRAPHICS_TYPE ${NANO_GRAPHICS_NOOP})
elseif(WIN32)
    message(STATUS "Nano Graphcis Type is D3D12.")
    set(NANO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Graphics/D3D12/GraphicsCore.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Graphics/D3D12/GraphicsCore.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Graphics/D3D12/GraphicsUtils.h)
    set(NANO_GRAPHICS_TYPE ${NANO_GRAPHICS_D3D12})
else()
    message(STATUS "Nano Graphcis Type is Unknown, Set to Noop.")
    set(NANO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Graphics/Noop/GraphicsCore.cpp)
    set(NANO_GRAPHICS_TYPE ${NANO_GRAPHICS_NOOP})
endif()

if(WIN32)
    set(NANO_PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Platform/Windows/MinWindows.h)
else()
    set(NANO_PLATFORM_SRC)
endif()

target_sources(${TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/Core/Application.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/Core/BaseDefines.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/nano.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Core/Application.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Core/Log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Core/Log.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/Graphics/GraphicsCore.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano/nano.cpp
        ${NANO_GRAPHICS_SRC}
        ${NANO_PLATFORM_SRC}
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
if(${NANO_GRAPHICS_TYPE} EQUAL ${NANO_GRAPHICS_D3D12})
    target_link_libraries(${TARGET} PRIVATE "d3d12.lib" "dxgi.lib")
endif()

target_link_libraries(${TARGET} PRIVATE fmt::fmt)

# ==================================================================================================
# Compiler flags
# ==================================================================================================
target_compile_features(${TARGET} PRIVATE cxx_std_17)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(${NANO_BUILD_LOG})
    set(NANO_USE_LOG_VALUE 1)
else()
    set(NANO_USE_LOG_VALUE 0)
endif()

target_compile_definitions(${TARGET} PRIVATE 
    NANO_USE_LOG=${NANO_USE_LOG_VALUE}
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
