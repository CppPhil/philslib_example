﻿# CMakeList.txt : CMake project for philslib_test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (philslib_test "main.cpp" )

# TODO: Add tests and install targets if needed.
target_link_libraries(
	philslib_test
	PRIVATE
	CppPhil::philslib)
