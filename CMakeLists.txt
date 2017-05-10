cmake_minimum_required(VERSION 3.5)
project(1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        1.cpp server.cpp server.h network.cpp network.h)

add_executable(1 ${SOURCE_FILES})

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

