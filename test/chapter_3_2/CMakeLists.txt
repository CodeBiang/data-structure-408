set (CUR_CHAPTER chapter_3_2)

set (
    QUESTIONS 
    q1
    q3
    q4
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} queue stack)
    
endforeach ()

