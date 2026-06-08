#include <time.h>

#include "globe.h"

#define GRID_H 19
#define GRID_W 17

static VEC GLB_Globe[GRID_H][GRID_W];                   /* Globe points array */
static INT GLB_RndFrameW, GLB_RndFrameH;                /* Frame widght and height */
static DBL 
         GLB_RndWp, GLB_RndHp,                          /* Project widght and height */
         GLB_RndProjSize = 0.1, GLB_RndProjDist = 0.1;  /* Project distance and size */

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
      GLB_Globe[i][j].X = R * sin(theta) * GLB_Power(sin(phi), 0.2);
      GLB_Globe[i][j].Y = R * GLB_Power(cos(theta), 0.3);
      GLB_Globe[i][j].Z = R * sin(theta) * GLB_Power(cos(phi), 0.4);
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
/*  static POINT pnts[GRID_H][GRID_W];
  POINT ps[4];
  INT i, j, s = 5;
  DBL t, xp, yp;

  t = GLB_SyncTime;

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));

  for (i = 0; i < GRID_H; i++)
  {
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = RotateZ(RotateX(RotateY(GLB_Globe[i][j], t * 30), sin(t) * -30), cos(t) * 30);
      
      p.Z -= 5;

      xp = p.X * GLB_RndProjDist / -p.Z;
      yp = p.Y * GLB_RndProjDist / -p.Z;

      pnts[i][j].x = xp * GLB_RndFrameW / GLB_RndWp + GLB_RndFrameW / 2;
      pnts[i][j].y = -yp * GLB_RndFrameH / GLB_RndHp + GLB_RndFrameH / 2;
    }
  }
  SetDCBrushColor(hDC, RGB(255, 0, 255));
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
  }*/
  /*for (i = 0; i < GRID_H - 1; i++)
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
    } */
} /* End of 'GLB_Draw' finction */

  /* END OF 'globe.c' FILE */
