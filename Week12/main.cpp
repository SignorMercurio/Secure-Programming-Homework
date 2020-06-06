#include "md5.cpp"

int menu();
void calcMD5();

int main()
{
   int choice;

   setvbuf(stdout, NULL, _IONBF, 0);

   while ((choice = menu()) != 2)
   {
      switch (choice)
      {
      case 1:
         calcMD5();
         break;
      }
   }

   return 0;
}

int menu()
{
   int choice;
   puts("===== MD5 Hash Function =====");
   puts("1. Digest");
   puts("2. Exit");
   puts("Your choice:");
   scanf("%d", &choice);
   getchar();

   return choice;
}

void calcMD5()
{
   string msg = "";
   cout << "[+] Please input your message:" << endl;
   getline(cin, msg);
   MD5 md5(msg);
   cout << "[+] MD5(\"" << msg << "\") = " << md5.hexdigest() << endl;
}