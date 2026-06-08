#include <time.h>
#include <stdlib.h>

#include "globe.h"

#define GRID_H 19
#define GRID_W 17

static VEC 
         GLB_Globe[GRID_H][GRID_W],                     /* Globe points array */
         GLB_GlobeN[GRID_H][GRID_W];                    /* Globe normals array */
static INT GLB_RndFrameW, GLB_RndFrameH;                /* Frame widght and height */
static DBL 
         GLB_RndWp, GLB_RndHp,                          /* Project widght and height */
         GLB_RndProjSize = 0.1, GLB_RndProjDist = 0.1;  /* Project distance and size */

COLORREF ColorTo255( VEC Color )
{
  INT
    R = (INT)(Color.X * 255),
    G = (INT)(Color.Y * 255),
    B = (INT)(Color.Z * 255);

  R = GLB_MIN(255, GLB_MAX(0, R));
  G = GLB_MIN(255, GLB_MAX(0, G));
  B = GLB_MIN(255, GLB_MAX(0, B));
  return RGB(R, G, B);
}

/* Power function.
 * ARGUMENTS:
 *   - Variable we want to power:
 *       DBL A;
 *   - rotation power itself:
 *       DBL B;
 * RETURNS:
 *   (DBL) result.
 */
static DBL GLB_Power( DBL A, DBL B )
{
  if (A < 0)
    return -pow(-A, B);
  return pow(A, B);
} /* End of 'GLB_Power' function */

/* Rotate vector around Y axis function.
 * ARGUMENTS:
 *   - vector to be rotate:
 *       VEC V;
 *   - rotation angle in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (VEC) rotated vector.
 */
static VEC RotateY( VEC V, DBL AngleInDegree )
{
  DBL a = AngleInDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = V.Z * si + V.X * co;
  r.Y = V.Y;
  r.Z = V.Z * co - V.X * si;
  return r;
} /* End of 'RotateY' function */

/* Rotate vector around X axis function.
 * ARGUMENTS:
 *   - vector to be rotate:
 *       VEC V;
 *   - rotation angle in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (VEC) rotated vector.
 */
static VEC RotateX( VEC V, DBL AngleInDegree )
{
  DBL a = AngleInDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.Z = V.Y * si + V.Z * co;
  r.X = V.X;
  r.Y = V.Y * co - V.Z * si;
  return r;
} /* End of 'RotateX' function */

/* Rotate vector around Z axis function.
 * ARGUMENTS:
 *   - vector to be rotate:
 *       VEC V;
 *   - rotation angle in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (VEC) rotated vector.
 */                                                                                   
static VEC RotateZ( VEC V, DBL AngleInDegree )
{
  DBL a = AngleInDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.Y = V.X * si + V.Y * co;
  r.Z = V.Z;
  r.X = V.X * co - V.Y * si;
  return r;
} /* End of 'RotateZ' function */

/* Globe initialization function.
 * ARGUMENTS:
 *   - sphere radius:
 *       DBL R;
 * RETURNS:
 *   None.
 */
VOID GLB_Init( DBL R )
{
  INT i, j;
  DBL theta, phi;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      /*
      GLB_Globe[i][j].X = R * sin(theta) * GLB_Power(sin(phi), 0.2);
      GLB_Globe[i][j].Y = R * GLB_Power(cos(theta), 0.3);
      GLB_Globe[i][j].Z = R * sin(theta) * GLB_Power(cos(phi), 0.4);
      */
      GLB_Globe[i][j].X = R * sin(theta) * sin(phi);
      GLB_Globe[i][j].Y = R * cos(theta);
      GLB_Globe[i][j].Z = R * sin(theta) * cos(phi);
    }
    for (i = 0; i < GRID_H - 1; i++)
      for (j = 0; j < GRID_W - 1; j++)
      {
        VEC N1 = VecNormalize(VecCrossVec(VecSubVec(GLB_Globe[i][j + 1], GLB_Globe[i][j]),
                                          VecSubVec(GLB_Globe[i + 1][j], GLB_Globe[i][j])));
        VEC N2 =
          VecNormalize(VecCrossVec(VecSubVec(GLB_Globe[i + 1][j], GLB_Globe[i + 1][j + 1]),
                                   VecSubVec(GLB_Globe[i][j + 1], GLB_Globe[i + 1][j + 1])));
        GLB_GlobeN[i][j] = VecNeg(VecNormalize(VecAddVec(N1, N2)));
      }
} /* End of 'GLB_Init' finction */

/* Globe resize function.
 * ARGUMENTS:
 *   - Sphere widght and height:
 *       INT Ws, Hs;
 * RETURNS:
 *   None.
 */
VOID GLB_Resize( INT Ws, INT Hs )
{
  GLB_RndFrameH = Hs;
  GLB_RndFrameW = Ws;
  if (Ws >= Hs)
    GLB_RndWp = GLB_RndProjSize * Ws / Hs, GLB_RndHp = GLB_RndProjSize;
  else
    GLB_RndWp = GLB_RndProjSize, GLB_RndHp = GLB_RndProjSize * Hs / Ws;
} /* End of 'GLB_Resize' finction */

/* Globe drawing function.
 * ARGUMENTS:
 *   - device context handle:
 *       HDC hDC;
 * RETURNS:
 *   None.
 */
VOID GLB_Draw( HDC hDC )
{
  static POINT pnts[GRID_H][GRID_W];
  POINT ps[4];
  INT i, j, s = 5;
  DBL t = (DBL)clock() / CLOCKS_PER_SEC, xp, yp;
  MATR m = MatrMulMatr3(MatrRotateX(t * 30), MatrRotateY(sin(t) * -30), MatrRotateZ(cos(t) * 30));
  VEC L, L1;

  /*t = GLB_SyncTime;

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));*/

  for (i = 0; i < GRID_H; i++)
  {
    for (j = 0; j < GRID_W; j++)
    {
      //VEC p = RotateZ(RotateX(RotateY(GLB_Globe[i][j], t * 30), sin(t) * -30), cos(t) * 30);
      VEC p = PointTransform(GLB_Globe[i][j], m);
      p.Z -= 5;

      xp = p.X * GLB_RndProjDist / -p.Z;
      yp = p.Y * GLB_RndProjDist / -p.Z;

      pnts[i][j].x = xp * GLB_RndFrameW / GLB_RndWp + GLB_RndFrameW / 2;
      pnts[i][j].y = -yp * GLB_RndFrameH / GLB_RndHp + GLB_RndFrameH / 2;
    }
  }
  /*
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
      Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s,
                   pnts[i][j].x + s, pnts[i][j].y + s);
  for (i = 0; i < GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 1; j < GRID_W; j++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  for (j = 0; j < GRID_W; j++)
  {
    MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
    for (i = 1; i < GRID_H; i++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  */

  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      ps[0] = pnts[i][j];
      ps[1] = pnts[i + 1][j];
      ps[2] = pnts[i + 1][j + 1];
      ps[3] = pnts[i][j + 1];

      if (((i ^ j) & 1) == 0)
        SetDCBrushColor(hDC, RGB(255, 0, 255));
      else
        SetDCBrushColor(hDC, 0);
      if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) + 
        (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) + 
        (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
        (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) <= 0)
        Polygon(hDC, ps, 4);

     L = VecNormalize(VecSet(3, 1, 1));
     L1 = VecNormalize(VecSet(-3, sin(t * 18), 1));
 
     for (i = 0; i < GRID_H - 1; i++)
       for (j = 0; j < GRID_W - 1; j++)
       {
         VEC N, C;
         DBL nl;

         N = VectorTransform(GLB_GlobeN[i][j], m);
         nl = VecDotVec(N, L);
         if (nl < 0.1)
           nl = 0.1;
         C = VecMulNum(VecSet(1, 0.8, 0.30), nl);
 
         nl = VecDotVec(N, L1);
         if (nl < 0.1)
           nl = 0.1;
         C = VecAddVec(C, VecMulNum(VecSet(0.30, 0.8, 0.47), nl));
 
         ps[0] = pnts[i][j];
         ps[1] = pnts[i][j + 1];
         ps[2] = pnts[i + 1][j + 1];
         ps[3] = pnts[i + 1][j];
 
         SetDCBrushColor(hDC, ColorTo255(C));
         /* SetDCBrushColor(hDC,
              ColorTo255(VecDivNum(VecAddVec(N, VecSet1(1)), 2))); */
 
         if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) +
             (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) +
             (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
             (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) > 0)
           Polygon(hDC, ps, 4);
       }
    }
} /* End of 'GLB_Draw' finction */

  /* END OF 'globe.c' FILE */