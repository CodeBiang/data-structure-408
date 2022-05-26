add_executable (structure_test test.c)

target_link_libraries (
    structure_test 
    
    linearlist
)

add_subdirectory (./chapter_2_2)
add_subdirectory (./chapter_2_3)
