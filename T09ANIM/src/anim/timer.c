#include "anim.h"

static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

VOID AM6_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;

  QueryPerformanceCounter(&t);
  StartTime = t.QuadPart;

  PauseTime = 0;
  FrameCounter = 0;
  AM6_Anim.IsPause = FALSE;
  AM6_Anim.Time = AM6_Anim.DeltaTime = 0;
  AM6_Anim.FPS = 30;
}

VOID AM6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);

  /* Global time */
  AM6_Anim.GlobalTime = (DOUBLE)(t.QuadPart - StartTime) / TimePerSec;
  AM6_Anim.GlobalDeltaTime = (DOUBLE)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (!AM6_Anim.IsPause)
  {
    AM6_Anim.Time = (DOUBLE)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
    AM6_Anim.DeltaTime = AM6_Anim.GlobalDeltaTime;
  }
  else
  {
    AM6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }

  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > 3 * TimePerSec)
  {
    AM6_Anim.FPS = FrameCounter * TimePerSec / (DOUBLE)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}