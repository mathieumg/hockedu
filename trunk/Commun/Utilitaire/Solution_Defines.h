#pragma once


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS 1
#elif __linux__
#define LINUX 1
#else
#define MACLOL 1
#endif

// Set to 1 when testing for shipping game
#define SHIPPING 0

#define STL_ITERATE(container, var) for(auto var=container.begin(); var != container.end(); ++var)

#if WIN32
#define PLAY_GAME 1
#define BOX2D_INTEGRATED 1
#define BOX2D_PLAY BOX2D_INTEGRATED && PLAY_GAME
#define BOX2D_DEBUG BOX2D_INTEGRATED && !SHIPPING && 1
#else
#define PLAY_GAME 0
#define BOX2D_INTEGRATED 1
#define BOX2D_PLAY BOX2D_INTEGRATED && PLAY_GAME
#define BOX2D_DEBUG BOX2D_INTEGRATED && !SHIPPING && 0
#endif

#define MANUAL_PHYSICS_DETECTION !BOX2D_PLAY && PLAY_GAME


#define HANDLE_CHARACTERE_0 0

// Optimization macros (uses __pragma to enable inside a #define).
#if WIN32

#if !SHIPPING
#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",off))
#else
#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",on))
#endif

#else
#define PRAGMA_DISABLE_OPTIMIZATION
#endif

#if WIN32
#ifdef _DEBUG
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",off))
#else
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",on))
#endif

#else
#define PRAGMA_ENABLE_OPTIMIZATION
#endif
// Méthode utilitaire
#ifdef WINDOWS
typedef char *  va_list;
#elif defined(LINUX) || defined(__APPLE__)
#include <cstdio>
#endif

// permet de connaitre la longueur d'un array
#define ARRAY_COUNT( array ) \
    ( sizeof(array) / sizeof((array)[0]) )

/// Solution utilities
#define CheckTime(content) \
{\
    clock_t t = clock();\
    content;\
    t = clock() - t;\
{int ms = t, s = ms/1000, m = s/60;\
    printf(#content"\n[%d:%d:%d]\n", m, s%60, ms%1000);}}\

// Permet de faire un breakpoint avec du code
//#define appDebugBreak()     ( *((int*)3) = 13 )
void appDebugBreak();

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


// Fonction pour faire la verification
#if !SHIPPING
#define checkf(expr, ...)   { if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__, ##__VA_ARGS__ ); }
#else
#define checkf(expr, ...)
#endif

#ifndef WINDOWS
#define sprintf_s sprintf
#define strcpy_s(buf, bufLen, arrToCopy) { strcpy(buf, arrToCopy); }
#define localtime_s(tmStructReference, time_tReference) { time(time_tReference); *tmStructReference = *localtime(time_tReference); }
#define memcpy_s(dstArray, dstArrayLen, srcArray, srcArrayLen) { memcpy(dstArray, srcArray, srcArrayLen); }
#define sscanf_s sscanf
#define _localtime64_s(tmStructReference, time_tReference) { time(time_tReference); tmStructReference = localtime(time_tReference); return (tMStructReference == NULL); }
#endif
