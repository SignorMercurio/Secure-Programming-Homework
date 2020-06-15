#ifndef MD5_H
#define MD5_H

#include <iostream>
#include <string>
using namespace std;

// Reference: RFC 1321
class MD5
{
public:
  typedef unsigned int size_t;

  MD5();
  MD5(const string &);
  void update(const unsigned char *, size_t);
  void update(const char *, size_t);
  MD5 &finalize();
  string hexdigest() const;

private:
  typedef unsigned char uint1;
  typedef unsigned int uint4;

  void init();
  static const int blocksize = 64;

  void transform(const uint1 block[blocksize]);
  static void decode(uint4 output[], const uint1 input[], size_t len);
  static void encode(uint1 output[], const uint4 input[], size_t len);

  bool finalized;
  uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
  uint4 count[2];          // 64bit counter for number of bits (low, high)
  uint4 state[4];
  uint1 digest[16];

  static inline uint4 F(uint4, uint4, uint4);
  static inline uint4 G(uint4, uint4, uint4);
  static inline uint4 H(uint4, uint4, uint4);
  static inline uint4 I(uint4, uint4, uint4);
  static inline uint4 lrot(uint4, int);
  static inline void FF(uint4 &, uint4, uint4, uint4, uint4, uint4, uint4);
  static inline void GG(uint4 &, uint4, uint4, uint4, uint4, uint4, uint4);
  static inline void HH(uint4 &, uint4, uint4, uint4, uint4, uint4, uint4);
  static inline void II(uint4 &, uint4, uint4, uint4, uint4, uint4, uint4);
};

#endif
