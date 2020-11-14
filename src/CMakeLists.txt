# Add the sources of libraries in this directory
add_library(handler SHARED "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3handler.cpp")
add_library(query SHARED "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3query.cpp")
