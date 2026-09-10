
add_executable(main)
target_sources(main
	PRIVATE
		main.c
	
	PUBLIC
		FILE_SET HEADERS
		FILES
			main.h
)

target_link_libraries(main
	PRIVATE
		stb_image
)

target_link_libraries(main
	PRIVATE
		cglm
)
