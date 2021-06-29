cmake_minimum_required(VERSION 3.10)

project(argparse)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

elseif(UNIX)

  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")

endif()

find_package(Threads REQUIRED)

include(gtest.cmake)

include_directories("${PROJECT_SOURCE_DIR}/include/")

# tests
add_subdirectory(tests)
