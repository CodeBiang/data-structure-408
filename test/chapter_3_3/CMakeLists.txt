set (CUR_CHAPTER chapter_3_3)

set (
    QUESTIONS 
    q1
    q2
    q3
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} stack)
    
endforeach ()

