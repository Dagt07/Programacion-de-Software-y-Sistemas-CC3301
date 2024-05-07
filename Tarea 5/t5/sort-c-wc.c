#include <string.h>

int contarpalabras(char *s) {
  int cont= 0;
  char *p= s;
  while (*p != 0) {
    if (*p != ' ') { 
      cont++;
      while (*p != ' ' && *p != 0)
        p++;
    }
    else
      p++;
  }
  return cont;
}

void sort(char **a, int n) {
  char **ult= &a[n-1];
  char **p= a;
  while (p<ult) {

    int t1= contarpalabras(p[0]) - contarpalabras(p[1]);

    if (t1 <= 0)
      p++;
    else {
      char *tmp= p[0];
      p[0]= p[1];
      p[1]= tmp;
      p= a;
    }
  }
}
