#include "keygen.cpp"
#include <iostream>
#include <fstream>
using namespace std;

/*
int main()
{
    //const char plain[] = "math";
    //unsigned a = *((unsigned *)plain); // 0x6874616d

    mpz_class m(a);
    cout << "[*] m: " << m << endl;
    paramGen();

    mpz_t cc, mm, nn;
    mpz_inits(cc, mm, nn, nullptr);
    mpz_set(nn, n.get_mpz_t());

    mpz_powm_sec(cc, m.get_mpz_t(), e.get_mpz_t(), nn);
    mpz_class c(cc);
    cout << "[+] After RSA encryption, we have c: " << c << endl;

    mpz_powm_sec(mm, c.get_mpz_t(), d.get_mpz_t(), nn);
    m = mpz_class(mm);
    cout << "[+] After RSA decryption, we have m: " << m << endl;

    return 0;
}*/

mpz_class encrypt(mpz_t m, mpz_t e, mpz_t n)
{
    mpz_t cc;
    mpz_init(cc);
    mpz_powm_sec(cc, m, e, n);
    return mpz_class(cc);
}

mpz_class decrypt(mpz_t c, mpz_t d, mpz_t n)
{
    mpz_t mm;
    mpz_init(mm);
    mpz_powm_sec(mm, c, d, n);
    return mpz_class(mm);
}

unsigned long getSeed(ifstream& urandom)
{
    unsigned long seed = 0;
    size_t size = sizeof(seed);
    urandom.read(reinterpret_cast<char*>(&seed), size);
    if (!urandom) {
        cerr << "Error reading from /dev/urandom." << endl;
        exit(1);
    }

    return seed;
}

void paramGen(mpz_class &p, mpz_class &q, mpz_class &n, mpz_class &phi, mpz_class &e, mpz_class &d)
{
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if (!urandom) {
        cerr << "Error opening /dev/urandom." << endl;
        exit(1);
    }
    
    p = keygen(getSeed(urandom));
    cout << "[*] p: " << p << endl;
    q = keygen(getSeed(urandom));
    cout << "[*] q: " << q << endl;

    n = p*q;
    cout << "[+] n: " << n << endl;
    phi = (p-1) * (q-1);
    cout << "[*] phi(n): " << phi << endl;

    gmp_randclass rstate(gmp_randinit_default);
    rstate.seed(getSeed(urandom));
    do {
        e = rstate.get_z_range(phi); // [0, phi-1]
    } while (gcd(e, phi) != 1);
    cout << "[+] e: " << e << endl;

    mpz_t dd;
    mpz_init(dd);
    /* d = e^(-1) mod phi(n) */
    if (!mpz_invert(dd, e.get_mpz_t(), phi.get_mpz_t())) {
        cerr << "Modular Inverse does not exist." << endl;
        exit(1);
    }
    d = mpz_class(dd);
    cout << "[*] d: " << d << endl;
}