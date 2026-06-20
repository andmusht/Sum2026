#include "units.h"

typedef struct tagam6UNIT_COW am6UNIT_COW;

typedef struct tagam6UNIT_COW
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  am6PRIM Cow;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT_COW *Uni, am6ANIM *Ani )
{
  if (AM6_RndPrimLoad(&Uni->Cow, "bin/models/cow.obj"))
    Uni->Pos = VecSet(Rnd1() * -13, -13, Rnd1() * -13);
} /* End of 'AM6_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT_COW *Uni, am6ANIM *Ani )
{
  Uni->Pos = VecSubNum(Uni->Pos, Ani->DeltaTime * 2.4);
} /* End of 'AM6_UnitClose' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitRender( am6UNIT_COW *Uni, am6ANIM *Ani )
{
  AM6_RndPrimDraw(&Uni->Cow, MatrMulMatr(MatrRotateY(Ani->Time * 100), MatrTranslate(VecAddVec(Uni->Pos, VecSet(-10, fabs(sin(5 * Ani->Time)), 0)))));
} /* End of 'AM6_UnitClose' function */

static VOID AM6_UnitClose( am6UNIT_COW *Uni, am6ANIM *Ani )
{
  AM6_RndPrimFree(&Uni->Cow);
} /* End of 'AM6_UnitClose' function */

/* Unit creation function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateCow( VOID )
{
  am6UNIT_COW *Uni;

  /* Memory allocation */
  if ((Uni = (am6UNIT_COW *)AM6_AnimUnitCreate(sizeof(am6UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;
  Uni->Close = (VOID *)AM6_UnitClose;

  return (am6UNIT *)Uni;
} /* End of 'AM6_AnimUnitCreate' function */