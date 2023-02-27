#include <math.h>
#include "pgm.h"

unsigned char **filtre_median(unsigned char **im, int taille, int n);
hist_init(int *hist, unsigned char **im, int n, int a, int b);
int hist_median(int *hist, int n);
update_hist(int *hist, unsigned char **im, int a, int b, int n);

unsigned char **filtre_median(unsigned char **im, int taille, int n)
{
    int *hist = calloc(256, sizeof(int));
    unsigned char **filtered = alloue_image(taille, taille);

    for (int i = n; i < taille - n; i++)
    {
        // Premier histogramme d'une ligne (à mettre à jour pour chaque colonne)
        hist_init(hist, im, n, i, n);

        // Remplacer im(i, n) par la médiane
        filtered[i][n] = hist_median(hist, n);
        // printf("The new value for (%d, %d) is filled\n", i, n);

        // Passer par les autres colonnes
        for (int j = n + 1; j < taille - n; j++)
        {
            update_hist(hist, im, i, j, n);

            // Remplacer im(i, j) par la médiane
            filtered[i][j] = hist_median(hist, n);

            // printf("The new value for (%d, %d) is filled\n", i, j);
        }
    }

    free(hist);
    return filtered;
}

hist_init(int *hist, unsigned char **im, int n, int a, int b)
{
    // Clean histogramme
    for (int k = 0; k <= 255; k++)
    {
        hist[k] = 0;
    }

    for (int i = a - n; i <= a + n; i++)
    {
        for (int j = b - n; j <= b + n; j++)
        {
            hist[im[i][j]] += 1;
        }
    }

    // // Debug
    // int sum = 0;
    // for (int i = 0; i <= 255; i++)
    // {
    //     sum += hist[i];
    // }

    // printf("Initial histogramme sum of elements is: %d ------------------\n", sum);
    // // Debug //
}

int hist_median(int *hist, int n)
{
    double sum = 0;
    int new_value = hist[0];

    for (int i = 0; i <= 255; i++)
    {
        sum += hist[i];
        if (sum >= (double)(pow((2 * n + 1), 2)) / 2)
        {
            new_value = i;
            break;
        }
    }

    return new_value;
}

// Mettre à jour l'histogramme efficacement
update_hist(int *hist, unsigned char **im, int a, int b, int n)
{
    // Supprimer la colonne j-n-1 de l'histogramme
    for (int i = a - n; i <= a + n; i++)
    {
        hist[im[i][b - n - 1]]--; // --
    }

    // Ajouter la colonne j+n à l'histogramme
    for (int i = a - n; i <= a + n; i++)
    {
        hist[im[i][b + n]]++; // ++
    }
}