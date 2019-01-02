file(GLOB SRCS *.cpp)

add_executable(static_matrix_test ${SRCS})

target_link_libraries(
	static_matrix_test 
    prim_matrix
	gtest
	gmock_main
)

if(ENABLE_COVERAGE)
	target_link_libraries(static_matrix_test --coverage)
endif()

install(TARGETS static_matrix_test DESTINATION bin)
add_test(UnitTests static_matrix_test)