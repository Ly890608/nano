cmake_minimum_required(VERSION 3.1...3.17)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(TARGET standalone)

project(${TARGET} 
		VERSION 1.0
		LANGUAGES CXX
		DESCRIPTION "Standalone For Desktop" 
)

if(WIN32)
	set(NANO_APP WIN32)
	set(NANO_PLATFORM windows)
else()
	set(NANO_APP)
	set(NANO_PLATFORM unix)
endif()

add_executable(${TARGET} ${NANO_APP})
target_sources(${TARGET} PRIVATE ${NANO_PLATFORM}/main.cpp)
target_link_libraries(${TARGET} PRIVATE nano::nano)