#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

float c2f (float c)
{
  return 9*c/5 + 32;
}

float f2c (float f)
{
  return (5*f-32)/9;
}

int main(int argc, char *argv[])
{
  float number = 0;
  int len;

  if(argc != 3)
  {
    printf("Wrong number of arguments supplied\n");
    return 1;
  }

  sscanf(argv[2], "%f%n", &number, &len);

  if(strlen(argv[2]) != len)
  {
    printf("Malformed number in second parameter\n");
    return 3;
  }

  if(strcmp(argv[1],"-f") == 0)
  {
    if(number < c2f(-273.15))
    {
      printf("Temperatures below absolute zero\n");
      return 4;
    }
    printf("%0.2f°C = %.2f°F\n", f2c(number), number);
  }

  else if(strcmp(argv[1],"-c") == 0)
  {
    if(number < -273.15)
    {
      printf("Temperatures below zero\n");
      return 4;
    }
    printf("%0.2f°F = %.2f°C\n", c2f(number), number);
  }
  else
  {
    printf("Failed to supply either -f or -c as the first option\n");
    return 2;
  }
  return 0;
}
