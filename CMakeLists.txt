cmake_minimum_required(VERSION 2.8.4)

project(pathgen)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.57.0 COMPONENTS program_options REQUIRED)

# probably a terrible way of doing this, but oh well.
add_definitions( -DBOOST_ALL_DYN_LINK )
# I've been told that this slows code down, but I don't think it's really a concern for this application.
# (also my tests show that the program isn't noticeably faster anyway)
add_definitions( -Os )

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(pathgen main.cpp)

if(Boost_FOUND)

    target_link_libraries(pathgen ${Boost_LIBRARIES})

endif()