#include "units.h"

typedef struct tagam6UNIT_LAND am6UNIT_LAND;

typedef struct tagam6UNIT_LAND
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  am6PRIM Land;
  DBL Shift, Scale;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
  HBITMAP hBm;
  BITMAP bm;

  Uni->Pos = VecSet(0, -64, 0);
  if ((hBm = LoadImage(NULL, "bin/heights/hfcolor1.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    INT w, h;
    am6GRID G;

    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if ((bm.bmBitsPixel == 8) && AM6_RndGridCreate(&G, w, h))
    {
      BYTE *Bits = bm.bmBits;
      INT x, y;
      FLT size = 1000;
 
      for (y = 0; y < h; y++)
      {
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x * (bm.bmBitsPixel >> 3)];
 
          G.V[y * G.W + x].P = VecSet((2 * x / (w - 1.0) - 1) * size,
                                    30 * hgt / 255.0,
                                    (1 - (2 * y / (h - 1.0) - 1) * size));
        }
      }
      AM6_RndGridAutoNormals(&G);
      AM6_RndPrimFromGrid(&Uni->Land, &G);
      AM6_RndGridFree(&G);
    }
  }
} /* End of 'AM6_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
} /* End of 'AM6_UnitClose' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitRender( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
  AM6_RndPrimDraw(&Uni->Land, MatrTranslate(Uni->Pos));
} /* End of 'AM6_UnitClose' function */

static VOID AM6_UnitClose( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
  AM6_RndPrimFree(&Uni->Land);
} /* End of 'AM6_UnitClose' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateLand( VOID )
{
  am6UNIT_LAND *Uni;

  /* Memory allocation */
  if ((Uni = (am6UNIT_LAND *)AM6_AnimUnitCreate(sizeof(am6UNIT_LAND))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;

  return (am6UNIT *)Uni;
} /* End of 'AM6_AnimUnitCreate' function */