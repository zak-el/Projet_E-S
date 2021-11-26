.SUFFIXES:
.PRECIOS: %.o

CC=gcc
CFLAGS=-Wall -g 
CFLAGS+= -DDEBUG

all: test test-format test-rand

%: stdes.o %.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	$(RM) *.o
	
	
	
