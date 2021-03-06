# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

IF(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
ENDIF(result)

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

IF(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
ENDIF(result)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Add the executable
add_executable(unit_tests "${TESTS_DIR}/tests.cpp")

# Link libraries
target_link_libraries(unit_tests PUBLIC handler query)
target_link_libraries(unit_tests PRIVATE gtest_main)


# Installation rules
# Tests source file
install(FILES "${TESTS_DIR}/tests.cpp" DESTINATION ${tests_dest})
