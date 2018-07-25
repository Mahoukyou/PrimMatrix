file(GLOB SRCS *.cpp)

add_executable(sampletest ${SRCS})

target_link_libraries(sampletest
    	prim_matrix
	gtest
	gmock_main
)

install(TARGETS sampletest DESTINATION bin)
add_test(UnitTests sampletest)