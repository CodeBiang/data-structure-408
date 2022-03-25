set (
    SSTK_FRAMEWORK_EXAMPLES 

    sstk_rbtree
    sstk_heap5
    sstk_framework
    sstk_log
    sstk_queue
    sstk_kdtree
    sstk_thread_pool
)

foreach (exp ${SSTK_FRAMEWORK_EXAMPLES})
    add_executable(example_${exp} example_${exp}.c)
    target_link_libraries(example_${exp} sstk_framework -pthread)
endforeach ()

