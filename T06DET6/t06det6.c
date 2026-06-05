#include <stdio.h>
#include <math.h>

typedef double DBL;
typedef char CHAR;
typedef int BOOL;
typedef int INT;
typedef void VOID;

#define TRUE 1
#define FALSE 0

#define MAX 10

DBL A[MAX][MAX], Det = 0;
INT N, P[MAX];

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &N);

  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
}

VOID Swap( DBL *A, DBL *B )
{
  DBL tmp;

  tmp = *A;
  *A = *B;
  *B = tmp;
}

VOID f( VOID )
{
  INT i, y, x, k, max_row, max_col;
  DBL coef;

  Det = 1;
  for (i = 0; i < N; i++)
  {
    /* look for maximum matrix element */
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_row = y, max_col = x;

    if (A[max_row][max_col] == 0)
    {
      Det = 0;
      break;
    }

    /* move max element to [i][i] position */
    if (max_row != i)
    {
      /* Swap max_row and i row (elements/columns: [i..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[max_row][x], &A[i][x]);
      Det = -Det;
    }
    
    if (max_col != i)
    {
      /* Swap max_col and i column (elements/columns: [0..N-1]) */
      for (y = i; y < N; y++)
        Swap(&A[y][max_col], &A[y][i]);
      Det = -Det;
    }
    /* Subtrack from every row k:[i+1..N-1] row [i] multipled by (A[k][i] / A[i][i]) */
    for (k = i + 1; k < N; k++)
    {
      coef = A[k][i] / A[i][i];
      A[k][i] = 0;
      for (x = i + 1; x < N; x++)
        A[k][x] -= A[i][x] * coef;
    }

    Det *= A[i][i];
  }
}

VOID main( VOID )
{
  if (LoadMatrix("b.txt"))
  {
    f();
    printf("Determinator is %lf\n", Det);
  }
  else
    printf("matrix is not loaded\n");
  getchar();
}
