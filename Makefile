CC = gcc
CFLAGS =-g  -Wall -Wextra -pedantic -std=gnu99 -I/local/courses/csse2310/include
LDFLAGS = -L/local/courses/csse2310/lib
LIBS = -lcsse2310a1

.DEFAULT_GOAL := uqunscramble


uqunscramble: uqunscramble.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@


uqunscramble.o: uqunscramble.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f uqunscramble *.o
