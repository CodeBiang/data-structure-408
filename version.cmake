

# get git hash
macro(get_git_hash _git_hash)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
            OUTPUT_VARIABLE ${_git_hash}
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            .
        )
    endif()
endmacro()

# get git branch
macro(get_git_branch _git_branch)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
            OUTPUT_VARIABLE ${_git_branch}
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            .
        )
    endif()
endmacro()


# 获取编译时间戳
string(TIMESTAMP BUILD_TIME %Y%m%d_%H%M%S)
# 获取git提交信息
set(GIT_HASH "")
get_git_hash(GIT_HASH)
# 转为大写
string(TOUPPER ${GIT_HASH} GIT_HASH)
set(GIT_BRANCH "")
get_git_branch(GIT_BRANCH)



# 版本设置
# Alpha Beta RC Release
set(VERSION_MAJOR       1)
set(VERSION_MINOR       0)
set(VERSION_PATCH       0)
set(VERSION_STAGE       Alpha)
set(PRODUCT_MODEL       TEST)
set(PRODUCT_NAME        TEST)
set(PRODUCT_NUM         01)
set(PRODUCT_CLIENT      NONE)
set(VERSION_GIT         GIT${GIT_HASH})

# 打印版本信息
message("VERSION_MAJOR   ${VERSION_MAJOR}")
message("VERSION_MINOR   ${VERSION_MINOR}")
message("VERSION_PATCH   ${VERSION_PATCH}")
message("VERSION_STAGE   ${VERSION_STAGE}")
message("PRODUCT_MODEL   ${PRODUCT_MODEL}")
message("PRODUCT_NAME    ${PRODUCT_NAME}")
message("PRODUCT_NUM     ${PRODUCT_NUM}")
message("PRODUCT_CLIENT  ${PRODUCT_CLIENT}")
message("VERSION_GIT     ${VERSION_GIT}")
