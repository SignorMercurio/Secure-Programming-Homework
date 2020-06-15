/* g++ main.cpp -g -lgmpxx -lgmp -std=c++11
 * Author: Mercury
 * Tested under Ubuntu 16.04 & 19.10
*/
#include "MD5/md5.cpp"
#include "RSA/rsa.cpp"

int menu();
void paramGen(mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void showParams();
void sign();
void verify();

mpz_class p, q, n, phi, e, d;

int main()
{
  int choice;

  setvbuf(stdout, NULL, _IONBF, 0);

  while ((choice = menu()) != 5)
  {
    switch (choice)
    {
    case 1:
      paramGen(p, q, n, phi, e, d);
      break;
    case 2:
      showParams();
      break;
    case 3:
      sign();
      break;
    case 4:
      verify();
      break;
    }
  }

  return 0;
}

int menu()
{
  int choice;
  puts("\n===== RSA Signature Algorithm =====");
  puts("1. Generate Params");
  puts("2. Show Params");
  puts("3. Sign");
  puts("4. Verify");
  puts("5. Exit");
  puts("Your choice:");
  scanf("%d", &choice);
  getchar();

  return choice;
}

void showParams()
{
  cout << "[*] p: " << p << endl;
  cout << "[*] q: " << q << endl;
  cout << "[+] n: " << n << endl;
  cout << "[*] phi(n): " << phi << endl;
  cout << "[+] e: " << e << endl;
  cout << "[*] d: " << d << endl;
}

string getMD5(string &msg)
{
  cout << "[+] Please input your message:" << endl;
  getline(cin, msg);
  MD5 md5(msg);
  string digest = md5.hexdigest();
  cout << "[+] MD5(\"" << msg << "\") = " << digest << endl;

  return digest;
}

void sign()
{
  string msg = "";
  mpz_class h(getMD5(msg), 16);
  mpz_class s = decrypt(h.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  cout << "[+] (M, s) = (" << msg << ", " << s.get_str(16) << ")" << endl;
}

void verify()
{
  string msg = "", sig = "";
  mpz_class h(getMD5(msg), 16);
  cout << "[+] Please input the signature(in hex form):" << endl;
  cin >> sig;
  mpz_class s(sig, 16);
  mpz_class h_ = encrypt(s.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  cout << "[+] Verify result: " << (h == h_ ? "Passed!" : "Failed!") << endl;
}