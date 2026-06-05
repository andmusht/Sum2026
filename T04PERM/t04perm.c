#include <stdio.h>

#define MAX 3
static int IsParity = 1;

void Swap( int *a, int *b )
{
  int c;

  c = *a;
  *a = *b;
  *b = c;
}

void Store( int Size, int *a )
{
  FILE *F;
  int i;

  if((F = fopen("a.txt", "a")) == NULL)
    return;
  
  for (i = 0; i < Size; i++)
    fprintf(F, "%i", a[i]);

  fprintf(F, ", Parity: %s \n", IsParity ? "even" : "odd");
  fclose(F);
}

void f( int n,  int Size, int *a )
{
  int i, SaveParity;

  if (n >= Size)
  {
    Store(Size, a);
    return;
  }

  for (i = n; i < Size; i++)
  {
    Swap(&a[n], &a[i]);
    if (n != i)
      IsParity = !IsParity;

    f(n + 1, Size, a);
    Swap(&a[n], &a[i]);
  }
}

void main( void )
{
  int a[MAX], i;

  for (i = 0; i < MAX; i++)
    a[i] = i + 1;

  f(0, MAX, a);
  getchar();
}
