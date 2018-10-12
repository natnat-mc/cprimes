NAME = primes

CC = gcc
LD = gcc

CCFLAGS = -O2 -Wall -Wextra
LDLIBS = -lpthread -lc

.PHONY: all run clean mrproper rebuild

all: $(NAME)

$(NAME): main.o common.o multithread.o monothread.o
	$(LD) -o $@ $^ $(LDLIBS)

main.o: main.c multithread.h monothread.h common.h
	$(CC) $(CCFLAGS) -o $@ -c $<

multithread.o: multithread.c multithread.h common.h
	$(CC) $(CCFLAGS) -o $@ -c $<

monothread.o: monothread.c monothread.h common.h
	$(CC) $(CCFLAGS) -o $@ -c $<

common.o: common.c common.h
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f *.o

mrproper: clean
	rm -f $(NAME)

rebuild: mrproper all

run: $(NAME)
	./$(NAME)
