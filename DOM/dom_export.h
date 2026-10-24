
#ifndef DOM_EXPORT_H
#define DOM_EXPORT_H

#ifdef DOM_STATIC_DEFINE
#  define DOM_EXPORT
#  define DOM_NO_EXPORT
#else
#  ifndef DOM_EXPORT
#    ifdef DOM_EXPORTS
        /* We are building this library */
#      define DOM_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define DOM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef DOM_NO_EXPORT
#    define DOM_NO_EXPORT 
#  endif
#endif

#ifndef DOM_DEPRECATED
#  define DOM_DEPRECATED __declspec(deprecated)
#endif

#ifndef DOM_DEPRECATED_EXPORT
#  define DOM_DEPRECATED_EXPORT DOM_EXPORT DOM_DEPRECATED
#endif

#ifndef DOM_DEPRECATED_NO_EXPORT
#  define DOM_DEPRECATED_NO_EXPORT DOM_NO_EXPORT DOM_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DOM_NO_DEPRECATED
#    define DOM_NO_DEPRECATED
#  endif
#endif

#endif /* DOM_EXPORT_H */
