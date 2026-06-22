/* FILE NAME  : rndfnt.c
 * PROGRAMMER : AM6
 * LAST UPDATE: 22.06.2026
 * PURPOSE    : 3D animation project.
 *              Rendering resources implementation module.
 *              Resource implemetation module.
 *              Font handle module.
 */

#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/* Current font description */
/* Font data */
static am6FONT AM6_RndFntFont;

/* Font characters primitives */
static am6PRIM AM6_RndFntChars[256];

/* Font material */
static INT AM6_RndFntMtlNo;

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AM6_RndFntLoad( CHAR *FileName )
{
  DWORD Sign, W, H, *Tex;
  am6VERTEX Chars[256][4];
  am6MATERIAL mtl;
  FILE *F;
  INT i;

  AM6_RndFntClose();

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }

  fread(&AM6_RndFntFont, sizeof(am6FONT), 1, F);
  fread(Chars, sizeof(am6VERTEX), 4 * 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
  if ((Tex = malloc(W * H * 4)) == NULL)
  {
    memset(&AM6_RndFntFont, 0, sizeof(AM6_RndFntFont));
    memset(&AM6_RndFntFont, 0, sizeof(AM6_RndFntFont));
    fclose(F);
    return FALSE;
  }
  fread(Tex, 4, W * H, F);
  fclose(F);

  /* Material */
  mtl = AM6_RndMtlGetDef();
  strncpy(mtl.Name, FileName, AM6_STR_MAX - 1);
  mtl.Tex[0] = AM6_RndTexAddImg(FileName, W, H, 4, Tex);
  mtl.ShdNo = AM6_RndShdAdd("font");
  AM6_RndFntMtlNo = AM6_RndMtlAdd(&mtl);

  for (i = 0; i < 256; i++)
  {
    AM6_RndPrimCreate(&AM6_RndFntChars[i], AM6_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    AM6_RndFntChars[i].MtlNo = AM6_RndFntMtlNo;
  }
  free(Tex);
  return TRUE;
} /* End of 'AM6_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AM6_RndFntInit( VOID )
{
  AM6_RndFntLoad("bin/fonts/ConsolasBold.g3df");
} /* End of 'AM6_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AM6_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    AM6_RndPrimFree(&AM6_RndFntChars[i]);
  memset(AM6_RndFntChars, 0, sizeof(AM6_RndFntChars));
  memset(&AM6_RndFntFont, 0, sizeof(AM6_RndFntFont));
} /* End of 'AM6_RndFntInit' function */

/* Draw screen space string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   - font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID AM6_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  VEC Start = Pos;

  if (Size <= 0)
    Size = AM6_RndFntFont.LineH;

  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = Start.X, Pos.Y -= Size;
    else if (AM6_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
    {
      AM6_RndPrimDraw(&AM6_RndFntChars[(UCHAR)*Str],
        MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
      Pos.X += AM6_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
    }
    Str++;
  }
} /* End of 'AM6_RndFntDraw' function */

/* END OF 'rndfnt.c' FILE */