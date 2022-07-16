
# cxx

add_compile_options(-Wall -Wcpp -Wno-missing-field-initializers)

add_definitions("-g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)

link_directories(${CMAKE_CURRENT_LIST_DIR}/lib)
