#include "Blum.h"
#include <stdio.h>

int main(int argc, char *argv[]){

  // Arguments
  if (argc != 3) {
    puts("Préciser le message chiffré et le chemin vers la clé privée.\n ex: ./decrypt encrypted.txt key.prvt\n");
    return 1;
  }
  char* cipher_path = argv[1];
  char* prvt_path = argv[2];

  // Lire le fichier chiffré
  mpz_t xt;
  mpz_init(xt);
  char* cipher = malloc(1000*sizeof(char));
  int t = read_cipher(cipher_path, cipher, xt);

  // Lire la clé privée
  mpz_t p, q;
  mpz_init(p);
  mpz_init(q);
  read_prvt(p, q, prvt_path);

  // Algorithme de Blum-Goldwasser
  mpz_t d1, d2;
  mpz_init(d1);
  mpz_init(d2);

  // d1 et d2
  mpz_t d1p;
  mpz_init(d1p);
  mpz_add_ui(d1p, p, 1);
  mpz_cdiv_q_ui(d1p, d1p, 4);
  mpz_t d2q;
  mpz_init(d2q);
  mpz_add_ui(d2q, q, 1);
  mpz_cdiv_q_ui(d2q, d2q, 4);

  mpz_t d1mod;
  mpz_init(d1mod);
  mpz_sub_ui(d1mod, p, 1);
  mpz_t d2mod;
  mpz_init(d2mod);
  mpz_sub_ui(d2mod, q, 1);

  mpz_powm_ui(d1, d1p, t+1, d1mod);
  mpz_powm_ui(d2, d2q, t+1, d2mod);
  
  // u et v 
  mpz_t u;
  mpz_t v;
  mpz_init(u);
  mpz_init(v);
  mpz_powm(u, xt, d1, p);
  mpz_powm(v, xt, d2, q);

  // a et b
  mpz_t a;
  mpz_init(a);
  mpz_invert(a,p,q);

  mpz_t b;
  mpz_init(b);
  mpz_invert(b,q,p);

  // x
  mpz_t x;
  mpz_init(x);

  mpz_mul(a, v, a);
  mpz_mul(a, p, a);
  mpz_mul(b, u, b);
  mpz_mul(b, q, b);

  mpz_t n;
  mpz_init(n);
  mpz_mul(n, p, q);
  mpz_add(x, a, b);
  mpz_mod(x, x, n);

  char z;
  char plain[t];
  for (int k = 0; k < t ; k++) {
    z = BBS_step(n,x);
    plain[k] = z ^ cipher[k];
  }

  // Write plain
  write_plain("decrypted.txt", plain, t);
  printf("\nMessage déchiffré dans 'decrypted.txt' !\n");
  
  // Clears
  mpz_clear(p);
  mpz_clear(q);
  mpz_clear(d1);
  mpz_clear(d2);
  mpz_clear(d1p);
  mpz_clear(d2q);
  mpz_clear(d1mod);
  mpz_clear(d2mod);
  mpz_clear(u);
  mpz_clear(v);
  mpz_clear(a);
  mpz_clear(b);
  mpz_clear(x);
  mpz_clear(n);

  exit(EXIT_SUCCESS);
}
