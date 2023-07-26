double_linked_list.o: src/double_linked_list.c src/double_linked_list.h
	gcc -c src/double_linked_list.c -o double_linked_list.o

test_init_dl_list.o: tests/test_init_dl_list.c src/double_linked_list.c src/double_linked_list.h
	gcc -c tests/test_init_dl_list.c -o test_init_dl_list.o 

test: double_linked_list.o test_init_dl_list.o
	gcc double_linked_list.o test_init_dl_list.o -l criterion -o test
