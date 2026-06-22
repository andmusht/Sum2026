#include <stdio.h>
#include "units/units.h"

#define WND_CLASS_NAME "tung tung tung sahur"

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg,
                               WPARAM wparam,LPARAM lparam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInctance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;
  MATR m;
  INT i;  CONSOLE_FONT_INFOEX cfi = {0};
  HWND hConWnd;
 
  SetDbgMemHooks();

  /* Create console */
  AllocConsole();
 
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 18;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
 
  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
  printf("\x1b[38;2;%d;%d;%dm \x1b[48;2;%d;%d;%dm", 255, 255, 0, 0, 102, 102);
  printf("Группа компьютерной графики ФМЛ № 30\n");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 255, 0, 90, 90, 90);
  printf("Computer Graphics Support Group\n");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 255, 255, 255, 0, 0, 0);
  fflush(stdout);
 
  hConWnd = GetConsoleWindow();
  /* MoveWindow(hConWnd, 2560 + 1920 / 2, 0, 1920 / 2, 1080, FALSE); */
  //SetWindowPos(hConWnd, HWND_TOP,1920 / 2, 0, 1920 / 2, 1000, 0);

  m = MatrView(VecSet(0, 0, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));

  /* Windows Class Register */
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;
  wc.hInstance = hInstance;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "ERROR REGIST WINDOW CLASS", "ERROR", MB_OK);
    return 0;
  }

  /* Create Windows */
  hWnd = CreateWindow(WND_CLASS_NAME, "ANIM", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    1500, 100, 500, 300, NULL, NULL, hInstance, NULL);

  srand(30);
  
  AM6_AnimAddUnit(AM6_AnimUnitCreateControl());
  //AM6_AnimAddUnit(AM6_AnimUnitCreateTexture());
  AM6_AnimAddUnit(AM6_UnitCreateLand());
  AM6_AnimAddUnit(AM6_AnimUnitCreateHelic());
  /*for (i = 0; i < 10; i++)
    AM6_AnimAddUnit(AM6_AnimUnitCreateCow());*/
  /*for (i = 0; i < 10; i++)
    AM6_AnimAddUnit(AM6_AnimUnitCreateBBalls());*/

  /* Main Program Loop */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 50, 0);
  return 0;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT pt;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 50, 1, NULL);
    AM6_AnimInit(hWnd);
    return 0;

  case WM_SIZE:
    AM6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;

  case WM_TIMER:
    AM6_AnimRender();

    /*hDC = GetDC(hWnd);
    SelectObject(hDC, GetStockObject(BLACK_PEN));*/

    AM6_AnimCopyFrame();
    /*ReleaseDC(hWnd, hDC);*/
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &pt);
    EndPaint(hWnd, &pt);
    return 0;

  case WM_MOUSEWHEEL:
    AM6_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_ACTIVATE:
    AM6_Anim.IsActive = LOWORD(wParam) != WA_INACTIVE;
    return 0;

  case WM_ENTERSIZEMOVE:
    AM6_Anim.IsActive = FALSE;
    return 0;

  case WM_EXITSIZEMOVE:
    AM6_Anim.IsActive = TRUE;
    return 0;

  case WM_DESTROY:
    AM6_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);

  /*case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;*/
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' func */