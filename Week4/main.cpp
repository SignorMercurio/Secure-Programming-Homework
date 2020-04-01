#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

typedef unsigned long ul;
typedef unsigned long long ull;

const ul IP[64] = {
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17, 9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};
const ul IP_1[64] = {
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41, 9,49,17,57,25
};

const ul E[48] = {
    32, 1, 2, 3, 4, 5,
     4, 5, 6, 7, 8, 9,
     8, 9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32, 1
};

const ul S[8][4][16] = {
    {
        {14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
        { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
        { 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
        {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}
    },
    {
        {15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
        { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
        { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
        {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}
    },
    {
        {10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
        {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
        { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}
    },
    {
        { 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
        {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
        {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
        { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}
    },
    {
        { 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9}, // ?
        {14,11, 2,12, 4, 7,13, 1, 5, 0,15,13, 3, 9, 8, 6},
        { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
        {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}
    },
    {
        {12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
        {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8},
        { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
        { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}
    },
    {
        { 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
        {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6},
        { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
        { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}
    },
    {
        {13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
        { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
        { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
        { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}
    }
};

const ul P[32] = {
    16, 7,20,21,
    29,12,28,17,
     1,15,23,26,
     5,18,31,10,
     2, 8,24,14,
    32,27, 3, 9,
    19,13,30, 6,
    22,11, 4,25
};

const ul PC1[56] = {
    57,49,41,33,25,17, 9,
     1,58,50,42,34,26,18,
    10, 2,59,51,43,35,27,
    19,11, 3,60,52,44,36,
    63,55,47,39,31,23,15,
     7,62,54,46,38,30,22,
    14, 6,61,53,45,37,29,
    21,13, 5,28,20,12, 4
};
const ul PC2[48] = {
    14,17,11,24, 1, 5,
     3,28,15, 6,21,10,
    23,19,12, 4,26, 8,
    16, 7,27,20,13, 2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};
const ul rotate_cnt[16] = {
    1,1,2,2,
    2,2,2,2,
    1,2,2,2,
    2,2,2,1
};

vector<bitset<48> > roundKeys(16);

#define PERM(len, input, permutation) \
    bitset<len> res; \
    size_t sz = input.size(); \
    for (int i = 0; i < len; ++i) res[len-1-i] = input[sz-permutation[i]];

int menu();
inline ul getRow(const bitset<6>&);
inline ul getCol(const bitset<6>&);
inline bitset<28> lrot(const bitset<28>&,ul);
void generateRoundKeys(const bitset<56>&);
bitset<32> f(const bitset<32>&, const bitset<48>&);
void encrypt();
void decrypt();

int main()
{
    int choice;

    setvbuf(stdout,NULL,_IONBF,0);

    while ((choice = menu()) != 3) {
        switch (choice) {
            case 1: encrypt(); break;
            case 2: decrypt(); break;
        }
    }
}

int menu()
{
    int choice;
    cout << "===== DES Cryptosystem =====" << endl;
    cout << "1. Encrypt" << endl;
    cout << "2. Decrypt" << endl;
    cout << "3. Exit" << endl;
    cout << "Your choice:";
    cin >> choice;

    return choice;
}

inline ul getRow(const bitset<6>& num)
{
    ul n = num.to_ulong();
    return ((n&0x20)>>4) | (n&1);
}

inline ul getCol(const bitset<6>& num)
{
    ul n = num.to_ulong();
    return (n&0x1e)>>1;
}

inline bitset<28> lrot(const bitset<28>& n, ul cnt)
{
    return (n<<cnt) | (n>>(28-cnt));
}

void generateRoundKeys(const bitset<64>& key)
{
    PERM(56, key, PC1)
    //cout << "[!] After PC1:" << endl << res << endl;
    ull r = res.to_ullong();
    bitset<28> c(r >> 28);
    bitset<28> d(r & 0xfffffff);
    for (int i = 0; i < 16; ++i) {
        c = lrot(c, rotate_cnt[i]);
        d = lrot(d, rotate_cnt[i]);
        bitset<56> cd((c.to_ulong()<<28) | d.to_ulong());
        PERM(48, cd, PC2)
        roundKeys[i] = res;
        cout << "[!] Round key " << i << ":" << endl << roundKeys[i] << endl;
    }
}

bitset<32> f(const bitset<32>& r, const bitset<48>& roundKey)
{
    bitset<48> tmp;
    {
        PERM(48, r, E)
        tmp = res ^ roundKey;
    }
    //cout << "[!] After Exp&Xor: " << tmp << endl;
    ul s_output = 0;
    for (int i = 0; i < 8; ++i) {
        bitset<6> s_input((tmp.to_ullong() >> ((7-i)*6)) & 0x3f);
        s_output |= S[i][getRow(s_input)][getCol(s_input)];
        //cout << "[!] s_output[" << i << "]: " << s_output << endl;
        s_output <<= 4;
    }
    bitset<32> inter(s_output>>4);
    PERM(32, inter, P)

    return res;
}

void encrypt()
{
    string k, p;
    cout << "[+] Please input the 64-bit key (e.g. 00101101...):" << endl;
    cin >> k;
    //k = "1010101010111011000010010001100000100111001101101100110011011101";
    bitset<64> key(k);
    generateRoundKeys(key);

    cout << "[+] Please input a 64-bit plaintext:" << endl;
    cin >> p;
    //p = "1010101111001101111001101010101111001101000100110010010100110110";
    bitset<64> plaintext(p);

    ull ru;
    {
        PERM(64, plaintext, IP)
        //cout << "[!]After IP: " << res << endl;
        ru = res.to_ullong();
    }
    bitset<32> l(ru >> 32);
    bitset<32> r(ru & 0xffffffff);
    bitset<32> temp;
    for (int i = 0; i < 16; ++i) {
        temp = r;
        r = l ^ (f(r, roundKeys[i]));
        l = temp;
    }
    bitset<64> lr((r.to_ullong()<<32) | l.to_ullong());
    //cout << "[!] After 16 rounds: " << lr << endl;
    {
        PERM(64, lr, IP_1)
        cout << "[+] Ciphertext:" << endl << res << endl;
    }
}

void decrypt()
{
    string k, c;
    cout << "[+] Please input the 64-bit key (e.g. 00101101...):" << endl;
    cin >> k;
    //k = "1010101010111011000010010001100000100111001101101100110011011101";
    bitset<64> key(k);
    generateRoundKeys(key);

    cout << "[+] Please input a 64-bit ciphertext:" << endl;
    cin >> c;
    //c = "1001111000100110100111110101101011111010010011011011101101110000";
    bitset<64> ciphertext(c);

    ull ru;
    {
        PERM(64, ciphertext, IP)
        ru = res.to_ullong();
    }
    bitset<32> l(ru >> 32);
    bitset<32> r(ru & 0xffffffff);
    bitset<32> temp;
    for (int i = 15; i >= 0; --i)
    {
        temp = r;
        r = l ^ (f(r, roundKeys[i]));
        l = temp;
    }
    bitset<64> lr((r.to_ullong() << 32) | l.to_ullong());
    {
        PERM(64, lr, IP_1)
        cout << "[+] Plaintext:" << endl << res << endl;
    }
}