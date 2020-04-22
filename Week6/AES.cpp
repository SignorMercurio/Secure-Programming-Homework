#include "AES.h"
#include <unistd.h>

AES::AES(int keyLen)
{
  Nb = 4;
  Nk = keyLen / 32;
  switch (keyLen)
  {
  case 128: Nr = 10; break;
  case 192: Nr = 12; break;
  case 256: Nr = 14; break;
  default: puts("Invalid key length"); _exit(1);
  }
}

void AES::EncryptBlock(uc in[], uc out[], uc key[])
{
  uc *w = new uc[4 * Nb * (Nr+1)];
  KeyExpansion(key, w);

  uc **state = new uc *[4];
  state[0] = new uc[4 * Nb];
  int i, j;
  for (i = 0; i < 4; ++i) {
    state[i] = state[0] + i*Nb;
  }

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < Nb; ++j) {
      state[i][j] = in[i + 4*j];
    }
  }

  AddRoundKey(state, w);
  for (int round = 1; round <= Nr-1; ++round) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, w + round*4*Nb);
  }
  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, w + Nr*4*Nb);

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < Nb; ++j) {
      out[i + 4*j] = state[i][j];
    }
  }

  delete[] state[0];
  delete[] state;
  delete[] w;
}

void AES::DecryptBlock(uc in[], uc out[], uc key[])
{
  uc *w = new uc[4 * Nb * (Nr+1)];
  KeyExpansion(key, w);

  uc **state = new uc *[4];
  state[0] = new uc[4 * Nb];
  int i, j;
  for (i = 0; i < 4; ++i) {
    state[i] = state[0] + i*Nb;
  }

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < Nb; ++j) {
      state[i][j] = in[i + 4*j];
    }
  }

  AddRoundKey(state, w + Nr*4*Nb);
  for (int round = Nr-1; round >= 1; --round) {
    InvSubBytes(state);
    InvShiftRows(state);
    AddRoundKey(state, w + round*4*Nb);
    InvMixColumns(state);
  }
  InvSubBytes(state);
  InvShiftRows(state);
  AddRoundKey(state, w);

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < Nb; ++j) {
      out[i + 4*j] = state[i][j];
    }
  }

  delete[] state[0];
  delete[] state;
  delete[] w;
}

void AES::SubBytes(uc **state)
{
  uc t;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < Nb; ++j) {
      t = state[i][j];
      state[i][j] = sbox[t / 16][t % 16];
    }
  }
}

void AES::InvSubBytes(uc **state)
{
  uc t;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < Nb; ++j) {
      t = state[i][j];
      state[i][j] = inv_sbox[t / 16][t % 16];
    }
  }
}

void AES::ShiftRows(uc **state)
{
  Shift1Row(state, 1, 1);
  Shift1Row(state, 2, 2);
  Shift1Row(state, 3, 3);
}

void AES::InvShiftRows(uc **state)
{
  Shift1Row(state, 1, Nb-1);
  Shift1Row(state, 2, Nb-2);
  Shift1Row(state, 3, Nb-3);
}

void AES::MixColumns(uc **state)
{
  uc col[4], newCol[4];

  for (int j = 0; j < Nb; ++j) {
    for (int i = 0; i < 4; ++i) {
      col[i] = state[i][j];
    }

    newCol[0] = Mul(0x02, col[0]) ^ Mul(0x03, col[1]) ^ col[2]            ^ col[3];
    newCol[1] = col[0]            ^ Mul(0x02, col[1]) ^ Mul(0x03, col[2]) ^ col[3];
    newCol[2] = col[0]            ^ col[1]            ^ Mul(0x02, col[2]) ^ Mul(0x03, col[3]);
    newCol[3] = Mul(0x03, col[0]) ^ col[1]            ^ col[2]            ^ Mul(0x02, col[3]);

    for (int i = 0; i < 4; ++i) {
      state[i][j] = newCol[i];
    }
  }
}

void AES::InvMixColumns(uc **state)
{
  uc col[4], newCol[4];

  for (int j = 0; j < Nb; ++j) {
    for (int i = 0; i < 4; ++i) {
      col[i] = state[i][j];
    }

    newCol[0] = Mul(0x0e, col[0]) ^ Mul(0x0b, col[1]) ^ Mul(0x0d, col[2]) ^ Mul(0x09, col[3]);
    newCol[1] = Mul(0x09, col[0]) ^ Mul(0x0e, col[1]) ^ Mul(0x0b, col[2]) ^ Mul(0x0d, col[3]);
    newCol[2] = Mul(0x0d, col[0]) ^ Mul(0x09, col[1]) ^ Mul(0x0e, col[2]) ^ Mul(0x0b, col[3]);
    newCol[3] = Mul(0x0b, col[0]) ^ Mul(0x0d, col[1]) ^ Mul(0x09, col[2]) ^ Mul(0x0e, col[3]);

    for (int i = 0; i < 4; ++i) {
      state[i][j] = newCol[i];
    }
  }
}

void AES::AddRoundKey(uc **state, uc *key)
{
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < Nb; ++j) {
      state[i][j] ^= key[i + 4*j];
    }
  }
}

void AES::Shift1Row(uc **state, int row, int n)
{
  uc t;
  for (int i = 0; i < n; ++i) {
    t = state[row][0];
    for (int col = 0; col < Nb - 1; ++col) {
      state[row][col] = state[row][col + 1];
    }
    state[row][Nb - 1] = t;
  }
}

uc AES::Doubles(uc b)
{
  uc mask = 0x80; // 1000 0000
  uc m = 0x1b;    // 0001 1011
  uc msb = b & mask;
  b <<= 1;
  if (msb) { // mod m(x)
    b ^= m;
  }

  return b;
}

uc AES::Mul(uc a, uc b)
{
  uc res = 0, lsb, t;
  for (int i = 0; i < 8; ++i) {
    lsb = b & 1;
    if (lsb) {
      t = a;
      for (int j = 0; j < i; ++j) { // multiply on x^i
        t = Doubles(t);
      }
      res ^= t;
    }
    b >>= 1;
  }

  return res;
}

void AES::RotWord(uc *a)
{
  uc t = a[0];
  a[0] = a[1];
  a[1] = a[2];
  a[2] = a[3];
  a[3] = t;
}

void AES::SubWord(uc *a)
{
  for (int i = 0; i < 4; ++i) {
    a[i] = sbox[a[i] / 16][a[i] % 16];
  }
}

void AES::Rcon(uc *a, int n)
{
  uc t = 1;
  for (int i = 0; i < n-1; ++i) {
    t = Doubles(t);
  }

  a[0] = t;
  a[1] = a[2] = a[3] = 0;
}

void AES::XorWord(uc *a, uc *b, uc *c)
{
  for (int i = 0; i < 4; ++i) {
    c[i] = a[i] ^ b[i];
  }
}

void AES::KeyExpansion(uc key[], uc w[])
{
  uc *t = new uc[4];
  uc *rcon = new uc[4];

  for (int i = 0; i < 4*Nk; ++i) {
    w[i] = key[i];
  }

  for (int i = 4*Nk; i < 4 * Nb * (Nr+1); i += 4) {
    t[0] = w[i-4 + 0];
    t[1] = w[i-4 + 1];
    t[2] = w[i-4 + 2];
    t[3] = w[i-4 + 3];

    if (i / 4 % Nk == 0) {
      RotWord(t);
      SubWord(t);
      Rcon(rcon, i / (Nk * 4));
      XorWord(t, rcon, t);
    }
    else if (Nk > 6 && i / 4 % Nk == 4) {
      SubWord(t);
    }

    w[i+0] = w[i+0 - 4*Nk] ^ t[0];
    w[i+1] = w[i+1 - 4*Nk] ^ t[1];
    w[i+2] = w[i+2 - 4*Nk] ^ t[2];
    w[i+3] = w[i+3 - 4*Nk] ^ t[3];
  }

  delete[] rcon;
  delete[] t;
}

uc* AES::readHex(const char* type, int bytes=16)
{
  uc *ret = new uc[bytes];
  printf("[+] Please input the %s in hex form (e.g. 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff):\n", type);
  for (int i = 0; i < bytes; ++i) {
    scanf("%02hhx", &ret[i]);
  }

  return ret;
}

void AES::printHex(uc a[], int n)
{
  for (int i = 0; i < n; ++i) {
    printf("%02x ", a[i]);
  }
  puts("");
}
