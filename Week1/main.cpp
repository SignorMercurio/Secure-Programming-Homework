/* g++ main.cpp -g -lgmpxx -lgmp 
 * Author: Mercury
 * Tested under Ubuntu 19.04
*/
#include <gmpxx.h>

int judge(mpz_t, mpz_class, mpz_t, mpz_class, mpz_t, unsigned);
int miller_rabin(mpz_class, int);

gmp_randclass rstate(gmp_randinit_default); // Mersene Twister

int main()
{
  int cnt = 24;
  mpz_class n;

  printf("Please input number of times to repeat the test: ");
  scanf("%d", &cnt);
  /*
  printf("Please input the number you want to judge (in base 10):\n");
  mpz_inp_str(n.get_mpz_t(), stdin, 10);
  printf("%sPrime\n", miller_rabin(n, cnt) ? "" : "Not ");
  */
  rstate.seed(time(0));
  do {
    n = rstate.get_z_bits(120);
  } while (miller_rabin(n, cnt) == 0);
  puts("The generated prime number with 120 bits is: (in base 10)");
  mpz_out_str(stdout, 10, n.get_mpz_t());

  return 0;
}

int miller_rabin(mpz_class n, int cnt)
{
  mpz_class n_1 = n-1;
  mpz_class b, r;
  unsigned s;
  int isprime = 1;
  mpz_t k, n_mpz, n_1_mpz;

  mpz_inits(k, n_mpz, n_1_mpz, nullptr);
  mpz_set(n_mpz, n.get_mpz_t());
  mpz_set(n_1_mpz, n_1.get_mpz_t());

  /* Fermat Test to speed up */
  b = 210;
  /* r = b^(n-1) mod n */
  mpz_powm(r.get_mpz_t(), b.get_mpz_t(), n_1_mpz, n_mpz);
  if (r != 1) return 0;

  /* Find s,k such that n-1 = 2^s * k */
  /* Starting from the least significant bit until the first 1 is found */
  s = mpz_scan1(n_1_mpz, 0);
  /* k = (n-1) / 2^s */
  mpz_tdiv_q_2exp(k, n_1_mpz, s);

  for (int i = 0; isprime && i < cnt; ++i) {
    b = rstate.get_z_range(n-3); // [0,n-4]
    b += 2; // [2,n-2]
    isprime = judge(n_mpz, n_1, b.get_mpz_t(), r, k, s);
  }

  return isprime;
}

int judge(mpz_t n, mpz_class n_1, mpz_t b, mpz_class r, mpz_t k, unsigned s)
{
  /* r = b^k mod n */
  mpz_powm(r.get_mpz_t(), b, k, n);
  if (r == 1 || r == n_1) {
    return 1;
  }
  for (unsigned i = 0; i < s-1; ++i) {
    /* r = r^2 mod n */
    mpz_powm_ui(r.get_mpz_t(), r.get_mpz_t(), 2, n);
    if (r == n_1) return 1;
    if (r == 1) return 0;
  }

  return 0;
}