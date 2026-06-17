#include "anim.h"

#include <mmsystem.h>

#pragma comment(lib, "winmm")

#define AM6_GET_JOYSTIC_AXIS(A) \
 2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1

static BYTE OldKeys[256];
static BYTE JButOld[256];

static VOID KeyboardInit( VOID )
{
  INT i;

  GetKeyboardState(AM6_Anim.Keys);
  for (i = 0; i < 256; i++)
    AM6_Anim.Keys[i] >>= 7;
  memcpy(OldKeys, AM6_Anim.Keys, 256);
}

static VOID KeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(AM6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AM6_Anim.Keys[i] >>= 7;
    AM6_Anim.KeysClick[i] = AM6_Anim.Keys[i] && !OldKeys[i];
  }

  memcpy(OldKeys, AM6_Anim.Keys, 256);
} 

static VOID MouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(AM6_Anim.hWnd, &pt);

  AM6_Anim.Mx = pt.x;
  AM6_Anim.My = pt.y;

  AM6_MouseWheel = AM6_Anim.Mdz = AM6_Anim.Mz = 0;
}

static VOID MouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(AM6_Anim.hWnd, &pt);

  AM6_Anim.Mdx = pt.x - AM6_Anim.Mx;
  AM6_Anim.Mdy = pt.y - AM6_Anim.My;

  AM6_Anim.Mx = pt.x;
  AM6_Anim.My =  pt.y;

  AM6_Anim.Mdz = AM6_MouseWheel;
  AM6_Anim.Mz += AM6_MouseWheel;
  AM6_MouseWheel = 0;
}

static VOID JoystickResponse( VOID )
{
   INT i;

  /* Joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;
 
    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;
 
 
      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          AM6_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          AM6_Anim.JButClick[i] = AM6_Anim.JBut[i] && !JButOld[i];
          JButOld[i] = AM6_Anim.JBut[i];
        }
        /* Axes */
        AM6_Anim.JX = AM6_GET_JOYSTIC_AXIS(X);
        AM6_Anim.JY = AM6_GET_JOYSTIC_AXIS(Y);
        AM6_Anim.JZ = AM6_GET_JOYSTIC_AXIS(Z);
        AM6_Anim.JR = AM6_GET_JOYSTIC_AXIS(R);
        AM6_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}

static VOID JoystickInit( VOID )
{
  JoystickResponse();
}

VOID AM6_AnimInputInit( VOID )
{
  KeyboardInit();
  MouseInit();
  JoystickInit();
}

VOID AM6_AnimInputResponse( VOID )
{
  KeyboardResponse();
  MouseResponse();
  JoystickResponse();
}