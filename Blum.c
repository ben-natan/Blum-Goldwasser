#include "Blum.h"

int Fermat(const mpz_t n, unsigned int k, gmp_randstate_t state){
  mpz_t a, bound, e, tmp;
  mpz_init(tmp);
  mpz_init(a);
  mpz_init_set(bound, n);
  mpz_sub_ui(bound, bound, 3);
  mpz_init_set(e, n);
  mpz_sub_ui(e, e, 1);

  for( ; k > 0 ; --k){
    mpz_urandomm(a, state, bound);
    mpz_add_ui(a, a, 2); // 1 < a < n-1
    mpz_powm(tmp, a, e, n);
    if(mpz_cmp_ui(tmp, 1)){
      mpz_clear(a); mpz_clear(bound); mpz_clear(e); mpz_clear(tmp);
      return 0;
    }
  }
  mpz_clear(a); mpz_clear(bound); mpz_clear(e); mpz_clear(tmp);
  return 1;
}

void GenPremier(mpz_t res, const mpz_t n, unsigned int k, gmp_randstate_t state){
  mpz_t rest;
  mpz_init(rest);
  while(1){
    mpz_urandomm(res, state, n);
    if (Fermat(res, k, state)) {
      mpz_mod_ui(rest, res, 4);
      if (!mpz_cmp_ui(rest, 3)) {
        mpz_clear(rest);
        return;
      }
    }
  }
}

char BBS_step(const mpz_t n, mpz_t x){
  mpz_powm_ui(x, x, 2, n);

  mpz_t z;
  mpz_init(z);
  mpz_mod_ui(z, x, 2);

  return ((mpz_get_ui(z) == 1) ? (char)0b00000001 : (char)0b00000000);
}

void read_pub(mpz_t pub, char * file_path){
  FILE *fd = fopen(file_path, "r");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  char str[2048];
  fgets(str, 2048, fd);
  mpz_init_set_str(pub, str, 10);
  fclose(fd);
  return;
}

void write_pub(char * file_path, mpz_t pub){
  FILE *fd = fopen(file_path, "w");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  gmp_fprintf(fd, "%Zd", pub);
  fclose(fd);
  return;
}


void read_prvt(mpz_t prvt1, mpz_t prvt2, char * file_path){
  FILE *fd = fopen(file_path, "r");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  char str[1024];
  fgets(str, 1024, fd);
  mpz_init_set_str(prvt1, str, 10);
  fgets(str, 1024, fd);
  mpz_init_set_str(prvt2, str, 10);
  fclose(fd);
  return;
}

void write_prvt(char * file_path, mpz_t prvt1, mpz_t prvt2){
  FILE *fd = fopen(file_path, "w");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  gmp_fprintf(fd, "%Zd\n%Zd", prvt1, prvt2);
  fclose(fd);
  return;
}

unsigned int read_plain(char * file_path, char * plain){
  FILE *fd = fopen(file_path, "r");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  char c; unsigned int i = 0, j;
  while ((c = fgetc(fd)) != EOF){
    for (j = 0 ; j < 8 ; ++j){
      plain[i++] = ( (c & 0b10000000) ? 1 : 0);
      c <<= 1;
    }
  }
  fclose(fd);
  return i;
}

void write_plain(char * file_path, char *plain, unsigned int size){
  FILE *fd = fopen(file_path, "w");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  char c; unsigned int i = 0, j;
  for ( ; i < size ; ){
    c = 0;
    for (j = 0 ; j < 8 ; ++j){
      c <<= 1;
      c ^= ( plain[i++] ? 1 : 0);
    }
    putc(c, fd);
  }
  fclose(fd);
  return;
}

unsigned int read_cipher(char * file_path, char * cipher, mpz_t xt){
  FILE *fd = fopen(file_path, "r");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  char c; unsigned int i = 0;
  while ((c = fgetc(fd)) != '\n'){
    cipher[i++] = ( c=='1' ? 1 : 0);
  }
  char str[1024];
  fgets(str, 1024, fd);
  mpz_init_set_str(xt, str, 10);
  fclose(fd);
  return i;
}

void write_cipher(char * file_path, char *cipher, unsigned int size, mpz_t xt){
  FILE *fd = fopen(file_path, "w");
  if (fd == NULL){
    fprintf(stderr, "Can't open %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  unsigned int i = 0;
  for ( ; i < size ; ++i){
    putc(( cipher[i] ? '1' : '0'), fd);
  }
  fputc('\n', fd);
  gmp_fprintf(fd, "%Zd", xt);
  fclose(fd);
  return;
}
