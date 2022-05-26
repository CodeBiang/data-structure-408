file(GLOB_RECURSE STACK_SRCS "*.c" "*.cc" "*.cpp")

add_library (stack SHARED ${STACK_SRCS})