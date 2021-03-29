#include "Blum.h"
#include <stdio.h>

int main(int argc, char *argv[]){

  // Clé publique n = pq, Clé privée (p,q)

  // Arguments
  if (argc != 2) {
    puts("Préciser l'entier maximum. ex: ./genkey 74346345\n");
    return 1;
  }
  unsigned long input_max;
  input_max = strtoul(argv[1], NULL, 10);

  mpz_t max;
  mpz_init_set_ui(max, input_max);

  printf("Géneration des clés pour N < %ld..\n", input_max);

  // Initialisations
  gmp_randstate_t state;
  gmp_randinit_default(state);

  mpz_t p, q, pub;
  mpz_init(p);
  mpz_init(q);
  mpz_init(pub);

  // Génération de la clé privée (p,q)
  GenPremier(p, max, 10, state);
  printf(" -> OK\n");
  GenPremier(q, max, 10, state);
  printf(" -> OK\n");

  // Calcul de la clé publique = p*q
  mpz_mul(pub, p, q);

  // Ecritures
  write_pub("key.pub", pub);
  write_prvt("key.prvt", p, q);

  printf("Les clés ont été générées!\n");

  // Clears
  mpz_clear(p);
  mpz_clear(q);
  mpz_clear(pub);
  mpz_clear(max);

  

  exit(EXIT_SUCCESS);
}
