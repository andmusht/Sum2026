#include "anim.h"

am6UNIT * Units[AM6_MAX_UNITS];
am6ANIM AM6_Anim;
INT NumOfUnits;

VOID AM6_AnimInit( HWND hWnd )
{
  memset(&AM6_Anim, 0, sizeof(am6ANIM));

  AM6_Anim.hWnd = hWnd;
  AM6_RndInit(hWnd);
  AM6_Anim.hDC = AM6_hRndDC;
  AM6_Anim.H = AM6_RndFrameH;
  AM6_Anim.W = AM6_RndFrameW;

  AM6_TimerInit();
  /*AM6_AnimInputInit();*/
}

VOID AM6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < AM6_Anim.NumOfUnits; i++)
  {
    AM6_Anim.Units[i]->Close(AM6_Anim.Units[i], &AM6_Anim);
    free(AM6_Anim.Units[i]);
    AM6_Anim.Units[i] = NULL;
  }
  AM6_RndClose();
  memset(&AM6_Anim, 0, sizeof(AM6_Anim));
}

VOID AM6_AnimResize( INT W, INT H )
{
  AM6_RndResize(W, H);
  AM6_Anim.W = W;
  AM6_Anim.W = W;
  AM6_AnimRender();
}

VOID AM6_AnimCopyFrame( VOID )
{
  AM6_RndCopyFrame();
}

VOID AM6_AnimRender( VOID )
{
  INT i;

  AM6_TimerResponse();
  /*AM6_AnimInputResponse();*/

  for (i = 0; i < AM6_Anim.NumOfUnits; i++)
    AM6_Anim.Units[i]->Response(AM6_Anim.Units[i], &AM6_Anim);

  AM6_RndStart();
  for (i = 0; i < AM6_Anim.NumOfUnits; i++)
    AM6_Anim.Units[i]->Render(AM6_Anim.Units[i], &AM6_Anim);
  AM6_RndEnd();
}

VOID AM6_AnimAddUnit( am6UNIT *Uni )
{
  if (AM6_Anim.NumOfUnits < AM6_MAX_UNITS)
    AM6_Anim.Units[AM6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &AM6_Anim);
}

VOID AM6_AnimExit( VOID )
{
}