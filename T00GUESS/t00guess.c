/* Mushtakov Andrey, 10-6, 01.06.2026 */

#include <windows.h>
#include <stdio.h>
void main( void )
{
  int a = 0, b = 100, m, ans;
  char buf[100];

  MessageBox(NULL, "chose number from 0 to 100\n OK - more, NO - less, cancel - equal", "T00GUESS", MB_OK | MB_ICONINFORMATION);
  while (a <= b)
  {
    m = (a + b) / 2;
    sprintf(buf, "Is your number more than %i", m); 
    ans = MessageBox(NULL, buf, "T00GUESS", MB_YESNOCANCEL | MB_ICONQUESTION);
    if (ans == IDYES)
      a = m + 1;
    else if (ans == IDNO)
      b = m - 1;
    else
      break;
  }

  sprintf(buf, "Is your number %i", m); 
  if (MessageBox(NULL, buf, "T00GUESS", MB_YESNO | MB_ICONQUESTION) == IDNO || a > b)
    MessageBox(NULL, "You nasty liar", "T00GUESS", MB_OK | MB_ICONERROR);
  else
    MessageBox(NULL, "You are not a liar. I guess...", "T00GUESS", MB_OK | MB_ICONASTERISK);
}