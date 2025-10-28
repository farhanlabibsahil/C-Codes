#include <stdio.h>


struct myStructure {
  int myNum;
  char myLetter;
};

int main() {
  int n, i;
  printf("Enter number of elements: ");
  scanf("%d", &n);

  struct myStructure s1[n];

  for (i = 0; i < n; i++) {
    printf("Enter Number: ");
    scanf("%d", &s1[i].myNum);
    printf("Enter Letter: ");
    scanf(" %c", &s1[i].myLetter); 
  }

  for (i = 0; i < n; i++) {
    printf("My number: %d\n", s1[i].myNum);
    printf("My letter: %c\n", s1[i].myLetter);
  }

  return 0;
}
