#include <time.h>
#include "convolution_separable.c"



main(int ac, char** av) { 

    clock_t debut, fin ;
    for (int i=1;i<=20;i++){
        double sigma = (double) i;
        debut=clock();
        lissage_separable(ac, av, sigma, 3.5*sigma);
        fin=clock();
        printf("%f %f\n",((double)fin-debut)/CLOCKS_PER_SEC, sigma);
    }


}