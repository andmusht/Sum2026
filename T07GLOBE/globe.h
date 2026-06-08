#ifndef __globe_h_
#define __globe_h_

#include <windows.h>

typedef DOUBLE DBL;

#define PI 3.14159265

#define GLB_GRID_W 140
#define GLB_GRID_H 200

#define GLB_MAX(A,B)((A) < (B) ? (B) : (A))
#define GLB_MIN(A,B)((A) < (B) ? (A) : (B))

typedef struct 
{
  DBL X, Y, Z;
} VEC;

VOID GLB_Init( DBL R );

VOID GLB_Resize( INT Ws, INT Hs );

VOID GLB_Draw( HDC hDC, DBL Rs );

#endif /* __globe_h_ */
