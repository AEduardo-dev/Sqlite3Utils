# Add the sources of libraries in this directory
add_library(handler SHARED "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3handler.cpp")
add_library(query SHARED "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3query.cpp")

# Link sqlite3handler with it's dependencies
target_link_libraries(handler PUBLIC sqlite3)
target_link_libraries(handler PUBLIC query)

# Installation rules
# Libraries
install(TARGETS handler EXPORT handler DESTINATION ${lib_dest})
install(TARGETS query EXPORT query DESTINATION ${lib_dest})
