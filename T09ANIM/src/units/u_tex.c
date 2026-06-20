#include "units.h"

typedef struct tagam6UNIT_TEX am6UNIT_TEX;

typedef struct tagam6UNIT_TEX
{
  UNIT_BASE_FIELDS;
  am6PRIM Pr;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  am6VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 1, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
  };
 
  FLT t[2][2] =
  {
    {0.8, 1},
    {1, 0.3}
  };
  /*am6MATERIAL mtl = AM6_RndMtlGetDef();

  mtl.ShdNo = AM6_RndShdAdd("tex");*/
  
  /*glGenTextures(1, &Uni->TexId);
  glBindTexture(GL_TEXTURE_2D, Uni->TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, 1, 2, 2, 0, GL_LUMINANCE, GL_FLOAT, t);*/
} /* End of 'AM6_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  //Uni->Pos = VecSubNum(Uni->Pos, Ani->DeltaTime * 2.4);
} /* End of 'AM6_UnitClose' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitRender( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  //AM6_RndPrimDraw(&Uni->Cow, MatrMulMatr(MatrRotateY(Ani->Time * 100), MatrTranslate(VecAddVec(Uni->Pos, VecSet(-10, fabs(sin(5 * Ani->Time)), 0)))));
} /* End of 'AM6_UnitClose' function */

static VOID AM6_UnitClose( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  AM6_RndPrimFree(&Uni->Pr);
} /* End of 'AM6_UnitClose' function */

/* Unit creation function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateTex( VOID )
{
  am6UNIT_TEX *Uni;

  /* Memory allocation */
  if ((Uni = (am6UNIT_TEX *)AM6_AnimUnitCreate(sizeof(am6UNIT_TEX))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;
  Uni->Close = (VOID *)AM6_UnitClose;

  return (am6UNIT *)Uni;
} /* End of 'AM6_AnimUnitCreate' function */