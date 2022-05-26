file(GLOB_RECURSE QUEUE_SRCS "*.c" "*.cc" "*.cpp")

add_library (queue SHARED ${QUEUE_SRCS})