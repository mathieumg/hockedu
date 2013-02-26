#pragma once

// Set to 1 when testing for shipping game
#define SHIPPING 0

#define BOX2D_INTEGRATED 1
#define BOX2D_DEBUG BOX2D_INTEGRATED && !SHIPPING && 1

// Optimization macros (uses __pragma to enable inside a #define).
#if !SHIPPING
#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",off))
#pragma warning(disable:4748)
#else
#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",on))
#endif

#ifdef _DEBUG
#pragma warning(default:4748)
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",off))
#else
#pragma warning(default:4748)
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",on))
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS
#else
#define LINUX
#endif

/// Solution utilities
#ifdef WINDOWS
#define CheckTime(content) \
{\
    clock_t t = clock();\
    content;\
    t = clock() - t;\
{int ms = t, s = ms/1000, m = s/60;\
    printf("%d:%d:%d\n", m, s%60, ms%1000);}}\

// Permet de faire un breakpoint avec du code
//#define appDebugBreak()     ( *((int*)3) = 13 )
void appDebugBreak();


// permet de connaitre la longueur d'un array
#define ARRAY_COUNT( array ) \
    ( sizeof(array) / sizeof((array)[0]) )

// Méthode utilitaire
#ifdef WINDOWS
typedef char *  va_list;
#elif defined(LINUX)
#include <cstdio>
#endif
int GetVarArgs( char* Dest, int DestSize, int Count, const char*& Fmt, va_list ArgPtr );
#define GET_VARARGS(msg,msgsize,len,lastarg,fmt) { va_list ap; va_start(ap,lastarg);GetVarArgs(msg,msgsize,len,fmt,ap); }

// Affiche un message d'assesrtion
enum AssertHandleMode {
    ASSERT_Break,
    ASSERT_Ignore,
    ASSERT_IgnoreAll,
};
AssertHandleMode __cdecl DisplayAssertMessage( const char *message );

/** Failed assertion handler.  Warning: May be called at library startup time. */
void __cdecl appFailAssertFunc( const char* Expr, const char* File, int Line, const char* Format="", ... );

// Utilitaire
#if __INTEL_COMPILER
#define appFailAssert(expr,file,line,...)				{ appFailAssertFunc(expr, file, line, __VA_ARGS__); }
#else
#define appFailAssert(expr,file,line,...)				{ appFailAssertFunc(expr, file, line, ##__VA_ARGS__); }
#endif

//ifdef WINDOWS
#endif

// Fonction pour faire la verification
#ifdef LINUX
#define checkf(expr, ...)
#elif !SHIPPING
#define checkf(expr, ...)   { if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__, ##__VA_ARGS__ ); }
#else
#define checkf(expr, ...)
#endif
