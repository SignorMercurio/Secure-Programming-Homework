#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int menu();
void init(vector<bool>&);
vector<bool> str2vb(string);
string vb2str(vector<bool>&);
vector<bool> generateKey(vector<bool>&, int);
string vb_xor(vector<bool>&, vector<bool>&);
void crypt(string);

int main()
{
    int choice;

    setvbuf(stdout,NULL,_IONBF,0);

    while ((choice = menu()) != 3) {
        switch(choice) {
            case 1: crypt("plaintext"); break;
            case 2: crypt("ciphertext"); break;
        }
    }

    return 0;
}

int menu()
{
    int choice;

    cout << "===== Stream Cipher Cryptosystem =====" << endl;
    cout << "1. Encrypt" << endl;
    cout << "2. Decrypt" << endl;
    cout << "3. Exit" << endl;
    cout << "Your choice: ";
    cin >> choice;
    
    return choice;
}

void init(vector<bool>& v)
{
    for (int i = 0; i < 11; ++i)
        v.push_back(v[i] ^ v[i+3]);
}

vector<bool> str2vb(string s)
{
    vector<bool> ret;
    for (int i = 0; i < s.size(); ++i)
        ret.push_back(s[i]=='1' ? true: false);
    
    return ret;
}

string vb2str(vector<bool>& v)
{
    string ret = "";
    for (int i = 0; i < v.size(); ++i)
        ret += (v[i] ? "1":"0");
    
    return ret;
}

vector<bool> generateKey(vector<bool>& keyUnit, int len)
{
    vector<bool> key;
    int copyTimes = (len/15) + 1;

    for (int i = 0; i < copyTimes-1; ++i)
        copy(keyUnit.begin(), keyUnit.end(), back_inserter(key));
    
    copy(keyUnit.begin(), keyUnit.end()-(15-len%15), back_inserter(key));
    return key;
}

string vbXor(vector<bool>& a, vector<bool>& b)
{
    vector<bool> res;
    for (int i = 0; i < a.size(); ++i)
        res.push_back(a[i] ^ b[i]);
    
    return vb2str(res);
}

void crypt(string type)
{
    string s, text;
    int len;

    cout << "Please input IV: (e.g. 1011)" << endl;
    cin >> s;
    vector<bool> keyUnit = str2vb(s);
    init(keyUnit);

    cout << "Please input bit length of " << type << ":" << endl;
    cin >> len;
    vector<bool> key = generateKey(keyUnit, len);
    cout << "Key: " << endl << vb2str(key) << endl;

    cout << "Please input " << type << " bit string:" << endl;
    cin >> text;
    vector<bool> target = str2vb(text);
    
    cout << (type == "plaintext" ? "CipherText":"Plaintext") << ":" << endl;
    cout << vbXor(target, key) << endl;
}