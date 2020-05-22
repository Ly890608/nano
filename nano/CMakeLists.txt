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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/Core/Application.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/Core/BaseDefines.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nano/nano.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Application.cpp
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
