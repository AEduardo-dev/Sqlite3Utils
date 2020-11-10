# Add the sources of libraries in this directory
add_library(handler OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3handler.cpp")
add_library(query OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3query.cpp")
