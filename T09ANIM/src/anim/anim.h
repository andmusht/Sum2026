#ifndef __anim_h_
#define __anim_h_

#include "anim/rnd/rnd.h"

typedef struct tagam6ANIM am6ANIM;
typedef struct tagam6UNIT am6UNIT;

#define UNIT_BASE_FIELDS \
  VOID (*Init)( am6UNIT *Uni, am6ANIM *Ani );      \
  VOID (*Close)( am6UNIT *Uni, am6ANIM *Ani );     \
  VOID (*Response)( am6UNIT *Uni, am6ANIM *Ani );  \
  VOID (*Render)( am6UNIT *Uni, am6ANIM *Ani )

#define AM6_MAX_UNITS 3000

extern am6UNIT * Units[AM6_MAX_UNITS];
extern am6ANIM AM6_Anim;
extern INT NumOfUnits;

typedef struct tagam6UNIT
{
  UNIT_BASE_FIELDS;
};

typedef struct tagam6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  DBL Shift, Scale;

  am6UNIT * Units[AM6_MAX_UNITS];
  INT NumOfUnits;

  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */

  BOOL
    IsPause,                     /* Pause flag */
    IsActive;

  BYTE Keys[256];
  BYTE KeysClick[256];
                          
  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  BYTE
    JBut[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */
};

extern am6ANIM AM6_Anim;

VOID AM6_AnimInit( HWND hWnd );

VOID AM6_AnimClose( VOID );

VOID AM6_AnimResize( INT W, INT H );

VOID AM6_AnimCopyFrame( VOID );

VOID AM6_AnimRender( VOID );

VOID AM6_AnimAddUnit( am6UNIT *Uni );

VOID AM6_AnimExit( VOID );

am6UNIT * AM6_AnimUnitCreate( INT Size );

VOID AM6_AnimInputInit( VOID );

VOID AM6_AnimInputResponse( VOID );

VOID AM6_TimerInit( VOID );
VOID AM6_TimerResponse();

/* Flip window full screen mode function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
/*VOID AM6_AnimFlipFullScreen( VOID );*/

#endif