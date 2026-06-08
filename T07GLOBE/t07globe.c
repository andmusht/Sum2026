#include "globe.h"

#define WND_CLASS_NAME "tung tung tung sahuhr"

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, 
                               WPARAM wparam,LPARAM lparam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInctance,
                   CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  /* Windows Class Register */
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;
  wc.hInstance = hInstance;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Create Windows */
  hWnd = CreateWindow(WND_CLASS_NAME, "Globe", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  CreateWindow("BUTTON", "Close", WS_CHILD | WS_VISIBLE,
    0, 0, 120, 60, hWnd, (HMENU)31, hInstance, NULL);

  /* Main Program Loop */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDc;
  PAINTSTRUCT pt;
  POINT p;
  CHAR Buf[100];
  SYSTEMTIME time;
  static INT W, H, is_flag = 0;
  static HDC hMemDC;
  static BITMAP bm;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_COMMAND:
    if (LOWORD(wParam) == 31)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure?", "EXIT", MB_YESNO) == IDNO)
      return 0;
    break;

  case WM_ERASEBKGND:
    return 0;

  case WM_PAINT:
    hDc = BeginPaint(hWnd, &pt);
    BitBlt(hDc, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &pt);
    return 0;

  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    GLB_Resize(W, H);

    if (hBm != NULL)
      DeleteObject(hBm);
    hDc = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDc, W, H);

    ReleaseDC(hWnd, hDc);
    return 0;

  case WM_CREATE:
    hDc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDc);
    GLB_Init(0.3);
    GetObject(hBmClockface, sizeof(BITMAP), &bm);
    ReleaseDC(hWnd, hDc);
    SetTimer(hWnd, 3, 8, NULL);
    break;

  case WM_TIMER:
    SelectObject(hMemDC, hBm);
    SelectObject(hDCClockface, hBmClockface);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(DC_PEN));

    GetLocalTime(&time);
    TextOut(hMemDC, W / 7, H / 7, Buf, wsprintf(Buf, "%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond));

   GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    Ellipse(hMemDC, p.x - 7, p.y - 7, p.x + 7, p.y + 7);

    InvalidateRect(hWnd, NULL, FALSE);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 3);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' func */
