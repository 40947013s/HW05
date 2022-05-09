all: hw0501.o hw0502.o hw0503.o 
	gcc hw0501.o -o hw0501
	gcc hw0503.o -o hw0503 -lm	

hw0501: hw0501.c
	gcc -c hw0501.c -o hw0501.o
	gcc -shared hw0501.o -o libhw0501.so

hw0502: hw0502.c
	gcc -c hw0502.c -o hw0502.o
	gcc -shared hw0502.o -o libhw0502.so

hw0503: hw0503.c
	gcc -c hw0503.c -o hw0503.o -lm
	gcc -shared hw0503.o -o libhw0503.so
