#include "pgm.h"
#include "math.h"

/* 
		Detection des contours avec un operateur de deuxieme ordre.
*/
laplace (int ac, char **av, double sigma2, double Sh, int a) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nb,nl,nc, oldnl,oldnc;
  double n, m, num;
  unsigned char **im2=NULL,** im1=NULL;
  double ** im4=NULL,** im5=NULL, ** im6 = NULL, ** im7 = NULL, ** im8 = NULL;
  
  if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);

  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
	/* Calcul de son inverse video */
  double**im3=imuchar2double(im1,nl,nc);
  
  oldnl=nl; oldnc=nc;
	/*  la fft demande des puissances de 2. On padde avec des 0, mais les dimensions nl et nc changent */ 
  im7=padimdforfft(im3,&nl,&nc);

  /* Matrice des valeurs du filtre depends on "n" and "sigma2" */

  im4 = alloue_image_double(nl,nc);im5 = alloue_image_double(nl,nc);im6 = alloue_image_double(nl,nc);

  fft(im7, im4, im5, im6, nl, nc);

  fftshift(im5, im6, im7, im4, nl, nc);

  for (int x =0; x<nl; x++){
      for (int y =0; y<nc; y++){
            n = ((double)x - (double)nl/2)/nl;  
            m = ((double)y - (double)nc/2)/nc; 
            num = -(M_PI*M_PI*sigma2*(m*m + n*n));
            im7[x][y] *= -(4*num/sigma2)*exp(2*num);
            im4[x][y] *= -(4*num/sigma2)*exp(2*num);
      }
  }

  fftshift(im7, im4, im5, im6, nl, nc);

  ifft(im5, im6, im7, im4, nl, nc);

  im8 = crop_double(im7, 0, 0, oldnl, oldnc);

  /* Extraction de contours */
  if(a == 1){
    for(int x = 0; x<oldnl-1; x++){
        for(int y= 0; y<oldnc-1; y++){
            if((im8[x][y]*im8[x+1][y]<0 || im8[x][y]*im8[x][y+1]<0) && (fabs(im8[x][y]) > Sh)
              ){
                im8[x][y] = 255;
            }else{
                im8[x][y] = 0;
            }
        }
    }
  }

  im2=imdouble2uchar(im8,oldnl,oldnc);

  ecritureimagepgm(av[2],im2,oldnl,oldnc);
}

main (int ac, char **av){
    double sigma2 = 5;   /* ici c'est sigma^2. */
    double Sh = 0.2;     /* le seuil pour enlever le bruit.*/
    int a = 1;           /* a=1 pour detection de contours , 0 pour calcule de laplacien uniquement. */
    laplace(ac, av, sigma2, Sh, a);
}

