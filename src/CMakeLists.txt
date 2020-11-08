# Configure a header file for version number
configure_file(DatabaseHandlerConfig.h.in DatabaseHandlerConfig.h)

# add the libraries
add_subdirectory(lib)
