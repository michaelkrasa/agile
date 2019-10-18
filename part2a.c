#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int i = 0, max = 0, maxPos = 0;

  if(argc == 1)
    return 0;

  int lengths[argc-1];

  for(i=1; i < argc; i++)
    lengths[i-1] = strlen(argv[i]);

  for(i=0; i < argc-1; i++)
    if(lengths[i] > max)
    {
      max = lengths[i];
      maxPos = i;
    }
  
  printf("%s\n", argv[maxPos+1]);
  return 0;
}
