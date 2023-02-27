
#include "filtre_gaussien.c"

main (int ac, char **av){
  for (int i = 1; i<100; i += 1){
    lissage(ac, av, ((double) i)/2);
  }

//   lissage(ac, av, 5*5);
//   lissage(ac, av, 10*10);
//   lissage(ac, av, 20*20);

}