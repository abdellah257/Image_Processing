#include "pgm.h"
#include "math.h"


/* 
		Detection des contours avec un operateur de premier ordre.
*/
gradient (int ac, char **av, double sigma2, double Sh, double Sb, int a) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nb,nl,nc, oldnl,oldnc;
  unsigned char **im2=NULL,** im1=NULL;
  double ** im4=NULL,** Gx=NULL, ** Gy=NULL;
  
  if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);

  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
	/* Calcul de son inverse video */
  double**im3=imuchar2double(im1,nl,nc);
  

  im4 = alloue_image_double(nl,nc);Gx = alloue_image_double(nl,nc);Gy = alloue_image_double(nl,nc);

    // Matrice des valeurs du filtre Prewitt verticale
  double M1 [3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};

    //G_x
  for (int i =0; i<nl; i++){
      for (int j =0; j<nc; j++){
            double Rij = 0;  
            for (int x =-1; x<=1; x++){
                for (int y =-1; y<=1; y++){
                    Rij += im3[((i+x)%nl+nl)%nl][((j+y)%nc+nc)%nc]*M1[x+1][y+1];
                }
            }
            Gx[i][j] = Rij;
        }
  }

    // Matrice des valeurs du filtre Prewitt horizentale
  double M2 [3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};

    //G_y
  for (int i =0; i<nl; i++){
      for (int j =0; j<nc; j++){
            double Rij = 0;  
            for (int x =-1; x<=1; x++){
                for (int y =-1; y<=1; y++){
                    Rij += im3[((i+x)%nl+nl)%nl][((j+y)%nc+nc)%nc]*M2[x+1][y+1];
                }
            }
            Gy[i][j] = Rij;
        }
  }
// Remplisage de im4 (l'image finale)
    if (a == 1){

        for (int x = 0; x<nl; x++){
            for (int y = 0; y<nc; y++){
                double N = sqrt(pow(Gx[x][y], 2) + pow(Gy[x][y], 2));
                double N1 = sqrt(pow(Gx[x][(y-1+nc)%nc], 2) + pow(Gy[x][(y-1+nc)%nc], 2));
                double N2 = sqrt(pow(Gx[(x+1)%nl][y], 2) + pow(Gy[(x+1)%nl][y], 2));
                double N3 = sqrt(pow(Gx[x][(y+1)%nc], 2) + pow(Gy[x][(y+1)%nc], 2));
                double N4 = sqrt(pow(Gx[(x-1+nl)%nl][y], 2) + pow(Gy[(x-1+nl)%nl][y], 2));
                double coeff = (Gy[x][y]/Gx[x][y]); //?(G_y[x][y]/G_x[x][y]):-(G_y[x][y]/G_x[x][y]);
                //coeff = (coeff <= 1)? coeff:1.0/coeff;
                double N_P1 = (coeff)*N1 + (1.0-coeff)*N2;
                double N_P2 = (coeff)*N3 + (1.0-coeff)*N4;
                if ((N>N_P1) && (N>N_P2)){
                im4[x][y] =  N;
                } else {
                im4[x][y] = 0;
                }
            }
        }
        for (int i =0; i<nl; i++){
            for (int j =0; j<nc; j++){
                if( im4[i][j] > Sh){
                    im4[i][j] = 255;
                }
                else if( im4[i][j] < Sh && im4[i][j] > Sb){
                    if(im4[(i+1)%nl][j] == 255 || im4[i][(j+1)%nc] == 255
                        || im4[(i+1)%nl][(j+1)%nc] == 255 || im4[i][((j-1)%nc+nc)%nc]==255
                        || im4[(i+1)%nl][((j-1)%nc+nc)%nc]==255 || im4[((i-1)%nl+nl)%nl][(j+1)%nc]==255
                        || im4[((i-1)%nl+nl)%nl][j]==255 || im4[((i-1)%nl+nl)%nl][((j-1)%nc+nc)%nc]==255
                        ){
                        im4[i][j] = 255;
                    }else{
                        im4[i][j] = 0;
                    }
                }else{
                    im4[i][j] = 0;
                }
            }
        }
    }else{
        for (int i =0; i<nl; i++){
            for (int j =0; j<nc; j++){
                    im4[i][j] = sqrt(pow(Gx[i][j], 2)+pow(Gy[i][j], 2));
            }
        }
    }


  im2=imdouble2uchar(im4,nl,nc);

  ecritureimagepgm(av[2],im2,nl,nc);
}

main (int ac, char **av){
    double sigma2 = 10;         /* on utilise ici sigma^2 */
    double Sh = 0.25*255;       /* le Seuil haut */
    double Sb = 0.01*255;       /* le Seuil bas  */
    int a = 1;                  /* a = 1 pour extraction de contours, a = 0 pour le calcule de gradient uniquement.*/
    gradient(ac, av, sigma2, Sh, Sb, a);
}