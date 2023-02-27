#include <math.h>
#include <stdbool.h>
#include "pgm.h"

unsigned char **filtre_recursif(unsigned char **im, int taille, double k);
double next_signal(double **im, int x, int y, double k, int taille);
double w(double **im, int i, int j, double k, int taille);
copy_image(double **im1, double **im2, int taille);
bool check_image(double **im1, double **im2, int taille);

unsigned char **filtre_recursif(unsigned char **im, int taille, double k)
{

    // Deux images qui sert à génerer à chaque itération
    // l'image suivante à partir de l'image courante

    double **current_im = imuchar2double(im, taille, taille);
    double **next_im = alloue_image_double(taille, taille);
    unsigned char **filtered;

    for (int l = 0; l < 1000; l++)
    {
        // On a pas considéré les points dont i/j = 0/taille-1 ou 2/taille-2

        for (int i = 0; i < taille; i++)
        {
            for (int j = 0; j < taille; j++)
            {

                next_im[i][j] = next_signal(current_im, i, j, k, taille);
            }
        }
        if(check_image(current_im, next_im, taille)){
            printf("recusive k = %i \n", l);
            filtered = imdouble2uchar(current_im, taille, taille);
            libere_image(current_im);
            libere_image(next_im);
            return filtered;
        }else{
            copy_image(current_im, next_im, taille);
        }
    }

    filtered = imdouble2uchar(current_im, taille, taille);
    libere_image(current_im);
    libere_image(next_im);
    return filtered;
}

double next_signal(double **im, int x, int y, double k, int taille)
{
    double num = 0;
    double denom = 0;
    double w_value;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            w_value = w(im, x + i, y + j, k, taille);
            num += w_value * im[(x + i + taille) % taille][(y + j + taille) % taille];
            denom += w_value;
        }
    }

    return num / denom;
}

double w(double **im, int i, int j, double k, int taille)
{
    double expo = pow((im[(i + 1 + taille) % taille][(j + taille) % taille] - im[(i - 1 + taille) % taille][(j + taille) % taille]), 2) 
                + pow((im[(i + taille) % taille][(j + 1 + taille) % taille] - im[(i + taille) % taille][(j - 1 + taille) % taille]), 2);
    expo = exp(-expo / (2 * pow(k, 2)));
    return expo;
}

copy_image(double **im1, double **im2, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        for (int j = 0; j < taille; j++)
        {
            im1[i][j] = im2[i][j];
        }
    }
}

bool check_image(double **im1, double **im2, int taille){
    bool t = TRUE;
    for (int i = 0; i < taille; i++)
    {
        for (int j = 0; j < taille; j++)
        {
            if( abs(im1[i][j] -im2[i][j])> 0.0000001){
                t = FALSE;
                return t;
            }
        }
    }
    return t;
}