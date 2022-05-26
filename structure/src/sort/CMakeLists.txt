file(GLOB_RECURSE SORT_SRCS "*.c" "*.cc" "*.cpp")

add_library (sort SHARED ${SORT_SRCS})