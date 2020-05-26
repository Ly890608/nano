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
    set(NANO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Noop/GraphicsCore.cpp)
    set(NANO_GRAPHICS_TYPE ${NANO_GRAPHICS_NOOP})
elseif(WIN32)
    message(STATUS "Nano Graphcis Type is D3D12.")
    set(NANO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/D3D12/GraphicsCore.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/D3D12/GraphicsCore.h)
    set(NANO_GRAPHICS_TYPE ${NANO_GRAPHICS_D3D12})
else()
    message(STATUS "Nano Graphcis Type is Unknown, Set to Noop.")
    set(NANO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Noop/GraphicsCore.cpp)
    set(NANO_GRAPHICS_TYPE ${NANO_GRAPHICS_NOOP})
endif()

if(WIN32)
set(NANO_PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows/MinWindows.h)
else()
set(NANO_PLATFORM_SRC)
endif()

target_sources(${TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/Core/Application.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/Core/BaseDefines.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/nano.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Application.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/GraphicsCore.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nano.cpp
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

# ==================================================================================================
# Compiler flags
# ==================================================================================================
target_compile_features(${TARGET} PRIVATE cxx_std_17)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
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
