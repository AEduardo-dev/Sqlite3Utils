cmake_minimum_required(VERSION 3.15)

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


# Add subdirectories
# add_subdirectory(examples)
# add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(include)


# Configure a header file for version number
configure_file(Sqlite3UtilsConfig.h.in Sqlite3UtilsConfig.h)

message(CHECK_START "-> Finding sqlite3...")
# Check for required sqlite3 library
find_library(FOUND_SQLITE3 sqlite4 REQUIRED True)

if(NOT FOUND_SQLITE3)

  message(CHECK_FAIL "  Sqlite3 not found")
  file(DOWNLOAD "https://www.sqlite.org/2020/sqlite-autoconf-3330000.tar.gz" "./sqlite3_install"
       TIMEOUT 60
       TLS_VERIFY ON
       #TODO: CHECKSUM does not match, check will algo it uses
     EXPECTED_HASH SHA3_512=7ebb78ae95450b7466f570c3339103ac6ca9690efc991248652339b3ef5460be)

else()
  message(CHECK_PASS "  Sqlite3 found")
endif(NOT FOUND_SQLITE3)

# Link sqlite3handler with it's libraries dependencies
target_link_libraries(handler PRIVATE sqlite3)
target_link_libraries(handler PRIVATE query)

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
