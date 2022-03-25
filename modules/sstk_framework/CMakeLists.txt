file(GLOB_RECURSE SSTK_FRAMEWORK_SRCS "*.c")
# foreach(v ${SSTK_FRAMEWORK_SRCS})
#     message(${v})
# endforeach()

# 头文件引用示例
include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/core 
    ${CMAKE_CURRENT_SOURCE_DIR}/radar 
    ${CMAKE_CURRENT_SOURCE_DIR}/math 
    ${CMAKE_CURRENT_SOURCE_DIR}/util 
    ${CMAKE_CURRENT_SOURCE_DIR}/external 
    ${CMAKE_CURRENT_SOURCE_DIR}/portable/unix
)

add_library (sstk_framework STATIC ${SSTK_FRAMEWORK_SRCS})
