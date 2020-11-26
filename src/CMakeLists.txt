# Add the sources of libraries in this directory
add_library(handler SHARED "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3handler.cpp")
add_library(query SHARED "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3query.cpp")

# Link sqlite3handler with it's dependencies
IF(UNIX)
  target_link_libraries(handler PRIVATE sqlite3)

ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES Windows OR ${CMAKE_SYSTEM_NAME} MATCHES MSYS)
  target_include_directories(handler PRIVATE ${SQLite3_INCLUDE_DIRS})
  target_link_libraries(handler PRIVATE ${SQLite3_LIBRARIES})

ENDIF(UNIX)
target_link_libraries(handler PRIVATE query)

# Installation rules
# Libraries
install(TARGETS handler EXPORT handler DESTINATION ${lib_dest})
install(TARGETS query EXPORT query DESTINATION ${lib_dest})
