set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF) 
 
set(
	prim_matrix_srcs
	fix.cpp
	DMatrix.h
	Matrix_Exception.h
	SMatrix.h
)
add_library(prim_matrix STATIC ${prim_matrix_srcs}) 
set_target_properties(prim_matrix PROPERTIES LINKER_LANGUAGE CXX) 
target_link_libraries(prim_matrix)
 
target_include_directories(prim_matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})