set (CUR_CHAPTER chapter_5_2)

set (
    QUESTIONS 
    q5
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} m)
    
endforeach ()

