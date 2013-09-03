CC = gcc
CFLAGS = -g -I.
LFLAGS =

NN: backpropagation.o data.o NN.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

NN.o: NN.c
	$(CC) $(CFLAGS) -c $<

data.o: data.c data.h
	$(CC) $(CFLAGS) -c $<

backpropagation.o: backpropagation.c backpropagation.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o NN
