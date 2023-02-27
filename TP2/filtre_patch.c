#include <math.h>
#include "pgm.h"

unsigned char **filtre_patch(unsigned char **im, int taille, int r, int t, double sigma, double h);
calcul_poids(double **im, double **region, int x, int y, int t, int r, int taille, double sigma, double h);
double moyenne_pondere(double **im, double **region, int x, int y, int t, int taille);



unsigned char **filtre_patch(unsigned char **im, int taille, int r, int t, double sigma, double h)
{
    double **filtered = alloue_image_double(taille, taille);
    unsigned char **uchar_filtered = NULL;
    double **im_double = imuchar2double(im, taille, taille);
    // Un patch de taille (2*r + 1)**2
    // Une région de taille (2*t + 1)**2
    double **region = alloue_image_double(2 * t + 1, 2 * t + 1);

    for (int x = 0; x < taille; x++)
    {
        for (int y = 0; y < taille; y++)
        {
            calcul_poids(im_double, region, x, y, t, r, taille, sigma, h);
            filtered[x][y] = moyenne_pondere(im_double, region, x, y, t, taille);
        }
    }

    uchar_filtered = imdouble2uchar(filtered, taille, taille);
    libere_image(filtered);
    libere_image(im_double);
    libere_image(region);

    return uchar_filtered;
}

calcul_poids(double **im, double **region, int x, int y, int t, int r, int taille, double sigma, double hh)
{
    // On va parcourir les pixels de la région
    // Et prendre un patch de chacune de ces pixels
    // Et enfin déterminer le poids de chaque pixel voisin
    double d2_PQ = 0;
    double w_pq = 0;
    double h = hh*sigma;

    for (int X = x - t; X <= x + t; X++)
    {
        for (int Y = y - t; Y <= y + t; Y++)
        {
            d2_PQ = 0;

            for (int i = -r; i <= r; i++)
            {
                for (int j = -r; j <= r; j++)
                {
                    d2_PQ += pow(im[(x + i + taille) % taille][(y + j + taille) % taille] - im[(X + i + taille) % taille][(Y + j + taille) % taille], 2);
                }
            }

            d2_PQ /= pow(2 * r + 1, 2);
            w_pq = exp(-fmax(d2_PQ - 2 * pow(sigma, 2), 0) / pow(h, 2));

            region[X - x + t][Y - y + t] = w_pq;
        }
    }
}

double moyenne_pondere(double **im, double **region, int x, int y, int t, int taille)
{
    double num = 0;
    double denum = 0;
    for (int i = -t; i <= t; i++)
    {
        for (int j = -t; j <= t; j++)
        {
            num += region[i + t][j + t] * im[(i + x + taille) % taille][(j + y + taille) % taille];
            denum += region[i + t][j + t];
        }
    }

    return num / denum;
}