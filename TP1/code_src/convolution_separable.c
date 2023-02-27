#include "pgm.h"
#include <math.h>
#include "filtre_gaussien.c"

	/* 
		Lissage par convolution spatiale SEPARABLE
 	*/
lissage_separable (int ac, char **av, double sigma, int n) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nl,nc;
  unsigned char **im2=NULL,** im1=NULL,** im11=NULL;
  double ** im4=NULL, ** im6=NULL;
  double sigma2 = pow(sigma, 2);
  
  if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
 
  im1=lectureimagepgm(av[1],&nl,&nc);

    /* Image débruité par FFT */
  im11 = lissage(ac,av,sigma);
  im6 = imuchar2double(im11, nl, nc);

  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
	/* Calcul de son inverse video */
  double**im3=imuchar2double(im1,nl,nc);
  

  // vecteur des valeurs du filtre --- depends on "n" and "sigma2"
  double vec_filtre [n+1];
    for (int j =0; j<=n; j++){
        double num = -(pow(j,2));
        vec_filtre[j] = exp(num/((double)2*sigma2));
    }

  // Convulution G*I
  // S1 = S2 in our case
  double S1 = 0;
  for (int i = 0; i<=n; i++){
      S1 += vec_filtre[i];
  }
  S1 = 2*S1 - 1;

  for (int j =0; j<=n; j++){
        vec_filtre[j] = vec_filtre[j]/S1;
    }

  
  im4 = alloue_image_double(nl,nc);

  double** G1_xy = alloue_image_double(nl, nc);

  for (int x = 0; x<nl; x++){
    for (int y = 0; y<nc; y++){
        double sum = 0;
        for (int i = -n; i<=n; i++){

            sum+= vec_filtre[abs(i)]*im3[x][(y+i+ nc)%nc];

        }

        G1_xy[x][y] = sum;
    }
}

  for (int x = 0; x<nl; x++){
      for (int y = 0; y<nc; y++){
          double R_xy = 0;
          for (int i = -n; i<=n; i++){

            R_xy+= vec_filtre[abs(i)]*G1_xy[(x+i+ nl)%nl][y];
  
          }

          im4[x][y] = R_xy;
      }
  }
  /* Calcul du PSNR entre l'image théorique et celle débruitée */
  // printf("%f -- %f -- %i\n", psnr(im2, im11, nl, nc),sigma2, n);

  /* Calcul de l'ecart quadratique moyen entre l'image produite par FFT et conv.spatiale*/
  //printf("%f %i\n", eqm_double(im4, im6, nl, nc), n);

  im2=imdouble2uchar(im4,nl,nc);
  //----------------- Added ---------------------

  char name[strlen(av[2])];
  char dest[100] = "./SPAT_SEP/";
  strcpy(name, av[2]);
  char* str_sigma2[20];
  name[strlen(name)-4] = '\0';
  strcat(dest, name);
  snprintf(str_sigma2, 20,"_%0.1f_%i.pgm", sigma2,n);

  ecritureimagepgm(strcat(dest,str_sigma2),im2,nl,nc);


}

/*
main (int ac, char **av){

  for (int i = 1; i<=6; i += 1){
    lissage_separable(ac, av, 1, i);
  }

  for (int i = 1; i<=30; i += 1){
    lissage_separable(ac, av, 5, i);
  }

  for (int i = 1; i<=60; i++){
    lissage_separable(ac, av, 10, i);
  }

  for (int i = 61; i<=120; i += 1){
    lissage_separable(ac, av, 20, i);
  }
}
*/