cmake_minimum_required (VERSION 3.10)

# 交叉编译配置
# include (cross.cmake)

# 版本配置
include (version.cmake)
# 编译即路径配置
include (config.cmake)

project (woflssl_test VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# 配置
configure_file (${PROJECT_SOURCE_DIR}/inc/version.h.in ${PROJECT_SOURCE_DIR}/inc/version.h)

# 单元测试, 交叉编译不可用
include(CTest)
enable_testing()

# 头文件路径
include_directories (./inc ./src)
include_directories(
    ${PROJECT_SOURCE_DIR}/meta/generated
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/core 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/external 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/radar 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/math 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/util 
    ${PROJECT_SOURCE_DIR}/modules/sstk_framework/portable/unix
    ${PROJECT_SOURCE_DIR}/modules/sstk_tcp
)

# 动态库
link_directories (./lib)

# 模块
add_subdirectory (modules/sstk_framework)
add_subdirectory (modules/sstk_tcp)


add_subdirectory (test)
add_definitions("-g")

# 业务源码
file (GLOB_RECURSE ALL_SRCS "./src/*.cc" "./src/*.cpp" "./src/*.c")
add_executable (${PROJECT_NAME} ${ALL_SRCS})

target_link_libraries(${PROJECT_NAME}
    -pthread
    wolfssl
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
