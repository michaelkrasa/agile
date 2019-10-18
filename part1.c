#include <ctype.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  char inputname[30];
  char outputname[30];
  printf("Type name of the input file:  ");
  scanf("%s", inputname);
  printf("Type name of the output file: ");
  scanf("%s", outputname);

  int uppercase  = 0, lowercase = 0, i = 0;
  char data;

  // A stream for getting data from the user.
  FILE *file1= fopen(inputname, "r");
  FILE *file2= fopen(outputname, "w");

  if (!file1)
    fprintf(stderr, "can't open %s for reading\n", inputname);

  while((data=fgetc(file1))!= EOF)
  {
    if(isupper(data))
    {
      fprintf(file2, "%c", tolower(data));
      uppercase++;
    }
    else if(islower(data))
    {
      fprintf(file2, "%c", toupper(data));
      lowercase++;
    }
    else
      fprintf(file2, "%c", data);

    i++;
  }
  fclose(file1);
  fclose(file2);
  printf("\nRead %d characters in total, %d converted to upper-case, %d to lower-case\n", i, lowercase, uppercase);

  return 0;
}
