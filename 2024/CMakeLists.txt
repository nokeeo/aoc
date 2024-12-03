cmake_minimum_required(VERSION 3.0)

project(your-project-name)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE source_files
   ./*.cc
)

file(GLOB_RECURSE header_files
  ./*.h
  ../cpp/*.h
)

include_directories("../cpp/")
add_executable(runner ${source_files} ${header_files})