
add_library(stb_image STATIC)
target_sources(stb_image
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
		FILES
			stb_image.h
)

add_library(cglm STATIC)
target_sources(cglm
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			cglm/
		FILES
			cglm/cglm.h
)


