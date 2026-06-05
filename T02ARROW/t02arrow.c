#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "tung tung tung sahur"

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, 
                               WPARAM wparam,LPARAM lparam );
VOID DrawArrow( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT W, INT H, INT S )
{
  POINT pt, pnts[4] = 
  {
    {0, 0}, {-H, W}, {S, 0}, {-H, -W}
  };
  INT mx, my, i, dx, dy, oldX;
  DOUBLE alpha, sine, cosine, d;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  mx = pt.x - Xc;
  my = pt.y - Yc;
  dx = mx ;
  dy = my;
  d = sqrt(dx * dx + dy * dy);
  sine = dy / d;
  cosine = dx / d;

  alpha = atan2(dy, dx);

  for (i = 0; i < 4; i++)
  {
    oldX = pnts[i].x;
    pnts[i].x = Xc + pnts[i].x * cos(alpha) - pnts[i].y * sin(alpha);
    pnts[i].y = Yc + oldX * sin(alpha) + pnts[i].y * cos(alpha);
  }
  Polygon(hDC, pnts, 4);
}

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
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create Windows */
  hWnd = CreateWindow(WND_CLASS_NAME, "Press me..", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  CreateWindow("BUTTON", "CLose", WS_CHILD | WS_VISIBLE,
    0, 0, 120, 60, hWnd, (HMENU)31, hInstance, NULL);
  CreateWindow("BUTTON", "normal mode", WS_CHILD | WS_VISIBLE,
    0, 70, 120, 60, hWnd, (HMENU)32, hInstance, NULL);
  CreateWindow("BUTTON", "colorfull mode", WS_CHILD | WS_VISIBLE,
    0, 140, 120, 60, hWnd, (HMENU)33, hInstance, NULL);
  CreateWindow("BUTTON", "random mode", WS_CHILD | WS_VISIBLE,
    0, 210, 120, 60, hWnd, (HMENU)34, hInstance, NULL);

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
  INT i, j;
  CHAR Buf[100];
  SYSTEMTIME time;
  static INT W, H, is_flag = 0;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_COMMAND:
    if (LOWORD(wParam) == 31)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    if (LOWORD(wParam) == 32)
      is_flag = 0;
    if (LOWORD(wParam) == 33)
      is_flag = 1;
    if (LOWORD(wParam) == 34)
      is_flag = 2;
    /* PostMessage(hWnd, WM_CLOSE, 0, 0); */
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

    if (hBm != NULL)
      DeleteObject(hBm);
    hDc = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDc, W, H);

    ReleaseDC(hWnd, hDc);
    return 0;

  case WM_CREATE:
    hDc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDc);
    ReleaseDC(hWnd, hDc);
    SetTimer(hWnd, 47, 30, NULL);
    break;

  case WM_TIMER:
    SelectObject(hMemDC, hBm);
    SelectObject(hMemDC, GetStockObject(LTGRAY_BRUSH));
    SelectObject(hMemDC, GetStockObject(BLACK_PEN));
    Rectangle(hMemDC, 0, H, W, 0);
    GetLocalTime(&time);
    TextOut(hMemDC, W / 13, H / 13, Buf, wsprintf(Buf, "%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond));

    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    Ellipse(hMemDC, p.x - 5, p.y - 5, p.x + 5, p.y + 5);

    SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));

    if (is_flag == 0)
    for (i = 0; i < W; i += W / 30)
      for (j = 0; j < H; j += H / 30)
        DrawArrow(hWnd, hMemDC, i, j, 13, 11, 19);

    if (is_flag == 1)
      for (i = 1; i < W; i += W / 30)
        for (j = 1; j < H; j += H / 30)
        {
          SelectObject(hMemDC, GetStockObject(DC_BRUSH));
          SelectObject(hMemDC, GetStockObject(DC_PEN));
          SetDCPenColor(hMemDC, RGB(rand() % i, rand() % j, rand() % (i + j)));
          SetDCBrushColor(hMemDC, RGB(rand() % i, rand() % j, rand() % (i + j)));
          DrawArrow(hWnd, hMemDC, rand() % i, rand() % j, 13, 11, 19);
        }

    srand(30);
    if (is_flag == 2)
      for (i = 1; i < W; i += W / 30)
        for (j = 1; j < H; j += H / 30)
          DrawArrow(hWnd, hMemDC, rand() % i, rand() % j, 13, 11, 19);
    
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' func */
