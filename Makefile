list.o: src/list.c src/list.h
	gcc -Wall -c src/list.c -o list.o

test_list.o: tests/test_list.c 
	gcc -Wall -c tests/test_list.c -o test_list.o 

test: list.o test_list.o
	gcc -Wall -zmuldefs list.o test_list.o -l criterion -o test
