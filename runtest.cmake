execute_process(COMMAND ./unit_tests
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
  TIMEOUT 1000 # it should be less than in add_test
  RESULT_VARIABLE status
)

file(REMOVE ${CMAKE_BINARY_DIR}/tests/MyDB.db)
file(REMOVE ${CMAKE_BINARY_DIR}/tests/CreatedDB.db)
file(REMOVE ${CMAKE_BINARY_DIR}/tests/NoExtensionDB)

if(status)
  MESSAGE(STATUS "${CMAKE_BINARY_DIR}")
  MESSAGE(FATAL_ERROR "'Unit Test executing status: ${status}")
endif()
