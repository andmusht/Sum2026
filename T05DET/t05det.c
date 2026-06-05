#include <stdio.h>

typedef double DBL;
typedef char CHAR;
typedef int BOOL;
typedef int INT;
typedef void VOID;

#define TRUE 1
#define FALSE 0
#define MAX 10 

DBL A[MAX][MAX],  Det = 0;
INT N, P[MAX];
BOOL IsParity = TRUE;

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &N);

  if (N < 0)
    N = 0;
  else
    if (N > MAX)
      N = MAX;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
}

VOID Swap( INT *A, INT *B )
{
  INT tmp;

  tmp = *A;
  *A = *B;
  *B = tmp;
}

VOID f( INT Pos )
{
  INT i;
  DBL prod;

  if (Pos >= N)
  {
    for (prod = 1.0, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity ? 1 : -1);
  }
  else
  {
    for (i = Pos; i < N; i++)
      {
        Swap(&P[Pos], &P[i]);
        if (Pos != i)
          IsParity = !IsParity;
        f(Pos + 1);
        if (Pos != i)
          IsParity = !IsParity; 
        Swap(&P[Pos], &P[i]);
    }
  }
}

VOID main( VOID )
{
  INT i;

  if (LoadMatrix("b.txt"))
  {
    for (i = 0; i < N; i++)
      P[i] = i;
    LoadMatrix("b.txt");
    f(0);
    printf("Determinator is %f\n", Det);
  }
  else
    printf("Matrix is not loaded\n");
  getchar();
}
