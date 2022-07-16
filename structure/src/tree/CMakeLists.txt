file(GLOB_RECURSE TREE_SRCS "*.c" "*.cc" "*.cpp")

add_library(tree SHARED ${TREE_SRCS})

target_link_libraries(tree queue)
