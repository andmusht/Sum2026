/* FILE NAME: mth.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: Andrey Mushtakov (AM6)
 * DATE: 08.06.2026
 */

#ifndef __mth_h_
#define __mth_h_

#include <math.h>

#include <windows.h>

#ifdef _MSC_VER
  #define AM6_INLINE __inline
#elif __MINGW32__
  #define AM6_INLINE static inline
#else
  #define AM6_INLINE __inline
#endif

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Unit matrix */
#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}

#define VecAddVec3(A, B, C) VecAddVec(VecAddVec(A, B), C)
#define VecAddVec4(A, B, C, D) VecAddVec((VecAddVec3(A, B, C), D)

#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr(A, B), C)
#define MatrMulMatr4(A, B, C, D) MatrMulMatr((MatrMulMatr3(A, B, C), D)
#define MatrMulMatr5(A, B, C, D, E) MatrMulMatr((MatrMulMatr4(A, B, C, D), E)

/* Base float point types */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct tagVEC2
{
  FLT X, Y; /* Vector coordinates */
} VEC2;

typedef struct tagVEC4
{
  FLT X, Y, Z, W; /* Vector coordinates */
} VEC4;

/* Transformation matrix representation type */
typedef struct tagMATR
{
  FLT A[4][4]; /* Matrix elements */
} MATR;

/* Vectors set function.
 * ARGUMENTS:
 *   - coordinates of the vector:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v = {X, Y, Z};

  return v;
} /* End of 'VecSet' function */

/* Vectors 4 set function.
 * ARGUMENTS:
 *   - coordinates of the vector:
 *       FLT X, Y, Z, P;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC4 Vec4Set( FLT X, FLT Y, FLT Z, FLT W )
{
  VEC4 v = {X, Y, Z, W};

  return v;
} /* End of 'VecSet' function */


/* Vectors set with one variable function.
 * ARGUMENTS:
 *   - coordinates of the vector:
 *       FLT A;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecSet1( FLT A )
{
  VEC v = {A, A, A};

  return v;
} /* End of 'VecSet1' function */

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Substract two vectors function.
 * ARGUMENTS:
 *   - vectors to be substract:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

/* Multiply vector by number function.
 * ARGUMENTS:
 *   - vector to be multiply:
 *       VEC V1;
 *   - number multiply:
 *       FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecMulNum( VEC V, FLT N )
{
  return VecSet(V.X * N, V.Y * N, V.Z * N);
} /* End of 'VecMulNum' function */

/* Divide vector by number function.
 * ARGUMENTS:
 *   - vector to be divide:
 *       VEC V1;
 *   - number divide:
 *       FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecDivNum( VEC V, FLT N )
{
  return VecSet(V.X / N, V.Y / N, V.Z / N);
} /* End of 'VecDivNum' function */

/* Vector invert function.
 * ARGUMENTS:
 *   - vector to be inverted:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Vector scal muitiply function.
 * ARGUMENTS:
 *   - vectors to be multyply:
 *       VEC V;
 * RETURNS:
 *   (FLT) result.
 */
AM6_INLINE FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V2.Z * V1.Z;
} /* End of 'VecDotVec' function */

/* Vector lenght function.
 * ARGUMENTS:
 *   - vectors whichs lenght to be found:
 *       VEC V;
 * RETURNS:
 *   (FLT) result.
 */
AM6_INLINE FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'VecLen' function */

/* Square of the vector lenght function.
 * ARGUMENTS:
 *   - vectors whichs lenght to be found:
 *       VEC V;
 * RETURNS:
 *   (FLT) result.
 */
AM6_INLINE FLT VecLen2( VEC V )
{
  FLT len = VecDotVec(V, V);

  return len;
} /* End of 'VecLen2' function */

/* Normalize vector function.
 * ARGUMENTS:
 *   - vectors to be normalized:
 *       VEC V;
 * RETURNS:
 *   (FLT) result.
 */
AM6_INLINE VEC VecNormalize( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
} /* End of 'VecNormalize' function */

/*  function.
 * ARGUMENTS:
 *   - vector to be transformed:
 *       VEC V;
 *   -  matrix 4x3
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC PointTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'PointTransform' function */

/*  function.
 * ARGUMENTS:
 *   - vector to be transformed:
 *       VEC V;
 *   -  matrix 3x3
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTransform' function */

/* Matrix identity function.
 * ARGUMENTS:
 *   None;
 * RETURNS:
 *   (MATR) identity matrix.
 */
AM6_INLINE MATR MatrIdentity( VOID )
{
  MATR i = UnitMatrix;
  return i;
} /* End of 'MatrIdentity' function */

/* Set matrix function.
 * ARGUMENTS:
 *   - varibles to be set:
 *       FLT from A00 to A33;
 * RETURNS:
 *   (MATR) matrix we set.
 */
AM6_INLINE MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
                       FLT A10, FLT A11, FLT A12, FLT A13,
                       FLT A20, FLT A21, FLT A22, FLT A23,
                       FLT A30, FLT A31, FLT A32, FLT A33 )
{
  MATR m =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };

  return m;
} /* End of 'MatrSet' function */

/* Translate vector to matrix function.
 * ARGUMENTS:
 *   - vector to be translated:
 *       VEC T;
 * RETURNS:
 *   (MATR) matrix translated matrix.
 */
AM6_INLINE MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;
  return m;
} /* End of 'MatrTranslate' function */

/* Matrix by matrix multiplication function.
 * ARGUMENTS:
 *   - source matrix:
 *       MATR M;
 *   - multiplied matri x:
 *       MATR M;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r = {{{0}}};
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMulMatr' function */

/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

AM6_INLINE MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Z, 0,
                 0, 0, 0, 1);
} /* End of 'MatrScale' function */

/* Matrix rotate function.
 * ARGUMENTS:
 *   - vector to be rotated:
 *       VEC V;
 *   - angle in degree to rotate:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrRotate( FLT AngleInDegrees, VEC R )
{
  FLT 
    a = D2R(AngleInDegrees),
    si = sin(a), co = cos(a);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co), 
        V.X * V.Y * (1 - co) + V.Z * si, 
        V.X * V.Z * (1 - co) - V.Y * si, 0},
      {V.Y * V.X * (1 - co) - V.Z * si,
         co + V.Y * V.Y * (1 - co),
         V.Y * V.Z * (1 - co) + V.X * si, 0},
      {V.Z * V.X * (1 - co) + V.Y * si,
         V.Z * V.Y * (1 - co) - V.X * si,
         co + V.Z * V.Z * (1 - co), 0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /* End of 'MatrRotate' function */

/* Matrix rotate by X axis function.
 * ARGUMENTS:
 *   - angle in degree to rotate:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrRotateX( FLT AngleInDegrees )
{
  FLT
    a = D2R(AngleInDegrees),
    si = sin(a), co = cos(a);

  return MatrSet(1, 0, 0, 0,
                 0, co, si, 0,
                 0, -si, co, 0,
                 0, 0, 0, 1);
} /* End of 'MatrRotateX' function */

/* Matrix rotate by Y axis function.
 * ARGUMENTS:
 *   - angle in degree to rotate:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrRotateY( FLT AngleInDegrees )
{
  FLT
    a = D2R(AngleInDegrees),
    si = sin(a), co = cos(a);

  return MatrSet(co, 0, -si, 0,
                 0, 1, 0, 0,
                 si, 0, co, 0,
                 0, 0, 0, 1);
} /* End of 'MatrRotateY' function */

/* Matrix rotate by Y axis function.
 * ARGUMENTS:
 *   - angle in degree to rotate:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrRotateZ( FLT AngleInDegrees )
{
  FLT
    a = D2R(AngleInDegrees),
    si = sin(a), co = cos(a);

  return MatrSet(co, si, 0, 0,
                 -si, co, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
} /* End of 'MatrRotateY' function */

/* Transpose matrix function.
 * ARGUMENTS:
 *   - matrix to be transposed:
 *       MATR M;
 * RETURNS:
 *   (MATR) transposed matrix.
 */
AM6_INLINE MATR MatrTranspose( MATR M )
{
  return MatrSet(M.A[0][0], M.A[1][0], M.A[2][0], M.A[3][0],
                 M.A[0][1], M.A[1][1], M.A[2][1], M.A[3][1],
                 M.A[0][2], M.A[1][2], M.A[2][2], M.A[3][2],
                 M.A[0][3], M.A[1][3], M.A[2][3], M.A[3][3]);
} /* End of 'MatrTranspose' function */

/* 3x3 matrix determinator find function.
 * ARGUMENTS:
 *   - 3x3 matrix :
 *       FLT from A11 to A33;
 * RETURNS:
 *   (FLT) determinator.
 */
AM6_INLINE FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /*End of 'MatrDeterm3x3' function */

/* Matrix determinator find function.
 * ARGUMENTS:
 *   - matrix which determinator to be found:
 *       MATR;
 * RETURNS:
 *   (FLT) determinator.
 */
AM6_INLINE FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

/* Inverse matrix function.
 * ARGUMENTS:
 *   - matrix which inverse matrix to be found:
 *       MATR;
 * RETURNS:
 *   (MATR) inverse matrix.
 */
AM6_INLINE MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return MatrIdentity();

  /* Build adjoint matrix */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[0][1] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[0][2] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[0][3] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[1][0] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[2][0] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[3][0] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]) / det;
  return r;
} /* End of 'MatrInverse' function */

/* Vecrtor multiply by vector function.
 * ARGUMENTS:
 *   - vectors to be multiplied:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecCrossVec( VEC V1, VEC V2 )
{
  VEC r;

  r.X = V1.Y * V2.Z - V1.Z * V2.Y;
  r.Y = V1.Z * V2.X - V1.X * V2.Z;
  r.Z = V1.X * V2.Y - V1.Y * V2.X;
  return r;
} /* End of 'VecCrossVec' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

AM6_INLINE VEC MatrRight( VEC Loc, VEC At, VEC Up1 )
{
 VEC
    Dir = VecNormalize(VecSubVec(Up1, Loc));

  return VecNormalize(VecCrossVec(Dir, Up1));
}

/* Orthographics projection matrix setup function.
 * ARGUMENTS:
 *   - orthographic box side facets coordinates:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR  ) result matrix.
 */
AM6_INLINE MATR MatrOrtho( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {              2 / (Right - Left),                                0,                            0, 0},
      {                               0,               2 / (Top - Bottom),                            0, 0},
      {                               0,                                0,            -2 / (Far - Near), 0},
      {-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1}
    }
  };

  return m;
} /* End of 'MatrOrtho' function */

/* Perspective (frustum) projection matrix setup function.
 * ARGUMENTS:
 *   - frustum side facets coordinates:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
AM6_INLINE MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };

  return m;
} /* End of 'MatrFrustum' function */

/* Random number obtain in range [0.0 .. 1.0] function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (DBL) result random number.
 */
AM6_INLINE DBL Rnd0( VOID )
{
  return (DBL)rand() / RAND_MAX;
} /* End of 'Rnd0' function */
 
/* Random number obtain in range [-1.0 .. 1.0] function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (DBL) result random number.
 */
AM6_INLINE DBL Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function */

/* Obtain minimum of two vectors function.
 * ARGUMENTS:
 *   - vectors to be compare:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecMinVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X < V2.X ? V1.X : V2.X,
                V1.Y < V2.Y ? V1.Y : V2.Y,
                V1.Z < V2.Z ? V1.Z : V2.Z);
} /* End of 'VecMinVec' function */

/* Obtain miaximum of two vectors function.
 * ARGUMENTS:
 *   - vectors to be compare:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
AM6_INLINE VEC VecMaxVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X > V2.X ? V1.X : V2.X,
                V1.Y > V2.Y ? V1.Y : V2.Y,
                V1.Z > V2.Z ? V1.Z : V2.Z);
} /* End of 'VecMaxVec' function */

#endif /* __mth_h_ */

/* END OF 'mth.h' FILE */
