#!/bin/bash

mkdir -p outputs

make
valgrind --leak-check=full --error-exitcode=1 ./tema2
valgrind_error=$?

echo ""

score=0

begin_test='outputs/output_'
begin_result='reference/result_'
termination='.out'
padding="......................................"


tests=( "init" "search" "minmax" "succ_pred" "rotations" "insert" "delete" "list_insert" "list_delete")
scores=( 5 5 5 5 5 10 10 10 5 )

for i in ${!tests[@]}
do
    element=${tests[$i]}
    testFileName=$begin_test$element$termination
    resultFileName=$begin_result$element$termination
    result=$(diff $testFileName $resultFileName | wc -l)

    if [ -f $testFileName ] && [ -f $resultFileName ] && [ "$result" -eq "0" ]; then
        printf "%s %s %s\n" "Test ${tests[$i]}" "${padding:${#tests[$i]}}" "passed" 
        score=$((score+${scores[$i]}))
    else
        printf "%s %s %s\n" "Test ${tests[$i]}" "${padding:${#tests[$i]}}" "failed" 
    fi
done


echo ""

# Test Build Tree
result_build_tree_simple=$(diff reference/simple_key_tree.dot outputs/simple_key_tree.dot | wc -l)
result_build_tree=$(diff reference/key_tree.dot outputs/key_tree.dot | wc -l)
result=$(($result_build_tree_simple + $result_build_tree))

if [ "$result" -eq "0" ]; then
    printf "Test build_tree %s %s\n" "${padding:10}" "passed" 
    score=$((score+10))
else
    printf "Test build_tree %s %s\n" "${padding:10}" "failed" 
fi


tests=( "inorder_key" "level_key" "range_key" )
scores=( 5 10 5 )

for i in ${!tests[@]}
do
    element=${tests[$i]}
    testFileName=$begin_test$element$termination
    resultFileName=$begin_result$element$termination
    result=$(diff $testFileName $resultFileName | wc -l)

    if [ -f $testFileName ] && [ -f $resultFileName ] && [ "$result" -eq "0" ]; then
        printf "%s %s %s\n" "Test ${tests[$i]}" "${padding:${#tests[$i]}}" "passed" 
        score=$((score+${scores[$i]}))
    else
        printf "%s %s %s\n" "Test ${tests[$i]}" "${padding:${#tests[$i]}}" "failed" 
    fi
done


echo ""

if [[ ${valgrind_error} -eq 0 ]]; then
    echo "Valgrind errors ............................ passed"
    score=$((score+10))
else
    echo "Valgrind errors ............................ failed"
fi

echo ""
echo "                                          Total: $score"
echo ""

make clean
