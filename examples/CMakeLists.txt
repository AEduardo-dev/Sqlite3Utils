# Define where the corresponding include files are located
target_include_directories(sqlite3handler
          INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
          )
target_include_directories(sqlite3query
          INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
          )

# Installation rules
# Libraries
install(TARGETS sqlite3handler sqlite3query DESTINATION lib)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3handler.hpp"
              "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3query.hpp"
              DESTINATION include
              )
