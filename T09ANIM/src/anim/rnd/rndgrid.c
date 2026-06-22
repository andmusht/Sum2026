#include "rnd.h"

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndGridCreate( am6GRID *G, INT W, INT H )
{
  INT i, j;
  memset(G, 0, sizeof(am6GRID));
  G->W = W;
  G->H = H;

  if ((G->V = malloc(sizeof(am6VERTEX) * W * H)) == NULL)
    return FALSE;

  memset(G->V, 0, sizeof(am6VERTEX) * W * H);

  for (j = 0; j < W; j++)
    for (i = 0; i < H; i++)
    {
      G->V[i * W + j].N = VecSet(i, 1, -j);
      G->V[i * W + j].T.X = j / (W - 1.0);
      G->V[i * W + j].T.Y = i / (H - 1.0);
    }

  return TRUE;
} /* End of 'AM6_RndGridCreate' function */

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 * RETURNS: None.
 */
VOID AM6_RndGridFree( am6GRID *G )
{
  free(G->V);
  memset(G, 0, sizeof(am6GRID));
} /* End of 'AM6_RndGridFree' function */

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       am6PRIM *Pr;
 *   - grid data:
 *       am6GRID *G;
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndPrimFromGrid( am6PRIM *Pr, am6GRID *G )
{   
  INT k, i, j, H = G->H, W = G->W, *Ind;

  if ((Ind = malloc(sizeof(INT) * ((H - 1) * (W * 2 + 1) - 1))) == NULL)
    return FALSE;
  for (k = 0, i = 0; i < H - 1; i++)
  {
    for (j = 0; j < W; j++)
    {
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j;
    }
    if (i != H - 2)
      Ind[k++] = -1;
  }
  AM6_RndPrimCreate(Pr, AM6_RND_PRIM_TRISTRIP, G->V, W * H,
    Ind, (H - 1) * (W * 2 + 1) - 1);

  free(Ind);

  return TRUE;
} /* End of 'AM6_RndPrimFromGrid' function */

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 * RETURNS: None.
 */
VOID AM6_RndGridAutoNormals( am6GRID *G )
{
  INT i, j, W = G->W, H = G->H;     

  for (i = 0; i < W * H; i++)
    G->V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < H - 1; i++)
  {
    for (j = 0; j < W - 1; j++)
    {
      am6VERTEX
        *P00 = G->V + i * W + j,
        *P01 = G->V + i * W + j + 1,
        *P10 = G->V + (i + 1) * W + j,
        *P11 = G->V + (i + 1) * W + j + 1;
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }
    G->V[i].N = VecNormalize(G->V[i].N);
  }

  for (i = 0; i < W * H; i++)                      
    G->V[i].C = Vec4Set(0.8, 0.47, 0.30, 1);
} /* End of 'AM6_RndGridAutoNormals' function */

/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndGridCreateSphere( am6GRID *G, FLT R, INT W, INT H )
{
  INT i, j, k;                   
  DBL theta, phi;

  if (!AM6_RndGridCreate(G, W, H))
    return FALSE;

  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      G->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                           R * cos(theta),
                           R * sin(theta) * cos(phi));

  return TRUE;
} /* End of 'AM6_RndGridCreateSphere' function */

/* END OF 'rndgrid.c' FILE */