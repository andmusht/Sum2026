/**/
#include "rndres.h"

/**/
VOID AM6_RndResInit( VOID )
{
  AM6_RndShdInit();
  AM6_RndTexInit();
  AM6_RndMtlInit();
  AM6_RndFntInit();
}

VOID AM6_RndResClose( VOID )
{
  AM6_RndShdClose();
  AM6_RndTexClose();
  AM6_RndMtlClose();
  AM6_RndFntClose();
}