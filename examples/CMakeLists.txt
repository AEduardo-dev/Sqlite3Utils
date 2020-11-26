# Installation of all the examples

install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${examples_dest}
FILES_MATCHING PATTERN "*.cpp")
