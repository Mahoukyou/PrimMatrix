
#SET(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
#SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
#SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")

file(GLOB SRCS *.cpp)

add_executable(sampletest ${SRCS})

target_link_libraries(
	sampletest
    prim_matrix
	gtest
	gmock_main
)

target_link_libraries(sampletest --coverage)


install(TARGETS sampletest DESTINATION bin)
add_test(UnitTests sampletest)