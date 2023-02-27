#include <math.h>
#include "pgm.h"

double **convolution_par_masque(double **im, double **masque, int n, int nl, int nc)
{
    // n  : la taille du masque étant 2n+1

    double **im_res = alloue_image_double(nl, nc);

    // Convolution avec le filtre (masque)
    for (int i = 0; i < nl; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            double sum_ij = 0;
            for (int x = -n; x <= n; x++)
            {
                for (int y = -n; y <= n; y++)
                {
                    double test = masque[x + n][y + n];
                    sum_ij += im[(i + x + nl) % nl][(j + y + nc) % nc] * masque[x + n][y + n];
                }
            }
            im_res[i][j] = sum_ij;
        }
    }

    return im_res;
}

double *histogramme(double **image, int t, int nl, int nc)
{
    // Initialisation d'histogramme
    double *hist = malloc(nl * nc * sizeof(double));

    // Parcours de tous les pixels de l'image
    for (int i = 0; i < nl; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            // Calcul de la variance du bloc centrée au pixel (i, j)

            // Calcul de l'esperance
            double mean = 0;
            for (int x = -t; x <= t; x++)
            {
                for (int y = -t; y <= t; y++)
                {
                    // printf("Les elements i et j sont : %f %f", (i + x + nl) % nl, (j + y + nc) % nc);
                    // fflush(stdout);
                    mean += image[(i + x + nl) % nl][(j + y + nc) % nc];
                }
            }
            mean /= (float)pow(2 * t + 1, 2);

            // Calcul de l'ecart-type
            double deviation;
            double sumsqr = 0;
            for (int x = -t; x <= t; x++)
            {
                for (int y = -t; y <= t; y++)
                {
                    deviation = image[(i + x + nl) % nl][(j + y + nc) % nc] - mean;
                    sumsqr += pow(deviation, 2);
                }
            }

            sumsqr /= (float)pow(2 * t + 1, 2);
            sumsqr = sqrt(sumsqr);

            // Remplir l'histogramme
            hist[nl * i + j] = sumsqr;
        }
    }

    return hist;
}

double percentile(double *arr, int n, double pourcentile)
{
    int i, j;
    double tmp;

    // ordonner (bubble sort) par ordre croissant
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }

    // trouver l'ecart type qui dépasse le pourcentile
    double p = 0;
    int index = -1;
    while (p * 100 < pourcentile)
    {
        index++;
        p = (double)(index + 1) / n;
    }

    return arr[index];
}

double estimation_bruit(unsigned char **im, int t, double pourcentile, int nl, int nc)
{
    double **im_double = imuchar2double(im, nl, nc);
    double **im_filtree;
    double *im_hist; // Histrogramme à remplir
    double ecart_type;

    // Filtre passe-haut
    // double masque[3][3] = {{0, 1. / 5., 0},
    //                        {-1. / 5., 1., -1. / 5.},
    //                        {0, 1. / 5., 0}};
    double **masque = alloue_image_double(3, 3);
    masque[0][0] = 0;
    masque[0][1] = 1. / 5.;
    masque[0][2] = 0;
    masque[1][0] = -1. / 5.;
    masque[1][1] = 1.;
    masque[1][2] = -1. / 5.;
    masque[2][0] = 0;
    masque[2][1] = 1. / 5.;
    masque[2][2] = 0;

    im_filtree = convolution_par_masque(im_double, masque, 1, nl, nc);
    im_hist = histogramme(im_filtree, t, nl, nc);
    ecart_type = percentile(im_hist, nl * nc, pourcentile);

    // Free tables
    libere_image(im_filtree);
    libere_image(im_double);
    libere_image(masque);
    free(im_hist);

    return ecart_type;
}

// int main(int ac, char **av)
// {
//     int nl, nc;
//     unsigned char **im1 = NULL;
//     double pourcentile = 0.5;
//     int taille_block = 5;

//     if (ac < 2)
//     {
//         printf("Usage : %s entree \n", av[0]);
//         exit(1);
//     }
//     /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
//     im1 = lectureimagepgm(av[1], &nl, &nc);
//     if (im1 == NULL)
//     {
//         puts("Lecture image impossible");
//         exit(1);
//     }

//     printf("L'ecart-type du pourcentile %.1f est : %f", pourcentile * 100, 1.13 * estimation_bruit(im1, taille_block, pourcentile, nl, nc));
//     libere_image(im1);
// }