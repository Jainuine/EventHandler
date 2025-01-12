cmake_minimum_required(VERSION 3.10)
project(EventHandler)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

#set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Build) 
set(MANAGER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/EventManager)
set(EXCEPTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Exception)
set(BUILD_DIR Build)

set(SOURCES
        ${MANAGER_DIR}/Manager.cpp
        ${EXCEPTION_DIR}/EventException.cpp
)

set(HEADERS
        ${EXCEPTION_DIR}/EventException.h
        ${MANAGER_DIR}/Manager.h
)

add_library(EventHandler SHARED ${SOURCES})

# Set the include directories for other projects
target_include_directories(EventHandler PUBLIC ${INCLUDE_DIR})