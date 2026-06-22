#include "units.h"

typedef struct tagam6UNIT_G3DM am6UNIT_G3DM;

typedef struct tagam6UNIT_G3DM
{
  UNIT_BASE_FIELDS;
  am6PRIMS Model;
  VEC Pos;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tagam6UNIT_G3DM *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT_G3DM *Uni, am6ANIM *Ani )
{
  AM6_RndPrimsLoad(&Uni->Model, "bin/models/x6.g3dm");
  //Uni->Model.Trans = MatrRotateX(-90);
} /* End of 'AM6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tagam6UNIT_G3DM *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitClose( am6UNIT_G3DM *Uni, am6ANIM *Ani )
{
  AM6_RndPrimsFree(&Uni->Model);
} /* End of 'AM6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT_G3DM *Uni, am6ANIM *Ani )
{
} /* End of 'AM6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitRender( am6UNIT_G3DM *Uni, am6ANIM *Ani )
{
  AM6_RndPrimsDraw(&Uni->Model, MatrIdentity());
} /* End of 'AM6_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateG3DM( VOID )
{
  am6UNIT_G3DM *Uni;

  /* Memory allocation */
  if ((Uni = (am6UNIT_G3DM *)AM6_AnimUnitCreate(sizeof(am6UNIT_G3DM))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;
  Uni->Close = (VOID *)AM6_UnitClose;

  return (am6UNIT *)Uni;
} /* End of 'AM6_AnimUnitCreate' function */