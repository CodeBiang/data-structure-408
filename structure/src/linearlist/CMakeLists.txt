file(GLOB_RECURSE LINEARLIST_SRCS "*.c" "*.cc" "*.cpp")

add_library (linearlist SHARED ${LINEARLIST_SRCS})