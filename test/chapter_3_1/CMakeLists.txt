set (CUR_CHAPTER chapter_3_1)

set (
    QUESTIONS 
    q4
    q5
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} stack)
    
endforeach ()

