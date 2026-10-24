
#ifndef SERIALIZATION_EXPORT_H
#define SERIALIZATION_EXPORT_H

#ifdef SERIALIZATION_STATIC_DEFINE
#  define SERIALIZATION_EXPORT
#  define SERIALIZATION_NO_EXPORT
#else
#  ifndef SERIALIZATION_EXPORT
#    ifdef Serialization_EXPORTS
        /* We are building this library */
#      define SERIALIZATION_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define SERIALIZATION_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef SERIALIZATION_NO_EXPORT
#    define SERIALIZATION_NO_EXPORT 
#  endif
#endif

#ifndef SERIALIZATION_DEPRECATED
#  define SERIALIZATION_DEPRECATED __declspec(deprecated)
#endif

#ifndef SERIALIZATION_DEPRECATED_EXPORT
#  define SERIALIZATION_DEPRECATED_EXPORT SERIALIZATION_EXPORT SERIALIZATION_DEPRECATED
#endif

#ifndef SERIALIZATION_DEPRECATED_NO_EXPORT
#  define SERIALIZATION_DEPRECATED_NO_EXPORT SERIALIZATION_NO_EXPORT SERIALIZATION_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SERIALIZATION_NO_DEPRECATED
#    define SERIALIZATION_NO_DEPRECATED
#  endif
#endif

#endif /* SERIALIZATION_EXPORT_H */
