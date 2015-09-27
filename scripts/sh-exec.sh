#! /bin/bash

# if [ -n $2 ]
# then
#   alg='$2'
# else
#   alg='1'
# if

if [ "$1" == "-v" ] 
then
  valgrind --leak-check=yes --track-origins=yes ./exec/ep1 1 input/input.txt output/output.txt -d
elif [ "$1" == "-d" ]
then
  ./exec/ep1 4 input/input-14.txt output/output.txt -d
else
  ./exec/ep1 1 input/input.txt output/output.txt
fi
