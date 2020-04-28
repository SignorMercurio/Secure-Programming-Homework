/* g++ main.cpp -g -lgmpxx -lgmp 
 * Author: Mercury
 * Tested under Ubuntu 16.04 & 19.10
*/
#include "keygen.cpp"
#include <iostream>
#include <fstream>
using namespace std;

mpz_class p, q, n, phi, e, d;

unsigned long getSeed(ifstream&);
void paramGen();

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    const char plain[] = "math";
    unsigned a = *((unsigned *)plain); // 0x6874616d
    mpz_class m(a);
    cout << "[*] m: " << m << endl;
    paramGen();

    mpz_t cc, mm, nn;
    mpz_inits(cc, mm, nn, nullptr);
    mpz_set(nn, n.get_mpz_t());

    /* c = m^e mod n */
    mpz_powm_sec(cc, m.get_mpz_t(), e.get_mpz_t(), nn);
    mpz_class c(cc);
    cout << "[+] After RSA encryption, we have c: " << c << endl;

    /* m = c^d mod n */
    mpz_powm_sec(mm, c.get_mpz_t(), d.get_mpz_t(), nn);
    m = mpz_class(mm);
    cout << "[+] After RSA decryption, we have m: " << m << endl;

    return 0;
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

void paramGen()
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