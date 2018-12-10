
#SET(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
#SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
#SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")

file(GLOB SRCS *.cpp)

add_executable(static_matrix_test ${SRCS})

target_link_libraries(
	static_matrix_test 
    	prim_matrix
	gtest
	gmock_main
)

target_link_libraries(static_matrix_test --coverage)


install(TARGETS static_matrix_test DESTINATION bin)
add_test(UnitTests static_matrix_test)