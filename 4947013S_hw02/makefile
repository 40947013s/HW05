CFLAGS = -O3 -Wall
INCLUDE = -I curl-7.63.0/include
LIB = -L curl-7.63.0/lib/.libs/	

all: hw0501.o hw0502.o hw0503.o hw0504.o 
	gcc hw0501.o -o hw0501
	gcc hw0503.o -o hw0503 -lm	
    gcc ${CFLAGS} ${INCLUDE} ${LIB} hw0504.c -o hw0504 -lcurl -lm

hw0501: hw0501.c
	gcc -c hw0501.c -o hw0501.o
	gcc -shared hw0501.o -o libhw0501.so

hw0502: hw0502.c
	gcc -c hw0502.c -o hw0502.o
	gcc -shared hw0502.o -o libhw0502.so

hw0503: hw0503.c
	gcc -c hw0503.c -o hw0503.o -lm
	gcc -shared hw0503.o -o libhw0503.so

hw0504: hw0504.c
	gcc -g -fPIC -c hw0504.c
	gcc -shared hw0504.o -o libfoo.so
