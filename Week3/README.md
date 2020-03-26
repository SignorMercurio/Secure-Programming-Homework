# Week3

Implement stream cipher cryptosystem.

Polynomial:
```
f(x) = c_4*x^4 + c_3*x^3 + c_2*x^2 + c_1*x + c_0

c_0 = 1
c_1 = 1
c_2 = 0
c_3 = 0
c_4 = 1
```

So we have：
```
a_{i+4} = c_4*a_i + c_3*a_{i+1} + c_2*a_{i+2} + c_1*a_{i+3}
        = a_i + a_{i+3}
```
where `+` refers to XOR.

**Note: Encryption & Decryption are the same.**