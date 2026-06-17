#include "anim/anim.h"

/* Material stock */
am6MATERIAL AM6_RndMaterials[AM6_MAX_MATERIALS]; /* Array of materials */
INT AM6_RndMaterialsSize;                        /* Materials array */

am6MATERIAL AM6_RndMtlGetDef( VOID )
{
  am6MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.90, 0.90, 0.90},
    {0.30, 0.30, 0.30},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };

  return def_mtl;
}

VOID AM6_RndMtlInit( VOID )
{
  am6MATERIAL def = AM6_RndMtlGetDef();

  AM6_RndMaterialsSize = 0;
  AM6_RndMtlAdd(&def);
}

VOID AM6_RndMtlClose( VOID )
{
  memset(AM6_RndMaterials, 0, sizeof(am6MATERIAL));
}

INT AM6_RndMtlAdd( am6MATERIAL *Mtl )
{
  if (AM6_RndMaterialsSize >= AM6_MAX_MATERIALS)
    return 0;
  AM6_RndMaterials[AM6_RndMaterialsSize] = *Mtl;
  return AM6_RndMaterialsSize++;
}

UINT AM6_RndMtlApply( INT MtlNo )
{
  INT i, loc;
  UINT prg;
  am6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= AM6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &AM6_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= AM6_RndShadersSize)
    prg = 0;
  prg = AM6_RndShaders[prg].ProgId;

  if (prg == 0)
    return 0;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, AM6_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, AM6_Anim.GlobalTime);

  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Set textures */
  /*for (i = 0; i < 8; i++)
  {
  }*/

  return prg;
}