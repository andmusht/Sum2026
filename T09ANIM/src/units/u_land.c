#include "units.h"

/* Structure of Land unit */
typedef struct tagam6UNIT_LAND am6UNIT_LAND;
typedef struct tagam6UNIT_LAND
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  am6PRIM Land;     /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
  DBL Size;         /* Size of Land */
  am6GRID G;
} am6UNIT_LAND;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT_LAND *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
  HBITMAP hBm;
  BITMAP bm;
  INT w, h, x, y;

  Uni->Size = 1000;

  if ((hBm = LoadImage(NULL, "bin/heights/hfcolor1.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if (bm.bmBitsPixel == 8 && AM6_RndGridCreate(&Uni->G, w, h))
    {
      BYTE *Bits = bm.bmBits;

      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x];

          Uni->G.V[y * w + x].P = VecSet(x / (w - 1.0),
                                    hgt / 255.0,
                                    1 - y / (h - 1.0));
        }
      AM6_RndGridAutoNormals(&Uni->G);
      AM6_RndPrimFromGrid(&Uni->Land, &Uni->G);
    }                                  
  }
  Uni->Pos = VecSet1(0);
} /* End of 'AM6_UnitInit' function */ 


/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT_LAND *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitRender( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
  AM6_RndPrimDraw(&Uni->Land, MatrMulMatr(MatrTranslate(Uni->Pos),
    MatrScale(VecSet1(Uni->Size))));
} /* End of 'AM6_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:                            
 *   - self-pointer to unit object:
 *       am6UNIT_LAND *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitClose( am6UNIT_LAND *Uni, am6ANIM *Ani )
{
  AM6_RndGridFree(&Uni->G);
  AM6_RndPrimFree(&Uni->Land);
} /* End of 'AM6_UnitClose' function */ 

/* Unit Land creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_UnitCreateLand( VOID )
{
  am6UNIT_LAND *Uni;

  if ((Uni = (am6UNIT_LAND *)AM6_AnimUnitCreate(sizeof(am6UNIT_LAND))) == NULL)
    return NULL;

  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Render = (VOID *)AM6_UnitRender;
  Uni->Close = (VOID *)AM6_UnitClose;
  return (am6UNIT *)Uni;
} /* End of 'AM6_UnitCreateLand' function */ 

/* END OF 'u_land.c' FILE */