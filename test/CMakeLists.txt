add_executable(cli_test cli.c)
target_link_libraries(cli_test wolfssl -pthread)

add_executable(sstk_tcp_ssl sstk_tcp_ssl.c)
target_link_libraries(sstk_tcp_ssl sstk_framework sstk_tcp -pthread)

add_executable(sstk_tcp_cli_ssl sstk_tcp_cli_ssl.c)
target_link_libraries(sstk_tcp_cli_ssl sstk_framework sstk_tcp -pthread)

add_subdirectory(./framework)
