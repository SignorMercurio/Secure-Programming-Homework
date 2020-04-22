#include "AES.cpp"

int menu();
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

    return 0;
}

int menu()
{
    int choice;
    puts("===== AES Cryptosystem =====");
    puts("1. Encrypt");
    puts("2. Decrypt");
    puts("3. Exit");
    puts("Your choice:");
    scanf("%d", &choice);

    return choice;
}

void encrypt()
{
    int keyLen;
    puts("[+] Please input key length (128 / 192 / 256):");
    scanf("%d", &keyLen);
    AES aes(keyLen);

    uc *key = aes.readHex("key", keyLen / 8);
    uc *plain = aes.readHex("plaintext");
    uc *cipher = new uc[16];
    aes.EncryptBlock(plain, cipher, key);
    puts("[+] Ciphertext:");
    aes.printHex(cipher, 16);

    delete[] cipher;
    delete[] plain;
    delete[] key;
}

void decrypt()
{
    int keyLen;
    puts("[+] Please input key length (128 / 192 / 256):");
    scanf("%d", &keyLen);
    AES aes(keyLen);

    uc *key = aes.readHex("key", keyLen / 8);
    uc *cipher = aes.readHex("ciphertext");
    uc *plain = new uc[16];
    aes.DecryptBlock(cipher, plain, key);
    puts("[+] Plaintext:");
    aes.printHex(plain, 16);

    delete[] plain;
    delete[] cipher;
    delete[] key;
}
