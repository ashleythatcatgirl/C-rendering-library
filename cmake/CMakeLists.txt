cmake_minimum_required(VERSION 4.4)

project(GLib)

add_subdirectory(../src/)

add_subdirectory(../include/)

include(GNUInstallDirs)

install(
	DIRECTORY ../src/
	DESTINATION include/Graphics_library
)

install(
	DIRECTORY ../include/
	DESTINATION include/Graphics_library
)
