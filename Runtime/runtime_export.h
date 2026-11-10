
#ifndef RUNTIME_EXPORT_H
#define RUNTIME_EXPORT_H

#ifdef RUNTIME_STATIC_DEFINE
#  define RUNTIME_EXPORT
#  define RUNTIME_NO_EXPORT
#else
#  ifndef RUNTIME_EXPORT
#    ifdef Runtime_EXPORTS
        /* We are building this library */
#      define RUNTIME_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define RUNTIME_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef RUNTIME_NO_EXPORT
#    define RUNTIME_NO_EXPORT 
#  endif
#endif

#ifndef RUNTIME_DEPRECATED
#  define RUNTIME_DEPRECATED __declspec(deprecated)
#endif

#ifndef RUNTIME_DEPRECATED_EXPORT
#  define RUNTIME_DEPRECATED_EXPORT RUNTIME_EXPORT RUNTIME_DEPRECATED
#endif

#ifndef RUNTIME_DEPRECATED_NO_EXPORT
#  define RUNTIME_DEPRECATED_NO_EXPORT RUNTIME_NO_EXPORT RUNTIME_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef RUNTIME_NO_DEPRECATED
#    define RUNTIME_NO_DEPRECATED
#  endif
#endif

#endif /* RUNTIME_EXPORT_H */
