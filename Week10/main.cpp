#include <cstdio>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

#define A 4
#define B 20
#define P 29
typedef long long ll;
typedef pair<ll,ll> Point;

ll egcd(ll,ll,ll&,ll&);
ll modinv(ll,ll);
inline ll E(ll);
inline ll dE(ll,ll);
Point add(Point,Point);
Point mul(Point,ll);
int menu();
void encrypt(Point);
void decrypt(Point);

int main()
{
    ll choice;

    setvbuf(stdout, NULL, _IONBF, 0);

    Point p(13, 23);
    while ((choice = menu()) != 3) {
        switch (choice) {
            case 1: encrypt(p); break;
            case 2: decrypt(p); break;
        }
    }

    return 0;
}

ll egcd(ll a, ll b, ll &x, ll &y)
{
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    ll d = egcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

ll modinv(ll a, ll m)
{
    ll x, y;
    ll g = egcd(a, m, x, y);
    if (g != 1) {
        puts("Modular inverse does not exist.");
        exit(1);
    }

    return (x % m + m) % m;
}

inline ll E(ll x)
{
    return (x*x*x + A*x + B) % P;
}

/* dy/dx */
inline ll dE(Point p)
{
    ll x = p.first, y = p.second;
    return (3*x*x + A) * modinv(2*y, P);
}

Point add(Point p, Point q)
{
    ll x1 = p.first, y1 = p.second;
    ll x2 = q.first, y2 = q.second;
    ll k;

    if (x1 == x2 && y1 == y2) {
        k = dE(p);
    }
    else {
        k = (y2-y1) * modinv(x2-x1+P, P);
    }
    ll x0 = (k*k - x1 - x2) % P;
    ll y0 = (k * (x1-x0) - y1) % P;
    if (y0 < 0) y0 += P;

    return make_pair(x0, y0);
}

Point mul(Point p, int x)
{
    Point q = p;
    for (int i = 0; i < x-1; ++i) {
        q = add(q, p);
    }

    return q;
}

int menu()
{
    int choice;
    puts("===== Elgamal-ECC Cryptosystem =====");
    puts("1. Encrypt");
    puts("2. Decrypt");
    puts("3. Exit");
    puts("Your choice:");
    scanf("%d", &choice);

    return choice;
}

void encrypt(Point p)
{
    int s;
    printf("[+] Please input secret key (between 1-%d): ", P);
    scanf("%d", &s); getchar();
    Point q = mul(p, s);

    printf("[+] Q(%lld,%lld)\n", q.first, q.second);

    srand(time(0));
    int k = 2 + rand() % ((P-1)-2+1);
    printf("[*] k = %d\n", k);
    Point c1 = mul(p, k);

    ll x,y;
    puts("[+] Please input plaintext (e.g. (x0,y0) ): ");
    scanf("(%lld,%lld)", &x, &y); getchar();
    Point c2 = add(make_pair(x,y), mul(q, k));

    printf("[+] Ciphertext: (c1, c2) = ((%lld,%lld), (%lld,%lld))\n",c1.first,c1.second,c2.first,c2.second);
    
}

void decrypt(Point p)
{
    int s;
    printf("[+] Please input secret key (between 1-%d): ", P);
    scanf("%d", &s); getchar();
    Point q = mul(p, s);

    printf("[+] Q(%lld,%lld)\n", q.first, q.second);

    puts("[+] Please input ciphertext (e.g. ((c1_x,c1_y), (c2_x,c2_y)) ): ");
    ll c1_x, c1_y, c2_x, c2_y;
    scanf("((%lld,%lld), (%lld,%lld))", &c1_x, &c1_y, &c2_x, &c2_y); getchar();
    
    Point sub_s_c1 = mul(make_pair(c1_x, c1_y), s);
    sub_s_c1.second *= -1;
    Point m = add(make_pair(c2_x, c2_y), sub_s_c1);
    printf("[+] Plaintext: P_m(%lld,%lld)\n", m.first, m.second);
}