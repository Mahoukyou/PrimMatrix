cmake_minimum_required (VERSION 3.2)

project (primmatrix)

#Coverage
if(CMAKE_COMPILER_IS_GNUCC)
	option(ENABLE_COVERAGE "Enable coverage" FALSE)

	if(ENABLE_COVERAGE)
		add_compile_options(--coverage -O0)
	endif()
endif()


enable_testing()

include_directories(".")

add_subdirectory(src)
add_subdirectory(test)