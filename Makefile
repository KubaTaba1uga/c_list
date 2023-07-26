dl_list.o: src/dl_list.c src/dl_list.h src/dl_body.h
	gcc -c src/dl_list.c -o dl_list.o

dl_body.o: src/dl_body.c src/dl_body.h
	gcc -c src/dl_body.c -o dl_body.o

test_dl_list.o: tests/test_dl_list.c src/dl_list.h
	gcc -c tests/test_dl_list.c -o test_dl_list.o 

test_dl_body.o: tests/test_dl_body.c src/dl_body.h
	gcc -c tests/test_dl_body.c -o test_dl_body.o 


test: dl_list.o test_dl_list.o dl_body.o test_dl_body.o
	gcc dl_list.o test_dl_list.o dl_body.o test_dl_body.o -l criterion -o test
