file(GLOB_RECURSE GRAGH_SRCS "*.c" "*.cc" "*.cpp")

add_library (graph SHARED ${GRAGH_SRCS})

target_link_libraries(graph queue)