#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

/* Shaders stock maximum size */
#define AM6_STR_MAX 300

VOID AM6_RndResInit( VOID );
VOID AM6_RndResClose( VOID );

#define AM6_MAX_SHADERS 30
/* Shader program store type */
typedef struct tagam6SHADER
{
  CHAR Name[AM6_MAX_SHADERS]; /* Shader filename prefix */
  UINT ProgId;                /* Shader program Id */
} am6SHADER;
/* Array of shaders */
extern am6SHADER AM6_RndShaders[AM6_MAX_SHADERS];
/* Shadres array store size */
extern INT AM6_RndShadersSize;

VOID AM6_RndShdInit( VOID );
VOID AM6_RndShdClose( VOID );

VOID AM6_RndShdUpdate( VOID );
INT AM6_RndShdAdd( CHAR *ShaderFileNamePrefix );

typedef struct tagam6MATERIAL
{
  CHAR Name[AM6_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */

  FLT Trans;                /* Transparency factor */

  INT Tex[8];               /* Texture references from texture table (or -1) */

  INT ShdNo;                /* Shader number in shader table */
} am6MATERIAL;
#define AM6_MAX_MATERIALS 300
/* Material stock */
extern am6MATERIAL AM6_RndMaterials[AM6_MAX_MATERIALS]; /* Array of materials */
extern INT AM6_RndMaterialsSize;                        /* Materials array */
VOID AM6_RndMtlInit( VOID );
VOID AM6_RndMtlClose( VOID );

am6MATERIAL AM6_RndMtlGetDef( VOID );
INT AM6_RndMtl  Add( am6MATERIAL *Mtl );
UINT AM6_RndMtlApply( INT MtlNo );
am6MATERIAL * AM6_RndMtlGet( INT MtlNo );

/* Texture representation type */
typedef struct tagam6TEXTURE
{
  CHAR Name[AM6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} am6TEXTURE;
#define AM6_MAX_TEXTURES 3000
/* Textures stock */
extern am6TEXTURE AM6_RndTextures[AM6_MAX_TEXTURES]; /* Array of textures */
extern INT AM6_RndTexturesSize;                      /* Textures array store size */

VOID AM6_RndTexInit( VOID );
VOID AM6_RndTexClose( VOID );
INT AM6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

#endif /* __rndres_h_ */