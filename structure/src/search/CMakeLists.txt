file(GLOB_RECURSE SEARCH_SRCS "*.c" "*.cc" "*.cpp")

add_library (search SHARED ${SEARCH_SRCS})