CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -ffast-math
INCLUDES = -I SDL3/include
LIBS = -L SDL3/lib -lSDL3 -lm

SRCS = $(wildcard src/*.c)

demo: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o demo $(LIBS)
	cp SDL3/bin/SDL3.dll .

clean:
	rm -f demo SDL3.dll