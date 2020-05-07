#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

/*
/  Given how the function has a for loop of max length p, complexity will be p
/  times the complexity of running fme p times for different values of i,
/  which will be an average of the different values of p. Therefore,
/  complexity will be O(p log(p/2))
/  Modular exponentiation is easy to calculate, however, undoing it, in the
/  sense of finding the exponent used, is hard to compute. This makes it a
/  one-way function
*/
unsigned long dl(unsigned long y, unsigned long g, unsigned long p)
{
  unsigned long i;
  for(i=0; i<p; i++)
    if(fme(g, i, p) == y)
      return i;

  return 0;
}

int main()
{
  unsigned long g = 445;
  unsigned long x = 4;
  unsigned long p = 497;
  unsigned long result = dl(g, x, p);
  printf("Result of fme: %lu\n", result);

  return 0;
}
