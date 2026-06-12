/* FILE NAME: rnd.h
 * PURPOSE: 3D render declaration module.
 * PROGRAMMER: AM6
 * DATE: 08.06.2026
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC

#include <glew.h>
#include "def.h"

extern HWND AM6_hRndWnd;                 /* Work window handle */
extern HDC AM6_hRndDC;              /* Work window memory device context  */
/*extern HBITMAP AM6_hRndBmFrame;           Work window background bitmap handle */
extern HGLRC AM6_hRndGLRC;
extern INT AM6_RndFrameW, AM6_RndFrameH; /* Work window size */

extern DBL
          AM6_RndProjSize,         /* Project plane fit square */
          AM6_RndProjDist,         /* Distance to project plane from viewer (near) */
          AM6_RndProjFarClip;      /* Distance to project far clip plane (far) */

extern MATR
           AM6_RndMatrView,              /* View coordinate system matrix */
           AM6_RndMatrProj,              /* Projection coordinate system matrix */
           AM6_RndMatrVP;                /* Stored (View * Proj) matrix */

/***
 *  Primitive handle
 ***/

/* Vertex representation type */
typedef struct tagam6VERTEX
{
  VEC P;   /* Vertex position */
  VEC2 T;  /* Vertex texture coordinate */
  VEC N;   /* Vertex normal */
  VEC4 C;  /* Vertex color */
} am6VERTEX;

/* Primitive representation type */
typedef struct tagam6PRIM
{
  am6VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */

  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */

  MATR Trans;   /* Additional transformation matrix */
} am6PRIM;


VOID AM6_RndInit( HWND hWnd );
VOID AM6_RndClose( VOID );
VOID AM6_RndResize( INT W, INT H );
VOID AM6_RndCopyFrame( VOID );
VOID AM6_RndStart( VOID );
VOID AM6_RndEnd( VOID );
VOID AM6_RndProjSet( VOID );
VOID AM6_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Primitive create function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       am6PRIM *Pr;
 *   - number of vertecis and indices:
 *       INT NoofV, NoofI;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL AM6_RndPrimCreate( am6PRIM *Pr, INT NoofV, INT NoofI );

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be free:
 *       am6PRIM *Pr;
 * RETURNS: None.
 */
VOID AM6_RndPrimFree( am6PRIM *Pr );

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive to be draw:
 *       am6PRIM *Pr;
 *   - transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID AM6_RndPrimDraw( am6PRIM *Pr, MATR World );

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       am6PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL AM6_RndPrimLoad( am6PRIM *Pr, CHAR *FileName );

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       am6PRIM *Pr;
 *   - sphere radius:
 *       DBL R;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndPrimCreateSphere( am6PRIM *Pr, DBL R, INT W, INT H );

BOOL AM6_RndPrimCreateCylinder( am6PRIM *Pr, DBL R, INT W, INT H );

/* Tri-mesh geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - vertex array:
 *       am6VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *Ind;
 *   - index array size:
 *       INT NumOfI;
 */
VOID AM6_RndPrimTriMeshAutoNormals( am6VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI );



#endif /* __rnd_h_ */