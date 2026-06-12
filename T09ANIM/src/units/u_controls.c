#include "stdio.h"

#include "units.h"

typedef struct tagam6UNIT_CONTROL am6UNIT_CONTROL;

typedef struct tagam6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamAt;
  DBL Speed;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitInit( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
  Uni->CamLoc = VecSet(8, 8, 8);
  Uni->CamAt = VecSet(3, 3, 0);

  Uni->Speed = 10;
} /* End of 'AM6_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitResponse( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
  VEC d;

  /*
  if (Ani->Keys[VK_CONTROL] && Ani->Keys['F'])
    AM6_AnimFlipFullScreen();
  */

  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  d = VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc));

  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(d, Ani->GlobalDeltaTime * (Uni->Speed + 3 * Ani->Keys[VK_SHIFT]) *
      (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] + Ani->Mdz)));

  AM6_RndCamSet(Uni->CamLoc, Uni->CamAt, VecSet(0, 1, 0));

} /* End of 'AM6_UnitClose' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       am6UNIT *Uni;
 *   - animation context:
 *       am6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AM6_UnitRender( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
  CHAR Buf[102];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(0, 0, 0));
  TextOut(Ani->hDC, Ani->W - 200, 30, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));

} /* End of 'AM6_UnitClose' function */

static VOID AM6_UnitClose( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
} /* End of 'AM6_UnitClose' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateControl( VOID )
{
  am6UNIT_CONTROL *Uni;

  /* Memory allocation */
  if ((Uni = (am6UNIT_CONTROL *)AM6_AnimUnitCreate(sizeof(am6UNIT_CONTROL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;

  return (am6UNIT *)Uni;
} /* End of 'AM6_AnimUnitCreate' function */