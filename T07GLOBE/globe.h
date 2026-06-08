#include "mth.h"

#ifndef __globe_h_
#define __globe_h_

#define GLB_GRID_W 14 * 40
#define GLB_GRID_H 20 * 40

#define GLB_MAX(A,B)((A) < (B) ? (B) : (A))
#define GLB_MIN(A,B)((A) < (B) ? (A) : (B))

VOID GLB_Init( DBL R );

VOID GLB_Resize( INT Ws, INT Hs );

VOID GLB_Draw( HDC hDC );

#endif /* __globe_h_ */
