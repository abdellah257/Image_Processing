CFLAGS=-c -g -std=gnu99
LDFLAGS=-lm

all:essai1 psnr_filtre_gaussien complexite main gradient_discret laplacien

main: main.o fft.o pgm.o
	gcc  -g $^ $(LDFLAGS) -o $@

gradient_discret: gradient_discret.o pgm.o
	gcc  -g $^ $(LDFLAGS) -o $@

laplacien: laplacien.o fft.o pgm.o
	gcc  -g $^ $(LDFLAGS) -o $@

complexite: complexite.o fft.o pgm.o
	gcc  -g $^ $(LDFLAGS) -o $@

psnr_filtre_gaussien: psnr_filtre_gaussien.o pgm.o fft.o
	gcc  -g $^ $(LDFLAGS) -o $@

convolution_separable: convolution_separable.o pgm.o fft.o
	gcc  -g $^ $(LDFLAGS) -o $@

convolution_2D: convolution_2D.o pgm.o
	gcc  -g $^ $(LDFLAGS) -o $@

filtre_gaussien: filtre_gaussien.o pgm.o fft.o
	gcc  $^ $(LDFLAGS) -o $@

essai1: essai1.o pgm.o
	gcc  $^ $(LDFLAGS) -o $@



%.o:%.c
	gcc $(CFLAGS) $<
