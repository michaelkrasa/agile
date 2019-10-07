#include <stdio.h>
//#include "name.h"
#include "hello.h"

//Prints my name
int main()
{
#ifdef WORLD
  sayHello("World");
#elif defined(NAME)
  sayHello(STR(NAME));
#else
  sayHello("Nobody");
#endif
  return 0;
}
void sayHello(char* name)
{
  printf("Hello %s!\n",name);
}
