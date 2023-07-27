list.o: src/list.c src/list.h
	gcc -c src/list.c -o list.o

test_list.o: tests/test_list.c src/list.h
	gcc -c tests/test_list.c -o test_list.o 

test: list.o test_list.o
	gcc list.o test_list.o -l criterion -D UNITTESTS -o test
