#ifndef __def_h_
#define __def_h_

#pragma warning(disable: 4244)

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     (_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF |        \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)),           \
      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE),   \
      _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR),                      \
      _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE),  \
      _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR),                     \
      _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE), \
      _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR))

#else /* NDEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* NDEBUG */
#include <stdlib.h>

#include "mth/mth.h"

typedef unsigned long long UINT64;
typedef long long INT64;

#endif /* __def_h_ */