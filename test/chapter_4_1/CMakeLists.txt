set (CUR_CHAPTER chapter_4_1)

set (
    QUESTIONS 
    kmp
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} stack)
    
endforeach ()

