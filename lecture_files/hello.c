#include <stdio.h>
//#include "name.h"
#include "hello.h"

//Prints my name
int main()
{
  sayHello(NAME);
  return 0;
}
void sayHello(char* name)
{
  printf("Hello %s!\n",name);
}
