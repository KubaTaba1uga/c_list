utils.o:  src/utils/memory.c src/utils/memory.h
	gcc -Wall -c src/utils/memory.c -o utils.o

ar_list.o: src/ar_list.c src/ar_list.h
	gcc -Wall -c src/ar_list.c -o ar_list.o

test_ar_list.o: tests/test_ar_list.c src/ar_list.c src/ar_list.h
	gcc -Wall -c tests/test_ar_list.c -o test_ar_list.o 

test: ar_list.o test_ar_list.o utils.o
	gcc -Wall -zmuldefs ar_list.o test_ar_list.o utils.o -l criterion -o test
