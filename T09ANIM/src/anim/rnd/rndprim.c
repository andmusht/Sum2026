#include <stdio.h>

#include "rnd.h"

VOID AM6_RndPrimFree( am6PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(am6VERTEX));  /* <-- <string.h>, WinAPI, ZeroMemory */
}

BOOL AM6_RndPrimCreate( am6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(am6PRIM));
  size = sizeof(am6VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);
  return TRUE;
}

VOID AM6_RndPrimDraw( am6PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, AM6_RndMatrVP);
  POINT *pnts;

  glLoadMatrixf(wvp.A[0]);

  /*if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;*/

  /* Build vertex projects */
  /*for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);

    pnts[i].x = (INT)((p.X + 1) * AM6_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * AM6_RndFrameH / 2);
  }*/

  /* Draw triangles by edges */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor4fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }

  glEnd();

  /*SelectObject(AM6_hRndDC, GetStockObject(BLACK_PEN));*/
  /*for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(AM6_hRndDC, pnts[Pr->I[i + 0]].x, pnts[Pr->I[i + 0]].y, NULL);
    LineTo(AM6_hRndDC, pnts[Pr->I[i + 1]].x,   pnts[Pr->I[i + 1]].y);
    LineTo(AM6_hRndDC, pnts[Pr->I[i + 2]].x,   pnts[Pr->I[i + 2]].y);
    LineTo(AM6_hRndDC, pnts[Pr->I[i + 0]].x,   pnts[Pr->I[i + 0]].y);
  }

  free(pnts);*/
}

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       am6PRIM *Pr;
 *   - sphere radius:
 *       DBL R;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndPrimCreateSphere( am6PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;

  if (!AM6_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;

  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));

  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
} /* End of 'AM6_RndPrimCreateSphere' function */

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       am6PRIM *Pr;
 *   - sphere radius:
 *       DBL R;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndPrimCreateCylinder( am6PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;

  if (!AM6_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;

  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 3 * PI / (W - 1))
      if (i == 0)
        Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R / 2,
                            R * sin(theta) * cos(phi));
      else if (i == H - 1)
        Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            -R / 2,
                            R * cos(phi) * sin(theta));
      else
        Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                              R * cos(theta),
                              R * sin(theta) * cos(phi));

  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i *   + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
} /* End of 'AM6_RndPrimCreateSphere' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       am6PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL AM6_RndPrimLoad( am6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0, i;
  VEC L = VecNormalize(VecSet1(1));
  static CHAR Buf[3000];

  memset(Pr, 0, sizeof(am6PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices and indices */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';

      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
          n++;
        oldc = *ptr++;
      }

      nf += n - 2;
    }
  }

  if (!AM6_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }

  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n, n1, n2, n3;
      INT fvn = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';

      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
        {
          sscanf(ptr, "%d", &n);
          if (n > 0)
            n--;
          else
            if (n < 0)
              n = nv + n;

          if (fvn == 0)
            n1 = n;
          else if (fvn == 1)
            n2 = n;
          else
          {
            n3 = n;

            Pr->I[nf++] = n1;
            Pr->I[nf++] = n2;
            Pr->I[nf++] = n3;

            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }

  fclose(F);

  AM6_RndPrimTriMeshAutoNormals(Pr->V, Pr->NumOfV, Pr->I, Pr->NumOfI);

  for (i = 0; i < Pr->NumOfV; i++)
  {
    FLT nl = VecDotVec(Pr->V[i].N, L);

    if (nl < 0.1)
      nl = 0.1;
    Pr->V[i].C = Vec4Set(0.54 * nl + 0.2, 0.17 * nl + 0.2, 0.80 * nl + 0.2, 1);
  }

  return TRUE;
} /* End of 'AM6_RndPrimLoad' function */

/* Tri-mesh geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - vertex array:
 *       am6VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *Ind;
 *   - index array size:
 *       INT NumOfI;
 */
VOID AM6_RndPrimTriMeshAutoNormals( am6VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecSet1(0);

  for (i = 0; i < NumOfI; i += 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

    V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
    V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
    V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'AM6_RndPrimTriMeshAutoNormals' function */