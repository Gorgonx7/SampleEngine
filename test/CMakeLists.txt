add_subdirectory(Catch2)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_files/shader)
configure_file(core/shader/test_files/shader.vert test_files/shader/shader.vert COPYONLY)