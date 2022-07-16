set (CUR_CHAPTER chapter_5_3)

# 二叉树遍历及线索二叉树

set (
    QUESTIONS 
    q3
    q4
    q5
    q6
    q7
    q8
    q9
    q10
    q11
    q12
    q13
    q14
    q15
    q16
    q17
    q18
    q19
    q20
    threaded_tree_test
        
)

foreach (v ${QUESTIONS})
    add_executable (${CUR_CHAPTER}_${v} ${v}.c)
    target_link_libraries (${CUR_CHAPTER}_${v} tree m stack queue)
    
endforeach ()

