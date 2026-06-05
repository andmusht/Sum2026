#include <stdio.h>

#define MAX 4 
static int IsParity = 1;

void Swap( int *A, int *B )
{
  int tmp;

  tmp = *A;
  *A = *B;
  *B = tmp;
}

void Store( int Size, int *A )
{
  FILE *F;
  int i;

  if((F = fopen("a.txt", "a")) == NULL)
    return;
  
  for (i = 0; i < Size; i++)
    fprintf(F, "%i", A[i]);

  fprintf(F, ", Parity: %s \n", IsParity ? "even" : "odd");
  fclose(F);
}

void f( int N, int Size, int *A )
{
  int i;

  if (N >= Size)
  {
    Store(Size, A);
    return;
  }

  for (i = N; i < Size; i++)
  {
    Swap(&A[N], &A[i]);
    if (N != i)
      IsParity = !IsParity;

    f(N + 1, Size, A);
    Swap(&A[N], &A[i]);
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
