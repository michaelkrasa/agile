#include <stdio.h>

int main(int argc, char **argv)
{
  int noOfChars = 0;

  char input[30];
  fgets(input, sizeof(input), stdin);
  int i;
  char temp;

  size_t len = strlen(input);

  for (i = 0; i < len; i++)
  {
    temp = input[i];
    if(input[i].isUpper())
    {
      input[i] = temp.toLower();
    }
    else
    {
      input[i] = temp.toUpper();
    }
  }
  #printf("Read %d characters in total, %d converted to upper-case, %d to lower-case\n", noOfChars, toUpper, toLower);
}
