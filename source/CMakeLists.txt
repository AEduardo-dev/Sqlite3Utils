cmake_minimum_required(VERSION 3.10)

# set project name
project(Database VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(include)
add_subdirectory(lib)

# Add the necessary libraries .cpp files
add_library(include sqlite3handler.cxx)
add_library(include sqlite3query.cxx)

# add executable
add_executable(Database main.cxx)

target_link_libraries(Database PUBLIC include)

configure_file(DatabaseConfig.h.in DatabaseConfig.h)

# add the binary tree to the search path for include files
# so that we will find DatabaseConfig.h
target_include_directories(Database PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           "${PROJECT_SOURCE_DIR}/lib"
                           )
