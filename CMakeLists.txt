cmake_minimum_required(VERSION 3.5)
project(1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        1.cpp
        server.cpp server.h
        network.cpp network.h
        OSRM/Route_calculation.cpp OSRM/Route_calculation.h
        db_class.cpp db_class.h
        db_conn_class.cpp db_conn_class.h
        db_conn_properties.cpp db_conn_properties.h
        db_request.cpp db_request.h
        db_table_class.cpp db_table_class.h)

add_executable(1 ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pqlib")

set(BOOST_COMPONENTS system thread filesystem date_time)
# Late 2017 TODO: remove the following checks and always use std::regex
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        set(BOOST_COMPONENTS ${BOOST_COMPONENTS} regex)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST_REGEX")
    endif()
endif()
find_package(Boost 1.58.0 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

target_link_libraries(1 ${Boost_LIBRARIES})

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR AND NOT MSVC_IDE)
    message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'. Please delete them.")
endif()

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_BUILD_TYPE Release)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/OSRM/cmake")

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(bitness 64)
    message(STATUS "Building on a 64 bit system")
else()
    message(STATUS "Building on a 32 bit system")
endif()

if(WIN32 AND MSVC_VERSION LESS 1900)
    message(FATAL_ERROR "Building with Microsoft compiler needs Latest Visual Studio 2015 (Community or better)")
endif()

link_directories(${LibOSRM_LIBRARY_DIRS})

find_package(LibOSRM REQUIRED)

target_link_libraries(1 ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES})
include_directories(SYSTEM ${LibOSRM_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")

include_directories(${YOUR_DIRECTORY}/db)