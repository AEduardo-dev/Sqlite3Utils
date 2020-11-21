# Installation of all the documentation
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/html DESTINATION ${docs_dest})
install(FILES "refman.pdf" DESTINATION ${docs_dest})
install(FILES  "${CMAKE_SOURCE_DIR}/README.md" DESTINATION ${docs_dest})
