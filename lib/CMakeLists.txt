# Add the sources of libraries in this directory
add_library(sqlite3handler OBJECT sqlite3handler.cpp)
add_library(sqlite3query OBJECT sqlite3query.cpp)

# Define where the corresponding include files are located
target_include_directories(sqlite3handler
          INTERFACE "${PROJECT_SOURCE_DIR}/include"
          )
target_include_directories(sqlite3query
          INTERFACE "${PROJECT_SOURCE_DIR}/include"
          )

# Installation rules
install(TARGETS sqlite3handler sqlite3query DESTINATION lib)
install(FILES "${PROJECT_SOURCE_DIR}/include/sqlite3handler.hpp"
              "${PROJECT_SOURCE_DIR}/include/sqlite3query.hpp"
              DESTINATION include
              )
