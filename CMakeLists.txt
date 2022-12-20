
cmake_minimum_required(VERSION 2.8)
project(Bilaschenko)

# Enable C++ 11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include header directories
include_directories("include")
include_directories("tests")

# Add source files
file(GLOB_RECURSE SRC "src/*.cc")
file(GLOB_RECURSE SRC_TEST "tests/*.cc")

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Main executable
add_executable(gomoku ${SRC})

# Profiling executable
if (ENABLE_PROFILING)
    set(CMAKE_BUILD_TYPE Debug)
    add_executable(gomoku_prof ${SRC})
    set_target_properties(gomoku_prof PROPERTIES COMPILE_FLAGS "-pg")
    set_target_properties(gomoku_prof PROPERTIES LINK_FLAGS "-pg")
endif()

# Test executable
if (ENABLE_TESTING)
    add_executable(gomoku_test ${SRC} ${SRC_TEST})
    set_target_properties(gomoku_test PROPERTIES COMPILE_FLAGS "-D BLUPIG_TEST")
    find_package(Threads)
    target_link_libraries(gomoku_test ${CMAKE_THREAD_LIBS_INIT})
endif()

# Allow installing using 'make install'
install(TARGETS gomoku DESTINATION bin)
