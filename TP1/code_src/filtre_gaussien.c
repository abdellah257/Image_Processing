#include "pgm.h"
#include <math.h>
#include "divers.c"

	/* 
		Exemple de code avec Entrees Sortie et transformations simples d'images
		S'utilise sous la forme  "exemple tangram.pgm res.pgm"
 	*/
unsigned char** lissage (int ac, char **av, double sigma) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nb,nl,nc, oldnl,oldnc;
  unsigned char **im2=NULL,** im1=NULL,** im11=NULL;
  double** im4,** im5, ** im6, ** im7, **im8, **im9,**im10;
  double sigma2 = pow(sigma, 2);
  
  if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);
  /* L'image originale pour le calcul du PSNR */
  im11=lectureimagepgm(av[3],&nl,&nc);
  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
	/* Calcul de son inverse video */
  double**im3=imuchar2double(im1,nl,nc);
  oldnl=nl; oldnc=nc;
	/*  la fft demande des puissances de 2. On padde avec des 0, mais les dimensions nl et nc changent */ 
  im7=padimdforfft(im3,&nl,&nc);
  /*
	On peut aussi directement utiliser 
	im7=padimucforfft(im1,&nl,&nc);
	sans convertir im1 en image de réels
  */ 
	/* Creation des images pour les parties reelles et imagianires des fft  */
  im4=alloue_image_double(nl,nc); im5=alloue_image_double(nl,nc); im6=alloue_image_double(nl,nc);
	/* Clacul de la fft de im7,im4 */
  fft(im7,im4,im5,im6,nl,nc);

  //----------------- Added --------------------
  // On shift les quadrants de la FFT de l'image entrée
  fftshift(im5,im6,im7,im4,nl,nc);

  // Multiplication par Gaussienne : sigma

  for (int i=0; i<nl; i++)
    for (int j=0; j<nc; j++){
      double e1 = ((double)(i - ((double)(nl))/2))/nl;
      double e2 = ((double)(j - ((double)(nc))/2))/nc;
      double filtre = exp(-2*pow(M_PI,2)*sigma2*(pow(e1, 2) + pow(e2, 2)));
      im7[i][j] *= filtre;
      im4[i][j] *= filtre;
    }

  // ReShift le produit des deux FFT
  fftshift(im7,im4,im5,im6,nl,nc);

  //----------------- Added ---------------------

	/* Creation des images pour les parties reelles et imagianires des fft inverses */
  im9=alloue_image_double(nl,nc); im10=alloue_image_double(nl,nc); 
        /* Clacul de la fft inverse de im5,im6 */
  ifft(im5,im6,im9,im10,nl,nc);

  
	/* Conversion en entier8bits de la partie reelle de la fftinverse, 
	   Suppresion des 0 qui ont servi a completer en utilisant la fonction crop
	   Sauvegarde au format pgm de cette image qui doit etre identique a 'linverse video 
	   car on a realise la suite fftinv(fft(image))*/

  im2 = crop(imdouble2uchar(im9,nl,nc),0,0,oldnl,oldnc);
  char name[strlen(av[2])];
  char dest[100] = "./FFT/";
  strcpy(name, av[2]);
  char* str_sigma2[20];
  name[strlen(name)-4] = '\0';
  strcat(dest, name);
  snprintf(str_sigma2, 20,"_%0.1f.pgm", sigma2);
  ecritureimagepgm(strcat(dest,str_sigma2),im2,oldnl,oldnc);
  // Affichage du PSNR entre l'image débruité et l'image théorique
  printf("%f %f\n", psnr(im2, im11, oldnl, oldnc), sigma);

  return im2;
}


// main (int ac, char **av){
//   // for (int i = 1; i<1000; i += 1){
//   //   lissage(ac, av, i);
//   // }
//   lissage(ac, av, 1);
//   lissage(ac, av, 5*5);
//   lissage(ac, av, 10*10);
//   lissage(ac, av, 20*20);

// }