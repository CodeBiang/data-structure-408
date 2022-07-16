add_executable (structure_test test.c)

target_link_libraries (
    structure_test 
    
    linearlist
    stack
    queue
    tree
)

add_subdirectory (./chapter_2_2)
add_subdirectory (./chapter_2_3)

add_subdirectory (./chapter_3_1)
add_subdirectory (./chapter_3_2)
add_subdirectory (./chapter_3_3)

add_subdirectory (./chapter_4_1)

add_subdirectory (./chapter_5_2)
add_subdirectory (./chapter_5_3)

add_subdirectory (./chapter_6_3)