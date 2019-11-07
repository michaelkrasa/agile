/* declare union for a person here */
typedef struct person {
  char* name;
  int age;
  struct person* next;
} person;


struct person* insert_start(struct person* head, char *name, int age);
struct person* insert_end(struct person* head, char *name, int age);
struct person* insert_sorted(struct person* head, char *name, int age, int (*compare_people)(struct person*, struct person*));
