CFLAGS=-c -g -std=gnu99
LDFLAGS=-lm

perf: performance.o pgm.o 
	gcc $^ $(LDFLAGS)  -o $@

main: main.o pgm.o
	gcc $^ $(LDFLAGS)  -o $@
	
algo: algorithm.o pgm.o
	gcc $^ $(LDFLAGS)  -o $@

essai1: essai1.o pgm.o
	gcc $^ $(LDFLAGS)  -o $@

essai2: essai2.o pgm.o fft.o
	gcc $^ $(LDFLAGS)  -o $@

%.o:%.c
	gcc $(CFLAGS) $<
