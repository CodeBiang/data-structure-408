set (CUR_CHAPTER chapter_2_2)

set (
    QUESTIONS 
    q1
    q2
    q3
    q4_and_5
    q6
    q7
    q8
    q9
    q10
    q11
    q12
    q13
    q14
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} linearlist)
    
endforeach ()

