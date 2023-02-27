#include "algorithm.c"
#include "filtre_median.c"
#include "filtre_recursif.c"
#include "filtre_patch.c"

int main(int ac, char **av)
{
    int nl, nc;
    unsigned char **im1 = NULL;
    unsigned char **im2 = NULL;
    double pourcentile = 0.005;
    int taille_block = 10;

    if (ac < 2)
    {

        printf("Usage : %s image_entree image_sortie \n", av[0]);
        exit(1);
    }
    /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
    im1 = lectureimagepgm(av[1], &nl, &nc);
    if (im1 == NULL)
    {
        puts("Lecture image impossible");
        exit(1);
    }

    printf("L'ecart-type du pourcentile %.1f est : %f", pourcentile * 100, 1.13 * estimation_bruit(im1, taille_block, pourcentile, nl, nc));
    // im2 = filtre_median(im1, nl, taille_block);
    // im2 = filtre_recursif(im1, nl, 40);
    // im2 = filtre_patch(im1, nl, 2, 10); // For images with sigma <= 45

    // ecritureimagepgm(av[2], im2, nl, nc);

    libere_image(im1);
    // libere_image(im2);
}