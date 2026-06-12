#ifndef __timer_h_
#define __timer_h_

#include <time.h>

#include "rnd/rnd.h"

extern DOUBLE
           AM6_GlobalTime, AM6_GlobalDeltaTime, /* Global time and interframe interval */
           AM6_Time, AM6_DeltaTime,             /* Time with pause and interframe interval */
           AM6_FPS;                             /* Frames per second value */
extern BOOL
           AM6_IsPause;                         /* Pause flag */

VOID AM6_TimerInit( VOID );

VOID AM6_TimerResponse( VOID );

#endif /* __globe_h_ */