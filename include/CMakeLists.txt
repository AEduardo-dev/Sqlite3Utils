# Define where the corresponding include files are located
# target_include_directories(sqlite3handler
#           INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
#           )
# target_include_directories(sqlite3query
#           INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
#           )

target_include_directories(handler INTERFACE
                       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # for headers when building
											 $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for config_impl.hpp when building
                       $<INSTALL_INTERFACE:${include_dest}> # for client in install mode
                       $<INSTALL_INTERFACE:${lib_dest}> # for config_impl.hpp in install mode
                       )

target_include_directories(query INTERFACE
                      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # for headers when building
											 $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for config_impl.hpp when building
                      $<INSTALL_INTERFACE:${include_dest}> # for client in install mode
                      $<INSTALL_INTERFACE:${lib_dest}> # for config_impl.hpp in install mode
                      )
