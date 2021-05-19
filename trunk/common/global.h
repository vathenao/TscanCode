#ifndef TSCANCODE_COMMON_GLOBAL_H__
#define TSCANCODE_COMMON_GLOBAL_H__

#ifdef _WIN32
#  ifdef TSCOMMONLIB_EXPORT
#    define TSCOMMONLIB __declspec(dllexport)
#  elif defined(TSCOMMONLIB_EXPORT)
#    define TSCOMMONLIB __declspec(dllimport)
#  else
#    define TSCOMMONLIB
#  endif
#else
#  define TSCOMMONLIB
#endif

#endif // TSCANCODE_COMMON_GLOBAL_H__
