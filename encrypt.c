#include "Blum.h"
#include <stdio.h>

int main(int argc, char *argv[]){

  // Arguments
  if (argc != 3) {
    puts("Préciser le message à chiffrer et le chemin vers la clé publique.\n ex: ./encrypt message.txt key.pub\n");
    return 1;
  }
  char* plain_path = argv[1];
  char* pub_path = argv[2];

  // Lecture fichier message
  char* plain = malloc(1000*sizeof(char));
  int t = read_plain(plain_path, plain);
  plain[t] = '\0';

  // Lecture clé publique 
  mpz_t n;
  mpz_init(n);
  read_pub(n,pub_path);

  // Algorithme de Blum-Goldwasser
  gmp_randstate_t state;
  gmp_randinit_default(state);
  mpz_t x;
  mpz_init(x);
  mpz_urandomm(x, state, n);

  BBS_step(n, x);
  
  char z;
  char c_array[t+1];
  for (int k =0; k < t; k++) {
    z = BBS_step(n,x);
    c_array[k] = z ^ plain[k];
  }
  
  BBS_step(n,x);

  // Write cipher
  write_cipher("encrypted.txt", c_array, t, x);

  printf("\nMessage chiffré dans 'encrypted.txt' !\n");

  mpz_clear(n);
  mpz_clear(x);
  exit(EXIT_SUCCESS);
}
