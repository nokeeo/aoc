cmake_minimum_required(VERSION 3.0)

project(your-project-name)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_EXE_LINKER_FLAGS "-lprofiler")
set(ENV{CPUPROFILE} "/tmp/prof.out /Users/ericlee/Documents/Projects/adventOfCode/2024/runner")

file(GLOB_RECURSE source_files
   ./*.cc
)

file(GLOB_RECURSE header_files
  ./*.h
  ../cpp/*.h
)

include_directories("../cpp/" "/usr/local/Cellar/gperftools/2.16/include/")
link_directories("/usr/local/Cellar/gperftools/2.16/lib/")
# link_libraries(profiler)
add_executable(runner ${source_files} ${header_files})