/* FILE NAME: rnd.h
 * PURPOSE: 3D render declaration module.
 * PROGRAMMER: AM6
 * DATE: 08.06.2026
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC

#include <glew.h>

#include "anim/rnd/res/rndres.h"
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
extern VEC
          AM6_RndCamLoc,
          AM6_RndCamAt,
          AM6_RndCamDir,
          AM6_RndCamUp;

extern VEC AM6_RndCamRight;

extern INT AM6_MouseWheel;

VOID AM6_RndInit( HWND hWnd );
VOID AM6_RndClose( VOID );
VOID AM6_RndResize( INT W, INT H );
VOID AM6_RndCopyFrame( VOID );
VOID AM6_RndStart( VOID );
VOID AM6_RndEnd( VOID );
VOID AM6_RndProjSet( VOID );
VOID AM6_RndCamSet( VEC Loc, VEC At, VEC Up );

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

/* Grid topology representation type */
typedef struct tagam6GRID
{
  INT W, H;
  am6VERTEX *V;
} am6GRID;

/* Primitive type */
typedef enum tagam6PRIM_TYPE
{
  AM6_RND_PRIM_POINTS,   /* Array of points  – GL_POINTS */
  AM6_RND_PRIM_LINES,    /* Line segments (by 2 points) – GL_LINES */
  AM6_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles – GL_TRIANGLES */
  AM6_RND_PRIM_TRISTRIP,
} am6PRIM_TYPE;

/* Primitive representation type */
typedef struct tagam6PRIM
{
  am6PRIM_TYPE Type; /* Primitive type */

  INT
    VA,              /* Vertex array Id */
    VBuf,            /* Vertex buffer Id */
    IBuf;            /* Index buffer Id (if 0 - use only vertex buffer) */

  INT NumOfElements; /* Number of indices/vecrtices */

  VEC MinBB, MaxBB;  /* Bound box */

  MATR Trans;   /* Additional transformation matrix */

  INT MtlNo;    /* Material number at stock array */

} am6PRIM;

/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       am6PRIM *Pr;
 *   - primitive type:
 *       am6PRIM_TYPE Type;
 *   - vertex attributes array:
 *       am6VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID AM6_RndPrimCreate( am6PRIM *Pr, am6PRIM_TYPE Type,
                        am6VERTEX *V, INT NoofV, INT *Ind, INT NoofI );
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

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndGridCreate( am6GRID *G, INT W, INT H );

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 * RETURNS: None.
 */
VOID AM6_RndGridFree( am6GRID *G );

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       am6PRIM *Pr;
 *   - grid data:
 *       am6GRID *G;
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndPrimFromGrid( am6PRIM *Pr, am6GRID *G );

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 * RETURNS: None.
 */
VOID AM6_RndGridAutoNormals( am6GRID *G );

/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       am6GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndGridCreateSphere( am6GRID *G, FLT R, INT W, INT H );

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *       UINT Source;
 *   - error type:
 *       UINT Type;
 *   - error message id:
 *       UINT Id, 
 *   - message severity:
 *       UINT severity, 
 *   - message text length:
 *       INT Length, 
 *   - message text:
 *       CHAR *Message, 
 *   - user addon parameters pointer:
 *       VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message,
                             const VOID *UserParam );

/***
 * Primitives support
 ***/

extern INT AM6_RndShdAddonI[8];
extern INT AM6_RndShdAddonF[8];
extern INT AM6_RndShdAddonV[8];

/* Primitive collection data type */
typedef struct tagam6PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  am6PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} am6PRIMS;

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       am6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL AM6_RndPrimsCreate( am6PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       am6PRIMS *Prs;
 * RETURNS: None.
 */
VOID AM6_RndPrimsFree( am6PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       am6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID AM6_RndPrimsDraw( am6PRIMS *Prs, MATR World );

/* Load primitives from '*.G3DM' file function.
 * ARGUMENTS:
 *   - pointer to primitives to create:
 *       am6PRIMS *Prs;
 *   - '*.G3DM' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndPrimsLoad( am6PRIMS *Prs, CHAR *FileName );


#endif /* __rnd_h_ */
