cmake_minimum_required(VERSION 3.16)

# set the project name
project(DatabaseHandler VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#configure a header file for version number
configure_file(DatabaseHandlerConfig.h.in DatabaseHandlerConfig.h)

# add the libraries
add_subdirectory(lib)

# Check for required sqlite3 library
find_library(sqlite3 REQUIRED True)

# Link sqlite3handler with it's libraries dependencies
target_link_libraries(sqlite3handler PRIVATE sqlite3)
target_link_libraries(sqlite3handler PRIVATE sqlite3query)

# add the executable
add_executable(DatabaseHandler "${PROJECT_SOURCE_DIR}/src/minimalHandler.cpp")

# Link libraries
target_link_libraries(DatabaseHandler
                      PUBLIC sqlite3handler
                      PUBLIC sqlite3query)




# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(DatabaseHandler PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

# Installation rules
install(TARGETS DatabaseHandler DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/DatabaseHandlerConfig.h"
        DESTINATION include
        )

enable_testing()

# Does the application run
add_test(NAME Runs COMMAND DatabaseHandler mydb.db)

# Does the usage message work?
add_test(NAME Usage COMMAND DatabaseHandler)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*<database_name || database_path>"
  )
