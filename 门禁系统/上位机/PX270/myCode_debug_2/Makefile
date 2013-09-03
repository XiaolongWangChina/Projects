CC = arm-linux-gcc
#CC = gcc
test:Serial485.o doParse.o myTty.o
	$(CC) -o test Serial485.o doParse.o myTty.o -lpthread  -DDEBUG
Serial485.o:Serial485.c Serial485.h doParse.h myTty.h
	$(CC) -c Serial485.c -DDEBUG
doParse.o:doParse.c doParse.h userStore.h
	$(CC) -c doParse.c  -DDEBUG
myTty.o:myTty.c myTty.h
	$(CC) -c myTty.c  -DDEBUG
clean:
	rm *.o test
