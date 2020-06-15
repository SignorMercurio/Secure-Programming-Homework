# Week 12

Implement RSA Signature Algorithm based on MD5 Hash Function (Week 12) and RSA Encryption (Week 8).

## Steps

- Sign
  - Input: message `M`, private key `(d, n)`
  - Calculate `h = MD5(M)`
  - Calculate `s = h^d mod n`
  - Send `(M, s)`
- Verify
  - Input: message `M`, signature `s`, public key `(e, n)`
  - Calculate `h = MD5(M)`
  - Calculate `h' = s^e mod n`
  - Verify that `h =?= h'`

## Dependencies

- [GNU MP Library](https://gmplib.org/)

Tested under Ubuntu 16.04 & Ubuntu 19.10.
