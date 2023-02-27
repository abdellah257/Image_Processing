#include "pgm.h"
#include <math.h>
#include "divers.c"

	/* 
		Lissage par convolution spatiale NON SEPARABLE
 	*/
lissage_2D (int ac, char **av, double sigma2, int n) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nl,nc;
  unsigned char **im2=NULL,** im1=NULL,** im11=NULL;
  double ** im4=NULL;
  
  if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);


  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
	/* Calcul de son inverse video */
  double**im3=imuchar2double(im1,nl,nc);
  

  /* Matrice des valeurs du filtre depends on "n" and "sigma2" */
  double mat_filtre [n+1][n+1];
  for (int i =0; i<=n; i++){
      for (int j =0; j<=n; j++){
          double num = -(pow(i,2)+pow(j,2));
          mat_filtre[i][j] = exp(num/(2*sigma2));
      }
  }   
  


  // Convulution G*I
  // S1 = S2 in our case
  double S1 = 0;
  for (int i = 0; i<=n; i++){
      S1 += mat_filtre[i][0];
  }
  S1 = 2*S1 - 1;



  im4 = alloue_image_double(nl,nc);

  for (int x = 0; x<nl; x++){
      for (int y = 0; y<nc; y++){
          double R_xy = 0;
          for (int i = -n; i<=n; i++){
              for (int j=-n; j<=n; j++){
                  R_xy+= mat_filtre[abs(i)][abs(j)]*im3[(x+i + nl)%nl][(y+j +nc)%nc];
              }
          }

          im4[x][y] = (1.0/pow(S1,2))*R_xy;
      }
  }

  im2=imdouble2uchar(im4,nl,nc);

  //----------------- Added ---------------------

  
	/* Conversion en entier8bits de la partie reelle de la fftinverse, 
	   Suppresion des 0 qui ont servi a completer en utilisant la fonction crop
	   Sauvegarde au format pgm de cette image qui doit etre identique a 'linverse video 
	   car on a realise la suite fftinv(fft(image))*/

  char name[strlen(av[2])];
  char dest[100] = "./SPAT_2D/";
  strcpy(name, av[2]);
  char* str_sigma2[20];
  name[strlen(name)-4] = '\0';
  strcat(dest, name);
  snprintf(str_sigma2, 20,"_%0.1f_%i.pgm", sigma2,n);

}


// main (int ac, char **av){
    
//   // for (int i = 1; i<=6; i += 1){
//   //   lissage_2D(ac, av, 1, i);
//   // }

//   // for (int i = 1; i<=30; i += 1){
//   //   lissage_2D(ac, av, 5*5, i);
//   // }

//   for (int i = 1; i<=60; i += 1){
//     lissage_2D(ac, av, 10*10, i);
//   }

//   // for (int i = 1; i<=120; i += 1){
//   //   lissage_2D(ac, av, 20*20, i);
//   // }

// }