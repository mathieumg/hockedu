///////////////////////////////////////////////////////////////////////////////
/// @file Utilitaire.cpp
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#if WIN32
#define _WINSOCKAPI_
#include <Windows.h>
#endif

#include "Solution_Defines.h"
#if __linux__
#include <string.h>
#else
#include <string>
#endif
#include <vector>
#include <algorithm>
#include <stdarg.h>
#include <iostream>
#include "Utilitaire.h"
#include "Enum_Declarations.h"

/**
* Helper function to write formatted output using an argument list
*
* @param Dest - destination string buffer
* @param DestSize - size of destination buffer
* @param Count - number of characters to write (not including null terminating character)
* @param Fmt - string to print
* @param Args - argument list
* @return number of characters written or -1 if truncated
*/
int GetVarArgs( char* Dest, int DestSize, int Count, const char*& Fmt, va_list ArgPtr )
{
    int Result = vsprintf(Dest,Fmt,ArgPtr);
    va_end( ArgPtr );
    return Result;
}

// Utilitaire pour obtenir un nombre associé à une chaine de caractère
int GenerateHashCode( const char* pString )
{
    size_t _Val = 2166136261U;
    size_t _First = 0;
    size_t _Last = strlen(pString);
    size_t _Stride = 1 + _Last / 10;

    for(; _First < _Last; _First += _Stride)
        _Val = 16777619U * _Val ^ (size_t)pString[_First];
    return (int)_Val;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void __cdecl appFailAssertFunc( const char* Expr, const char* File, int Line, const char* Format/*=""*/, ... )
///
/// /*Description*/
///
/// @param[in] const char * Expr
/// @param[in] const char * File
/// @param[in] int Line
/// @param[in] const char * Format
/// @param[in] ...
///
/// @return void __cdecl
///
////////////////////////////////////////////////////////////////////////
void __cdecl appFailAssertFunc( const char* Expr, const char* File, int Line, const char* Format/*=""*/, ... )
{
    char assertIDText[2048];
    sprintf_s(assertIDText,"[File:%s] [Line: %i]",File,Line);
    char AssertMsg[2048];
    GET_VARARGS( AssertMsg, ARRAY_COUNT(AssertMsg), ARRAY_COUNT(AssertMsg)-1, Format, Format );

    std::string displayMessage;
    displayMessage += assertIDText;
    displayMessage += "\n";
    displayMessage += Expr;
    displayMessage += "\n";
    displayMessage += AssertMsg;

    static std::vector<int> IgnoredAssertTrackingList;
    int hash = GenerateHashCode(assertIDText);

    bool bIsAssertIgnored = find(IgnoredAssertTrackingList.begin(),IgnoredAssertTrackingList.end(),hash) != IgnoredAssertTrackingList.end();

    if(!bIsAssertIgnored)
    {
        AssertHandleMode HandleMode = utilitaire::DisplayAssertMessage( displayMessage.c_str( ) );
        switch( HandleMode )
        {
        case ASSERT_IgnoreAll:
            IgnoredAssertTrackingList.push_back( hash );
            // fall through
        case ASSERT_Ignore:
            bIsAssertIgnored = true;
            break;
        case ASSERT_Break:
            // fall through
        default:
            break;
        }
    }

    if( !bIsAssertIgnored )
    {
        appDebugBreak();
    }

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void appDebugBreak()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void appDebugBreak()
{
#if WIN32
    __debugbreak();
#endif
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
