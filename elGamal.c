#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Task 1
// Implemetation of Euclidean algorithm
unsigned long hcf(unsigned long long a, unsigned long long b) {
  if(b == 0)
    return a;
  else {
    unsigned long long c = a % b;
    return hcf(b, c);
  }
}

/* Task 2
/  Fast modular exponentiation
/  The running time of this function is O(log x) because in the while
/  loop we're diving x by two until the condition is satisfied.
*/
unsigned long fme(unsigned long long g, unsigned long long x, unsigned long p) {

  unsigned long long result = 1;
  g %= p; // Update g if g >= p

  if (g == 0) return 0; // if g is divisible by p;

  while (x > 0) {
    // If x is odd, multiply g with result
    if (x % 2 == 1)
      result = (result * g) % p;

    // x is even
    x /= 2;
    g = (g * g) % p;
  }
  return result;
}

/* Task 4
/  x * y = 1 mod p
/  using Fermat's little theorem because we know p is prime
/  Since we're only using the fme function, the complexity is the same
/  which is O(log p)
*/
unsigned long imp(unsigned long long y, unsigned long long p) {
  return fme(y, p-2, p);
}

int main() {

  unsigned long p = 65537;
  unsigned long g = 3;
  unsigned long a, b, k, inv;
  unsigned long msg, secret, privKey, pubKey;

  char in;

  printf("Prime modulus is 65537\n");
  printf("Primitive root wrt 65537 is 3\n");

  while (1) {
    printf("Choose: e (encrypt) | d (decrypt) | k (get public key) | x (exit)?\n");
    scanf(" %c", &in); // read char

    switch (in) {
      case 'e': printf("Type your message: "); scanf("%lu", &msg);
                printf("Recipient's public key: "); scanf("%lu", &pubKey);
                k = rand() % p;
                if (k == 0) k++;
                a = fme(g, k, p);
                b = msg * fme(pubKey, k, p) % p;
                printf("\nEncrypted secret is (%lu, %lu)\n", a, b);
                break;

      case 'd': printf("Type in the received message in form (a,b): \n");
                scanf(" ( %lu,%lu)", &a, &b);
                printf("Type in your private key: \n");
                scanf(" %lu", &privKey);

                secret = fme(a, privKey, p);
                inv = imp(secret, p);
                msg = b * inv % p;
                printf("The decrypted secret is: %lu\n", msg); break;

      case 'k': printf("Type private key: "); scanf(" %lu", &privKey);
                pubKey = fme(g, privKey, p);
                printf("Public key is: %lu\n", pubKey); break;

      case 'x': printf("Goodbye\n"); return 0; break;
      default: break;
    }

  }


  return 0;
}
