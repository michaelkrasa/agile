#include <stdio.h>
#include <stdlib.h>

struct A {
  char x;
  char y;
  int z;
} sA;

typedef struct {
  char x;
  int z;
  char y;
} B;

int main()
{
 sA.x = 'a';
 struct A sC = sA;
 struct {int a,b;} sD = {1,2};
 B* sE = malloc(sizeof(B)); 
 printf("%c %d %d\n",sC.x,sD.b,sE->z); 
 printf("%d %d\n",sizeof(sA),sizeof(B));
}
