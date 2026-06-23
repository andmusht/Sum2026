#include "rnd.h"

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

VOID AM6_RndInit( HWND hWnd )
{
  INT i, i1[10];
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };
 
  AM6_hRndWnd = hWnd;
 
  /* Prepare frame compatible device contesxt */
  AM6_hRndDC = GetDC(AM6_hRndWnd);
 
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AM6_hRndDC, &pfd);
 
  DescribePixelFormat(AM6_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AM6_hRndDC, i, &pfd);
 
  /* OpenGL init: rendering context setup */
  AM6_hRndGLRC = wglCreateContext(AM6_hRndDC);
  wglMakeCurrent(AM6_hRndDC, AM6_hRndGLRC);

  if (glewInit() != GLEW_OK)
    exit(0);

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(AM6_hRndDC, PixelAttribs, NULL, 1, i1, &i);
  hRC = wglCreateContextAttribsARB(AM6_hRndDC, NULL, ContextAttribs);

  if (hRC != NULL)
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AM6_hRndGLRC);
    AM6_hRndGLRC = hRC;
    wglMakeCurrent(AM6_hRndDC, AM6_hRndGLRC);
  }

#ifndef NDEBUG
  OutputDebugString(glGetString(GL_VERSION));
  OutputDebugString("\n");
  OutputDebugString(glGetString(GL_VENDOR));
  OutputDebugString("\n");
  OutputDebugString(glGetString(GL_RENDERER));
  OutputDebugString("\n");

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, NULL);
#endif /* NDEBUG */

  /* Render parameters setup */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  wglSwapIntervalEXT(0);
  AM6_RndProjSize = 0.1;
  AM6_RndProjDist = AM6_RndProjSize;
  AM6_RndProjFarClip = 30000;
  AM6_RndFrameW = 47;
  AM6_RndFrameH = 47;
  AM6_RndCamSet(VecSet(17, 17, 17), VecSet(1, -2, 8), VecSet(0, 1, 0));

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  AM6_RndResInit();
}

VOID AM6_RndClose( VOID )
{
  AM6_RndResClose();

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AM6_hRndGLRC);
  ReleaseDC(AM6_hRndWnd, AM6_hRndDC);
}

VOID AM6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);
 
  /* Setup projection */
  AM6_RndFrameW = W;
  AM6_RndFrameH = H;
  AM6_RndProjSet();
}

VOID AM6_RndCopyFrame( VOID )
{
  SwapBuffers(AM6_hRndDC);
}

VOID AM6_RndStart( VOID )
{
  VEC4 ClearColor = {0.30, 0.47, 0.8, 1};
  FLT DepthClearValue = 1;

  AM6_RndShdUpdate();

  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, &ClearColor.X);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
}

VOID AM6_RndEnd( VOID )
{
  glFinish();
}

VOID AM6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = AM6_RndProjSize;
  /* Correct aspect ratio */
  if (AM6_RndFrameW > AM6_RndFrameH)
    rx *= (DBL)AM6_RndFrameW / AM6_RndFrameH;
  else
    ry *= (DBL)AM6_RndFrameH / AM6_RndFrameW;
  AM6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      AM6_RndProjDist, AM6_RndProjFarClip);
  AM6_RndMatrVP = MatrMulMatr(AM6_RndMatrView, AM6_RndMatrProj);
}

VOID AM6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  AM6_RndMatrView = MatrView(Loc, At, Up);
  AM6_RndCamRight = CamRight(Loc, At, Up);
  AM6_RndMatrVP = MatrMulMatr(AM6_RndMatrView, AM6_RndMatrProj);
  AM6_RndCamDir = VecSet(-AM6_RndMatrView.A[0][2],
                         -AM6_RndMatrView.A[1][2],
                         -AM6_RndMatrView.A[2][2]);
  AM6_RndCamAt = At;
  AM6_RndCamLoc = Loc;
  AM6_RndCamUp = Up;

}


/* Flip window full screen mode function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */   
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRect);

    /* Obtain nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);

    GetMonitorInfo(hmon, &mi);

    /* Go to full screen mode */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);


    /* Expand window */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
    /* Restore from full screen mode */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */