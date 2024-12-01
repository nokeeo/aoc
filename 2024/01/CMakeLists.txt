cmake_minimum_required(VERSION 3.0)

project(your-project-name)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the variable source_files to the list of names of your C++ source code
# Note the lack of commas or other deliminators
set(source_files
   main.cc
)

include_directories("../../cpp/")
# Build a program called 'program' from the source files we specified above
add_executable(program ${source_files})