cmake_minimum_required (VERSION 3.2)

project (primmatrix)

option(ENABLE_TESTS "Enable tests" TRUE)

#Coverage
if(CMAKE_COMPILER_IS_GNUCC)
	option(ENABLE_COVERAGE "Enable coverage" FALSE)

	if(ENABLE_COVERAGE)
		add_compile_options(--coverage -O0)
	endif()
endif()

include_directories(".")
add_subdirectory(src)

if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()