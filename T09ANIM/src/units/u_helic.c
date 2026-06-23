#include <stdio.h>
#include "units.h"

typedef struct tagam6UNIT_G3DM am6UNIT_G3DM;

typedef struct tagam6UNIT_G3DM
{
  UNIT_BASE_FIELDS;
  am6PRIMS Model;
  VEC Center;
  FLT Dist;
  FLT AngX, AngY;
  FLT VelX, VelY;
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
  AM6_RndPrimsLoad(&Uni->Model, "bin/models/Apache.g3dm");
  Uni->Model.Trans = MatrRotateX(-90);
  Uni->Center = VecSet1(0);
  Uni->Dist = 100.0;
  Uni->AngX = 15.0;
  Uni->AngY = 0;
  Uni->VelX = Uni->VelY = 0;
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
  FLT radX, radY, coRadX, coRadY, siRadX, siRadY;
  VEC camPos, dir;
  DBL moveSpeed = 50.0 * Ani->DeltaTime;
  CHAR buf[100];
  INT mode[2];

  if (Ani->Keys['0'])
      AM6_RndCamSet(VecSet(5, -5, 5), VecSet(-1, 20, -8), VecSet(0, 1, 0));  
  if (Ani->Keys['F'] == 1 && Ani->KeysClick['F'] == 1)
    FlipFullScreen(Ani->hWnd);
  /* Clear console on F8 */
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
  
  /* Pause toggle */
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Exit */
  if (Ani->KeysClick[VK_ESCAPE])
    AM6_AnimExit();

  if (Ani->Keys[VK_SHIFT] == 1 && Ani->KeysClick['V'] == 1)
  {
    glGetIntegerv(GL_POLYGON_MODE, mode);

    if (mode[0] == GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  if (Ani->KeysClick['1'])
    glLineWidth(1);
  if (Ani->KeysClick['2'])
    glLineWidth(2);
  if (Ani->KeysClick['3'])
    glLineWidth(3);

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->AngX += Ani->Mdy * 0.3;
    Uni->AngY += Ani->Mdx * 0.3;

    Uni->VelY = Ani->Mdx * 0.5;
    Uni->VelX = Ani->Mdy * 0.5;
  }
  else
  {
    Uni->AngX += Uni->VelY;
    Uni->AngY += Uni->VelX;

    Uni->VelX *= 0.05;
    Uni->VelY *= 0.05;
  }

  if (Uni->AngX > 89.0)
    Uni->AngX = 89.0;
  if (Uni->AngX < -89.0)
    Uni->AngX = -89.0;

  Uni->Dist -= Ani->Mdz * 0.05;
  if (Uni->Dist < 1.0)
    Uni->Dist = 1.0;

  radX = D2R(Uni->AngX);
  radY = D2R(Uni->AngY);

  coRadX = cos(radX), coRadY = cos(radY), siRadX = sin(radX), siRadY = sin(radY);
  dir.X = coRadX * siRadY;
  dir.Y = siRadX;
  dir.Z = coRadX * coRadY;

  if (Ani->Keys['S'])
    Uni->Center = VecAddVec(Uni->Center, VecMulNum(dir, moveSpeed));
  if (Ani->Keys['W'])
    Uni->Center = VecSubVec(Uni->Center, VecMulNum(dir, moveSpeed));
  if (Ani->Keys['A'])
    Uni->Center = VecSubVec(Uni->Center, VecMulNum(AM6_RndCamRight, moveSpeed));
  if (Ani->Keys['D'])
    Uni->Center = VecAddVec(Uni->Center, VecMulNum(AM6_RndCamRight, moveSpeed));

  camPos.X = Uni->Center.X + Uni->Dist * coRadX * siRadY;
  camPos.Y = Uni->Center.Y + Uni->Dist * siRadX;
  camPos.Z = Uni->Center.Z + Uni->Dist * coRadX * coRadY;

  AM6_RndCamSet(camPos, Uni->Center, VecSet(0, 1, 0));

  //Uni->Model.Trans = MatrMulMatr3(MatrRotateX(-90 - Uni->AngX), MatrRotateY(Uni->AngY), MatrTranslate(Uni->Center));
  Uni->Model.Trans = MatrMulMatr(MatrRotateX(-90), MatrTranslate(Uni->Center));
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
  CHAR buf[102];

  sprintf(buf, "FPS: %.3f  Dist: %.2f", Ani->FPS, VecLen(VecSubVec(AM6_RndCamAt, AM6_RndCamLoc)));
  AM6_RndFntDraw(buf, VecSet1(10), 10);
  AM6_RndPrimsDraw(&Uni->Model, MatrIdentity());
} /* End of 'AM6_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (am6UNIT *) pointer to created unit.
 */
am6UNIT * AM6_AnimUnitCreateHelic( VOID )
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