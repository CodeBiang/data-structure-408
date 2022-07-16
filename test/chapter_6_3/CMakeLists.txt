set (CUR_CHAPTER chapter_6_3)

set (
    QUESTIONS 
    test
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} graph)
    
endforeach ()

