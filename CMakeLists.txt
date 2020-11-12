cmake_minimum_required(VERSION 3.18)

# Set the project name
project(Sqlite3Utils VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set some variables for convenience
set(EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")
set(INCLUDES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(main_lib_dest "lib/sqlite3_utils-${Sqlite3Utils_VERSION}")
set(lib_dest "${main_lib_dest}/${CMAKE_BUILD_TYPE}")
set(include_dest "include/sqlite3_utils-${Sqlite3Utils_VERSION}")
set(include_dest "tests")
set(SQLITE3_INSTALL_FILE "sqlite-autoconf-3330000.tar.gz")
set(SQLITE3_INSTALL_DIRNAME "${Sqlite3Utils_BINARY_DIR}/sqlite-autoconf-3330000")
# set(GTEST_ROOT "/usr/src/googletest/googletest/include" CACHE PATH "/usr/src/googletest")

# Define some options for processing

option(INSTALL_DEPENDENCIES "Install dependencies not found during configuration" OFF)
option(BUILD_TESTS "Build the google test framework program" OFF)
option(BUILD_EXAMPLES "Build the example programs" OFF)
option(GENERATE_DOCS "Generate doxygen documentation (needs doxygen installation)" OFF)

# Configure a header file for version number
configure_file(Sqlite3UtilsConfig.h.in Sqlite3UtilsConfig.h)

message(CHECK_START " FINDING --> sqlite3")

# Check for dependency sqlite3 library
find_library(FOUND_SQLITE3 sqlite3)

IF(NOT FOUND_SQLITE3)
  message(CHECK_FAIL " sqlite3 NOT FOUND")

  # Ask if the user wants the Installation to be done for sqlite3
  IF(INSTALL_DEPENDENCIES)
    file(DOWNLOAD "https://www.sqlite.org/2020/${SQLITE3_INSTALL_FILE}" "${Sqlite3Utils_BINARY_DIR}/sqlite-autoconf"
         TIMEOUT 60
         TLS_VERIFY ON
         EXPECTED_HASH SHA3_256=6e94e9453cedf8f2023e3923f856741d1e28a2271e9f93d24d95fa48870edaad
         )
    file(ARCHIVE_EXTRACT INPUT sqlite-autoconf)

    execute_process(COMMAND echo "Installation starting...")

    IF(UNIX)
      execute_process(COMMAND ./configure
                      WORKING_DIRECTORY ${SQLITE3_INSTALL_DIRNAME})
      execute_process(COMMAND make WORKING_DIRECTORY ${SQLITE3_INSTALL_DIRNAME})

    ELSEIF(WIN32)
      execute_process(COMMAND nmake /f Makefile.msc
                      WORKING_DIRECTORY ${SQLITE3_INSTALL_DIRNAME})

    ENDIF()

  ELSE()
    # If installation is not selected, let the user know they can re-run the cmake
    message(NOTICE "\nRequired dependency sqlite3 missing.\n")
    message(NOTICE "Please re-run this cmake like the following->\"cmake -DINSTALL_DEPENDENCIES=ON ..\", or install it manually\n")
    # Exit the configuration with an error
    message(FATAL_ERROR "Sqlite3 is not installed. Configuration stopped.")
  ENDIF()


  # Check if installation was done. If not, exit with error message
  message(CHECK_START " VERIFYING INSTALLATION sqlite3 ->")

  find_library(FOUND_SQLITE3 sqlite3 REQUIRED True)

  IF(FOUND_SQLITE3)
    message(CHECK_PASS "Sqlite3 installed succesfully")
  ENDIF()

ELSE()
  message(CHECK_PASS "Sqlite3 FOUND")
ENDIF(NOT FOUND_SQLITE3)

# Add subdirectories four source and include files
add_subdirectory(src)
add_subdirectory(include)

# Link sqlite3handler with it's dependencies
target_link_libraries(handler PRIVATE sqlite3)
target_link_libraries(handler PRIVATE query)

# If selected, add the subdirectories for examples, docs or tests
add_subdirectory(tests)
# add_subdirectory(examples)
# add_subdirectory(docs)

# Add the executable
add_executable(DatabaseHandler "${SOURCES_DIR}/minimalHandler.cpp")

# Link libraries
target_link_libraries(DatabaseHandler
                      PUBLIC handler
                      PUBLIC query
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
        DESTINATION ${include_dest}
        )

# Installation rules
# Libraries
install(TARGETS handler EXPORT handler DESTINATION ${lib_dest})
install(TARGETS query EXPORT query DESTINATION ${lib_dest})

# Version Control
configure_file(sqlite3_utils-config-version.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/sqlite3_utils-config-version.cmake @ONLY)

# Package support
install(FILES sqlite3_utils-config.cmake DESTINATION ${main_lib_dest})

# Export
install(EXPORT handler DESTINATION "${lib_dest}")
install(EXPORT query DESTINATION "${lib_dest}")

# Include files
install(FILES "${INCLUDES_DIR}/handler.hpp"
              "${INCLUDES_DIR}/query.hpp"
              DESTINATION ${include_dest}
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
