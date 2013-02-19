#pragma once

// Set to 1 when testing for shipping game
#define SHIPPING 0

#define BOX2D_INTEGRATED 1
#define BOX2D_DEBUG BOX2D_INTEGRATED && !SHIPPING && 1

// Optimization macros (uses __pragma to enable inside a #define).
#if !SHIPPING
#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",off))
#else
#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",on))
#endif

#ifdef _DEBUG
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",off))
#else
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",on))
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS
#else
#define LINUX
#endif
