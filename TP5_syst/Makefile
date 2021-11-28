.SUFFIXES:

CC=gcc
CFLAGS=-g 
CFLAGS+= -DDEBUG 

all: test test-format test-rand test-comparaison test-buffer

%: stdes.o %.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	$(RM) test test-format test-rand *.o
	
	
	
