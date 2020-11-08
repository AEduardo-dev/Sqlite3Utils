cmake_minimum_required(VERSION 3.15)

# Set the project name
project(DatabaseHandler VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
# add_subdirectory(examples)
# add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(include)

# Configure a header file for version number
configure_file(DatabaseHandlerConfig.h.in DatabaseHandlerConfig.h)

# Check for required sqlite3 library
find_library(sqlite3 REQUIRED True)

# Link sqlite3handler with it's libraries dependencies
target_link_libraries(sqlite3handler PRIVATE sqlite3)
target_link_libraries(sqlite3handler PRIVATE sqlite3query)

# Add the executable
add_executable(DatabaseHandler "${PROJECT_SOURCE_DIR}/src/minimalHandler.cpp")

# Link libraries
target_link_libraries(DatabaseHandler
                      PUBLIC sqlite3handler
                      PUBLIC sqlite3query
                      )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(DatabaseHandler PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

# Installation rules
# Executable
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

# Define a function to simplify adding tests in the future
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${DatabaseHandler_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${DatabaseHandler_VERSION_MINOR}")
include(CPack)
