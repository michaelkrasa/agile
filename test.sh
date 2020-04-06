#!/bin/bash

algs=( enum bnb dp greedy )
inputs=( 'easy.20.1.txt' 'easy.200.4.txt' 'hard1.200.11.txt' 'hard1.2000.1.txt' )
declare -A times=(
  ['easy.20.1.txt']=5
  ['easy.200.4.txt']=5
  ['hard1.200.11.txt']=60
  ['hard1.2000.1.txt']=60
)
declare -A solutions=(
  ['easy.20.1.txt enum']=377
  ['easy.20.1.txt bnb']=377
  ['easy.20.1.txt dp']=377
  ['easy.200.4.txt bnb']=4077
  ['easy.200.4.txt dp']=4077
  ['hard1.200.11.txt dp']=126968
)

for FILE in ${inputs[@]}
do
  LIMIT="${times[$FILE]}"
  echo "==========================================="
  echo "Running on $FILE for $LIMIT seconds"
  echo "==========================================="
  echo
  echo "Algorithm |      Optimal Value       | Time Taken  | Result"
  echo "----------------------------------------------------------------"
  for alg in ${algs[@]}
  do
    TIME=$({ time timeout ${LIMIT}s ./$alg $FILE > ${alg}_${FILE}_out ; } 2>&1 | grep real | grep -o '[0-9].*')
    LAST=$(grep -o '\(Current best solution\|value\)=[0-9]*' ${alg}_${FILE}_out | tail -1)
    VALUE=$(echo $LAST | sed -n -e 's/.*=\([0-9]\+\).*/\1/p')
    CORRECT="?"
    if [ -z $VALUE ]
    then
      VALUE="-"
      CORRECT="-"
    fi
    if [[ "$LAST" == *Current* ]]
    then
      VALUE="$VALUE (after ${LIMIT}s)"
    fi
    key="$FILE $alg"
    if [ ${solutions[$key]+abc} ]
    then
      ANSWER=${solutions[$key]}
      if [ $ANSWER == $VALUE ]
      then
        CORRECT="Yes"
      else
        CORRECT="No"
      fi
    fi
    printf -v alg %-10.10s $alg
    printf -v VALUE %-25.25s "$VALUE"
    echo "$alg| $VALUE| ${TIME}    | $CORRECT"
  done
  echo
  echo
done
