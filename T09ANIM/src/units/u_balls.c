#include "units.h"

typedef struct tagam6UNIT_BBALL am6UNIT_BBALL;

typedef struct tagam6UNIT_BBALL
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  am6PRIM Ball;
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
static VOID AM6_UnitInit( am6UNIT_BBALL *Uni, am6ANIM *Ani )
{
  AM6_RndPrimCreateSphere(&Uni->Ball, 1, 18, 20);
  Uni->Pos = VecSet(Rnd1() * 8, 1, Rnd1() * 8);
 
  Uni->Shift = 1 + Rnd0() * 47;
  Uni->Scale = 3 + Rnd1() * 0.5;
} /* End of 'AM6_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT_BBALL *Uni, am6ANIM *Ani )
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
static VOID AM6_UnitRender( am6UNIT_BBALL *Uni, am6ANIM *Ani )
{
  AM6_RndPrimDraw(&Uni->Ball, MatrTranslate(VecAddVec(Uni->Pos, VecSet(0, fabs(sin(5 * Ani->Time) * Uni->Scale), 0))));
} /* End of 'AM6_UnitClose' function */

static VOID AM6_UnitClose( am6UNIT_BBALL *Uni, am6ANIM *Ani )
{
  AM6_RndPrimFree(&Uni->Ball);
} /* End of 'AM6_UnitClose' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateBBalls( VOID )
{
  am6UNIT_BBALL *Uni;

  /* Memory allocation */
  if ((Uni = (am6UNIT_BBALL *)AM6_AnimUnitCreate(sizeof(am6UNIT_BBALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;

  return (am6UNIT *)Uni;
} /* End of 'AM6_AnimUnitCreate' function */