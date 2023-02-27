#include "algorithm.c"
#include "filtre_median.c"
#include "filtre_recursif.c"
#include "filtre_patch.c"
#include "divers.c"

double return_h(double sigma);
int return_t(double sigma);
int return_r(double sigma);

int main(int ac, char **av)
{
    int nl, nc;
    unsigned char **im1 = NULL;
    unsigned char **im2 = NULL;
    unsigned char **im3 = NULL;
    // double pourcentile = 0.5;
    int taille_block = 5;
    double psnr_value = 0;
    double sigma, h;
    int r, t;

    if (ac < 4)
    {

        printf("Usage : %s image_à_filter image_originale sigma\n", av[0]);
        exit(1);
    }
    /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
    im1 = lectureimagepgm(av[1], &nl, &nc);
    if (im1 == NULL)
    {
        puts("Lecture image impossible");
        exit(1);
    }

    im3 = lectureimagepgm(av[2], &nl, &nc);
    sscanf(av[3],"%lf",&sigma);
    h = return_h(sigma);
    r = return_r(sigma);
    t = return_t(sigma);
    printf("sigma=%f \t h=%f \t patch=%d \t region=%d \n",sigma, h, t, r);
    clock_t debut, fin;

    // Filtre median
    debut = clock();
    im2 = filtre_median(im1, nl, taille_block);
    ecritureimagepgm("./Result/perf_median_reel.pgm", im2, nl, nc);
    fin = clock();
    psnr_value = psnr(im3, im2, nl, nc);
    libere_image(im2);
    printf("Filtre median :: Time = %f, PSNR (Quality) = %f.\n", ((double)fin - debut) / CLOCKS_PER_SEC, psnr_value);

    // Filtre recursif
    debut = clock();
    im2 = filtre_recursif(im1, nl, 100);
    ecritureimagepgm("./Result/perf_recursif_reel.pgm", im2, nl, nc);
    fin = clock();
    psnr_value = psnr(im3, im2, nl, nc);
    libere_image(im2);
    printf("Filtre recursif :: Time = %f, PSNR (Quality) = %f.\n", ((double)fin - debut) / CLOCKS_PER_SEC, psnr_value);

    // Filtre patch
    debut = clock();
    im2 = filtre_patch(im1, nl, t, r, sigma, h); // For images with 15 <= sigma <= 45
    ecritureimagepgm("./Result/perf_patch_reel.pgm", im2, nl, nc);
    fin = clock();
    psnr_value = psnr(im3, im2, nl, nc);
    libere_image(im2);
    printf("Filtre patch :: Time = %f, PSNR (Quality) = %f.\n", ((double)fin - debut) / CLOCKS_PER_SEC, psnr_value);

    libere_image(im1);
    libere_image(im3);
}

double return_h(double sigma){
    if( sigma <= 30){
        return 0.4;
    }else if(sigma <= 75){
        return 0.35;
    }else if( sigma <= 100){
        return 0.3;
    }else{
        return 0;
    }
}

int return_t(double sigma){
    if( sigma <= 15){
        return 1;
    }else if(sigma <= 30){
        return 2;
    }else if( sigma <= 45){
        return 3;
    }else if( sigma <= 75){
        return 4;
    }else{
        return 5;
    }
}

int return_r(double sigma){
    if( sigma <= 30){
        return 10;
    }else if( sigma <= 100){
        return 17;
    }else{
        return 0;
    }
}
