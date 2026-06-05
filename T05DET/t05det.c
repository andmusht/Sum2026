#include <stdio.h>

typedef double DBL;
typedef char CHAR;
typedef int BOOL;
typedef int INT;
typedef void VOID;

#define TRUE 1
#define FALSE 0
#define MAX 3

DBL A[MAX][MAX], prod = 1, Det = 0;
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

VOID Swap( INT *a, INT *b )
{
  INT c;

  c = *a;
  *a = *b;
  *b = c;
}

DBL f( INT n )
{
  INT i;

  if (n == MAX)
  {
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity ? 1 : -1);
  }

  for (i = n; i < MAX; i++)
    {
      Swap(&P[n], &P[i]);
      if (n != i)
        IsParity = !IsParity;

      f(n + 1);
      Swap(&P[n], &P[i]);
      IsParity = !IsParity;
    }

  return Det;
}

VOID main( VOID )
{
  INT i;
  DBL Det;

  for (i = 0; i < MAX; i++)
    P[i] = i;

  LoadMatrix("b.txt");
  Det = f(0);
  printf("%f", Det);
  getchar();
}
