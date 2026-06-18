#include "stdio.h"

#include "units.h"

typedef struct tagam6UNIT_CONTROL am6UNIT_CONTROL;

typedef struct tagam6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamAt, CamDir;
  DBL Speed, AngleSpeed;
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
  Uni->CamDir = VecSet(2, 2, 2);

  Uni->Speed = 10;
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

  if (Ani->KeysClick[VK_F8])
  {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD NumCharsWritten;
    COORD Pos = {0};
 
    GetConsoleScreenBufferInfo(hCon, &info);
    FillConsoleOutputCharacter(hCon, ' ',
      (UINT)(info.dwSize.X * info.dwSize.Y), Pos, &NumCharsWritten);  
    FillConsoleOutputAttribute(hCon, 0x0F,
      (UINT)(info.dwSize.X * info.dwSize.Y), Pos, &NumCharsWritten);  
    SetConsoleCursorPosition(hCon, info.dwCursorPosition);
    SetConsoleCursorPosition(hCon, Pos);
  }

  d = VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc));

  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->Mdz));

  Uni->CamLoc =
  PointTransform(Uni->CamLoc,
    MatrRotateY(Ani->JX *
                Uni->AngleSpeed * Ani->Mdx * 0.1));
  Uni->CamLoc =
  PointTransform(Uni->CamLoc,
    MatrRotateX(Ani->JY *
                Uni->AngleSpeed * Ani->Mdy));

  Uni->CamLoc =
  PointTransform(Uni->CamLoc,
    MatrRotateY(Ani->JZ *
                Uni->AngleSpeed * Ani->Mdy));

  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(d, Ani->GlobalDeltaTime * (Uni->Speed + 10 * Ani->Keys[VK_SHIFT]) *
      10 * (Ani->JR + Ani->Mdz)));

  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(d, Ani->GlobalDeltaTime * (Uni->Speed + 10 * Ani->Keys[VK_SHIFT]) *
      10 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] + Ani->Mdz)));

    Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Uni->AngleSpeed * Ani->Mdx));

    Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(-Ani->Keys[VK_RBUTTON] *
                   Uni->AngleSpeed * Ani->Mdx));

  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(AM6_RndMatrRight, Ani->Keys[VK_RIGHT] * (Uni->Speed + 3 * Ani->Keys[VK_SHIFT]) * 0.01));
  Uni->CamLoc = VecSubVec(Uni->CamLoc, VecMulNum(AM6_RndMatrRight, Ani->Keys[VK_LEFT] * (Uni->Speed + 3 * Ani->Keys[VK_SHIFT]) * 0.01));

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
  static DBL OldTime;

  if (Ani->GlobalTime - OldTime > 2)
  {
    sprintf(Buf, "FPS: %.3f, J: x %6.3f, but %i", Ani->FPS, Ani->JX, Ani->JBut[0]);
    SetWindowText(Ani->hWnd, Buf);
    OldTime = Ani->GlobalTime;
  }
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