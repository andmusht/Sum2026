#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT *Uni, am6ANIM *Ani )
{
} /* End of 'AM6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitClose( am6UNIT *Uni, am6ANIM *Ani )
{
} /* End of 'AM6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT *Uni, am6ANIM *Ani )
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
static VOID AM6_UnitRender( am6UNIT *Uni, am6ANIM *Ani )
{
} /* End of 'AM6_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreate( INT Size )
{
  am6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(am6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = AM6_UnitInit;
  Uni->Close = AM6_UnitClose;
  Uni->Response = AM6_UnitResponse;
  Uni->Render = AM6_UnitRender;

  return Uni;
} /* End of 'AM6_AnimUnitCreate' function */