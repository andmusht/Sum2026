#include "rnd.h"

HWND AM6_hRndWnd;                 /* Work window handle */
HDC AM6_hRndDC;                   /* Work window memory device context  */
/*HBITMAP AM6_hRndBmFrame;           Work window background bitmap handle */
HGLRC AM6_hRndGLRC;
INT AM6_RndFrameW, AM6_RndFrameH; /* Work window size */

DBL
   AM6_RndProjSize = 0.1,         /* Project plane fit square */
   AM6_RndProjDist = 0.1,         /* Distance to project plane from viewer (near) */
   AM6_RndProjFarClip = 3000;      /* Distance to project far clip plane (far) */

MATR
    AM6_RndMatrView,              /* View coordinate system matrix */
    AM6_RndMatrProj,              /* Projection coordinate system matrix */
    AM6_RndMatrVP;                /* Stored (View * Proj) matrix */

VEC AM6_RndMatrRight;

INT AM6_MouseWheel;
VEC
  AM6_RndCamLoc,
  AM6_RndCamAt,
  AM6_RndCamDir,
  AM6_RndCamUp;

INT AM6_RndShdAddonI[8];
INT AM6_RndShdAddonF[8];
INT AM6_RndShdAddonV[8];