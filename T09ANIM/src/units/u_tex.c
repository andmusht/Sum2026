#include "units.h"

typedef struct tagam6UNIT_TEX am6UNIT_TEX;

typedef struct tagam6UNIT_TEX
{
  UNIT_BASE_FIELDS;
  am6PRIM Pr;
  INT MtlNo, TexId;
};

static VOID AM6_UnitInit( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  am6VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 1, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
  };
  FLT t[2][2] =
  {
    {0.8, 1},
    {1, 0.3}
  };
  am6MATERIAL mtl = AM6_RndMtlGetDef();

  strncpy(mtl.Name, "Texture sample", AM6_STR_MAX - 1);
  mtl.ShdNo = AM6_RndShdAdd("tex");
  Uni->MtlNo = AM6_RndMtlAdd(&mtl);

  glGenTextures(1, &Uni->TexId);
  glBindTexture(GL_TEXTURE_2D, Uni->TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, 1, 2, 2, 0, GL_LUMINANCE, GL_FLOAT, t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  AM6_RndPrimCreate(&Uni->Pr, AM6_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
}

static VOID AM6_UnitClose( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  glDeleteTextures(1, &Uni->TexId);
  AM6_RndPrimFree(&Uni->Pr); 
}

static VOID AM6_UnitResponse( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
} 

static VOID AM6_UnitRender( am6UNIT_TEX *Uni, am6ANIM *Ani )
{
  glActiveTexture(GL_TEXTURE0 + 5);
  glBindTexture(GL_TEXTURE_2D, Uni->TexId);
  AM6_RndPrimDraw(&Uni->Pr, MatrIdentity());
}

am6UNIT * AM6_AnimUnitCreateTexture( VOID )
{
  am6UNIT_TEX *Uni;
 
  if ((Uni = (am6UNIT_TEX *)AM6_AnimUnitCreate(sizeof(am6UNIT_TEX))) == NULL)
    return NULL;

  Uni->Init = (VOID *)AM6_UnitInit;
  Uni->Close = (VOID *)AM6_UnitClose;
  Uni->Response = (VOID *)AM6_UnitResponse;
  Uni->Render = (VOID *)AM6_UnitRender;

  return (am6UNIT *)Uni;
}