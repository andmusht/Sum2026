#include <stdio.h>
#include "units.h"

typedef struct tagam6UNIT_CONTROL am6UNIT_CONTROL;

typedef struct tagam6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamAt;
  DBL Speed;
};
 
static VOID AM6_UnitInit( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
  Uni->CamLoc = VecSet(8, 8, 8);
  Uni->CamAt = VecSet(0, 0, 0);
  Uni->Speed = 1;
}
 
static VOID AM6_UnitClose( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
}
 
static VOID AM6_UnitResponse( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
  FLT Wp, Hp, sx, sy;
  DBL CosT, SinT, CosP, SinP, 
      Dist, plen, Azimuth, Elevator;
  VEC NewLoc, dv;
  INT mode[2];

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

  Dist = VecLen(VecSubVec(AM6_RndCamAt, AM6_RndCamLoc));
  
  if (Dist < 0.001)
    Dist = 0.001;

  CosT = (AM6_RndCamLoc.Y - AM6_RndCamAt.Y) / Dist;
  
  if (CosT > 1.0)
    CosT = 1.0;
  else if (CosT < -1.0)
    CosT = -1.0;
    
  SinT = sqrt(1 - CosT * CosT);

  plen = Dist * SinT;
  if (plen < 0.001)
    plen = 0.001;
    
  CosP = (AM6_RndCamLoc.Z - AM6_RndCamAt.Z) / plen;
  SinP = (AM6_RndCamLoc.X - AM6_RndCamAt.X) / plen;

  Azimuth = R2D(atan2(SinP, CosP));
  Elevator = R2D(atan2(SinT, CosT)); 
  
  Azimuth += 
    Ani->GlobalDeltaTime * Uni->Speed * 47 * 
      (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]) +
    Ani->Keys[VK_LBUTTON] * 0.01 * -30 * Ani->Mdx;

  Elevator += 
    Ani->GlobalDeltaTime * Uni->Speed * 47 * 
      (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) +
    Ani->Keys[VK_LBUTTON] * 0.01 * -30 * Ani->Mdy;

  if (Elevator < 0.08)
    Elevator = 0.08;
  if (Elevator > 178.0)
    Elevator = 178.0;

  Dist += 
    Ani->Mdz * 0.01 +
    Ani->GlobalDeltaTime * 8 * 
      (1 + Ani->Keys[VK_SHIFT] * 30) *  
      (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]);
  
  if (Dist < 0.001)
    Dist = 0.001;

  Wp = AM6_RndProjSize;
  Hp = AM6_RndProjSize;
  
  if (Ani->W > Ani->H)
    Wp *= (FLT)Ani->W / Ani->H;
  else
    Hp *= (FLT)Ani->H / Ani->W;

  sx = Ani->Keys[VK_RBUTTON] * -Ani->Mdx * Wp / Ani->W * Dist / AM6_RndProjDist;
  sy = Ani->Keys[VK_RBUTTON] * Ani->Mdy * Hp / Ani->H * Dist / AM6_RndProjDist;

  dv = VecAddVec(VecMulNum(AM6_RndCamRight, sx),
                 VecMulNum(AM6_RndCamUp, sy));
  AM6_RndCamAt = VecAddVec(AM6_RndCamAt, dv);
  AM6_RndCamLoc = VecAddVec(AM6_RndCamLoc, dv);

  
  NewLoc = PointTransform(VecSet(0, Dist, 0),
                          MatrMulMatr(MatrRotateX(Elevator),
                                      MatrRotateY(Azimuth)));

  AM6_RndCamSet(
    PointTransform(VecSet(0, Dist, 0),
                   MatrMulMatr3(MatrRotateX(Elevator),
                                MatrRotateY(Azimuth),
                                MatrTranslate(AM6_RndCamAt))),
    AM6_RndCamAt,
    VecSet(0, 1, 0)
  );
}
 
static VOID AM6_UnitRender( am6UNIT_CONTROL *Uni, am6ANIM *Ani )
{
  CHAR buf[102];

  sprintf(buf, "FPS: %.3f  Dist: %.2f", Ani->FPS, 
      VecLen(VecSubVec(AM6_RndCamAt, AM6_RndCamLoc)));
  AM6_RndFntDraw(buf, VecSet1(10), 10);
}
 
am6UNIT * AM6_AnimUnitCreateControl( VOID )
{
  am6UNIT *Uni;
 
  if ((Uni = AM6_AnimUnitCreate(sizeof(am6UNIT_CONTROL))) == NULL)
    return NULL;
 
  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Close = (VOID *)AM6_UnitClose;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;
  return Uni;
}