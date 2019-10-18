CC=gcc
CFLAGS=-g -std=c99 -Wall
LDFLAGS=-lm

test2a:
	./part2a
	./part2a "only one parameter"
	./part2a "biggest parameter" "at" "start"
	./part2a "biggest" "parameter" "at" "end" "very very very big parameter"
	./part2a "answer" "somewhere" "in" "the" " middle"
	./part2a "two" "strings" "the" "same" "length" "ha ha!"

test2b:
	./part2b -f 70
	./part2b -c 26.8
	./part2b
	./part2b 80
	./part2b -x
	./part2b -f 60.ABC
	./part2b -x 80
	./part2b -c -280

