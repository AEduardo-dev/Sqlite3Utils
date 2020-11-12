#Find google tests
find_package(GTest REQUIRED True)

# If found include it's directories
include_directories(${GTEST_INCLUDE_DIRS})

# Define executable
add_executable(unit_tests "${TESTS_DIR}/tests.cpp")

# Link libraries used in the unit_tests
# Libraries needed for google_test framework (gmock not included)
target_link_libraries(unit_tests PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
# User libraries
target_link_libraries(unit_tests
                      PRIVATE sqlite3
                      PUBLIC query
                      PUBLIC handler
                      )

# unit testing installation
install(TARGETS unit_tests DESTINATION ${tests_dest})
