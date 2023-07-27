list.o: src/list.c
	gcc -c src/list.c -o list.o

test_list.o: tests/test_list.c
	gcc -c tests/test_list.c -o test_list.o 

test: list.o test_list.o
	gcc list.o test_list.o -l criterion -o test
