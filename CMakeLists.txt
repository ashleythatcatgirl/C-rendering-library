cmake_minimum_required(VERSION 4.4)

project(
	GLib
	VERSION 1.0.0
	LANGUAGES C
)

include(GNUInstallDirs)


find_package(glfw3 3.5 REQUIRED)


add_library(GLib STATIC)

target_sources(GLib
	PRIVATE
		src/glib.c
		include/glad.c

	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
		FILES
			src/glib.h
			include/stb_image.h
			include/cglm/cglm.h
)

target_link_libraries(GLib
	PRIVATE 
		glfw
		m
)

install(
	TARGETS GLib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
	FILES src/glib.h
	DESTINATION include/Glib
)
