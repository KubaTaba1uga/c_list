ar_list.o: src/ar_list.c src/ar_list.h
	gcc -Wall -c src/ar_list.c -o ar_list.o

test_ar_list.o: tests/test_ar_list.c 
	gcc -Wall -c tests/test_ar_list.c -o test_ar_list.o 

test: ar_list.o test_ar_list.o
	gcc -Wall -zmuldefs ar_list.o test_ar_list.o -l criterion -o test
