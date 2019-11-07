#define HOW_MANY 7

char *names[HOW_MANY] = { "Simon", "Suzie", "Alfred", "Chip", "John", "Tim",
"Harriet" };
int ages[HOW_MANY] = { 22, 24, 106, 6, 18, 32, 24 };
int person_type[HOW_MANY] = { 1,1,2,2,1,0,0 };
char *info[HOW_MANY] = { "Computer Science", "Artificial Intelligence", "",
"", "HCI", "Kilburn 2.72", "QL" };


/* declare union for a person here */
typedef struct person {
  char* name;
  int age;
  struct person* next;
} person;
