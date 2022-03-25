file(GLOB_RECURSE SSTK_TCP_SRCS "*.c")

add_library(sstk_tcp SHARED ${SSTK_TCP_SRCS})

include_directories(
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/core 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/radar 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/math 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/util 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/portable/unix
    ${PROJECT_SOURCE_DIR}/modules/sstk_tcp
)

target_link_libraries(sstk_tcp 
    sstk_framework 
    wolfssl
)
