///////////////////////////////////////////////////////////////////////////////
/// @file Utilitaire.cpp
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "Utilitaire.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdarg.h>
#include <Windows.h>

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
    int Result = vsprintf_s(Dest,Count,Fmt,ArgPtr);
    va_end( ArgPtr );
    return Result;
}

// Utilitaire pour obtenir un nombre associé à une chaine de caractère
int GenerateHashCode( const char* pString )
{
    size_t _Val = 2166136261U;
    size_t _First = 0;
    size_t _Last = ARRAY_COUNT(pString);
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

    static std::vector<int> IgnoredAssertTrackingList;
    int hash = GenerateHashCode(assertIDText);

    bool bIsAssertIgnored = find(IgnoredAssertTrackingList.begin(),IgnoredAssertTrackingList.end(),hash) != IgnoredAssertTrackingList.end();

    if(!bIsAssertIgnored)
    {
        AssertHandleMode HandleMode = DisplayAssertMessage( AssertMsg );
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

    if(!bIsAssertIgnored)
    {
        appDebugBreak();
    }

}


/** Displays an assert dialog AssertMsg with choice to BREAK, IGNORE or IGNORE ALL (release builds only) */
AssertHandleMode __cdecl DisplayAssertMessage( const char *Fmt, ... )
{
    enum AssertHandleMode HandleMode = ASSERT_Break;
    char TempStr[4096];
    char MessageStr[4096] = "Press [Abort] to exit and create crash report\nPress [Retry] to ignore the assertion and continue\nPress [Ignore] to *always* ignore this assertion\n\n";

    // Prepare assert AssertMsg
    GET_VARARGS( TempStr, ARRAY_COUNT(TempStr), ARRAY_COUNT(TempStr)-1, Fmt, Fmt );

    // Also add the instructions
    strcat_s( MessageStr, TempStr);

    int Result = MessageBoxA( NULL, MessageStr, "Assertion Failed", MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_TOPMOST );
    if( Result == IDRETRY )
    {
        HandleMode = ASSERT_Ignore;
    }
    else if (Result == IDIGNORE)
    {
        HandleMode = ASSERT_IgnoreAll;
    }
    return HandleMode;
}

namespace utilitaire {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void afficherErreur(const std::string& message)
   ///
   /// Cette fonction affiche une message d'erreur.
   ///
   /// @param[in] message : La chaîne décrivant l'erreur.
   ///
   /// @return Aucune
   ///
   ////////////////////////////////////////////////////////////////////////
   void afficherErreur(const std::string& message)
   {
   #ifdef WIN32
      MessageBoxA(0, message.c_str(), "Erreur", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
   #else
      std::cerr << message << std::endl;
   #endif
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool EGAL_ZERO(double nombre)
   ///
   /// Fonction pour savoir si un double est égal à zéro.
   ///
   /// @param[in] nombre : Valeur à vérifier.
   ///
   /// @return true si nombre est égal a 0, false autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool EGAL_ZERO(double nombre)
   {
      return (nombre < EPSILON) && (-nombre < EPSILON);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn double RAD_TO_DEG(double angleRad)
   ///
   /// Cette fonction convertit les angles de radians en degrés.
   ///
   /// @param[in] angleRad : L'angle en radians à convertir.
   ///
   /// @return L'angle converti en degrés
   ///
   ////////////////////////////////////////////////////////////////////////
   double RAD_TO_DEG(double angleRad) 
   {
      return angleRad * 57.2957795130823208767981548141052;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn double DEG_TO_RAD(double angleDeg)
   ///
   /// Cette fonction convertit les angles de degrés en radians.
   ///
   /// @param[in] angleDeg : L'angle en degrés à convertir.
   ///
   /// @return L'angle converti en radians.
   ///
   ////////////////////////////////////////////////////////////////////////
   double DEG_TO_RAD(double angleDeg)
   {
      return angleDeg * 0.0174532925199432957692369076848861;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float KMH_TO_MS( float kmh )
   ///
   /// Fonction de conversion des km/h en m/s.
   ///
   /// @param[in] kmh : Valeur en kilomètres / heure.
   ///
   /// @return float => vitesse en mètres par seconde.
   ///
   ////////////////////////////////////////////////////////////////////////
   float KMH_TO_MS( float kmh )
   {
      return (kmh * 1000.0f / 3600.0f);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float MS_TO_KMH( float ms )
   ///
   /// Fonction de conversion des m/s en km/h.
   ///
   /// @param[in] ms : Valeur en mètres / seconde.
   ///
   /// @return float => vitesse en kilomètres / heure.
   ///
   ////////////////////////////////////////////////////////////////////////
   float MS_TO_KMH( float ms )
   {
      return (ms * 3.6f);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn int SIGN( float nombre )
   ///
   /// Fonction pour connaître le signe d'un nombre.
   ///
   /// @param[in] nombre : Valeur dont on veut trouver le signe.
   ///
   /// @return int => 1 si positif, -1 si négatif, 0 si = 0.
   ///
   ////////////////////////////////////////////////////////////////////////
   float SIGN( float nombre )
   {
      if (nombre == 0.0f) {
         return 0.0f;
      }
      else if (nombre > 0.0f) {
         return  1.0f;
      }
      else {
         return -1.0f;
      }
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float ABS( float nombre )
   ///
   /// Fonction qui retourne la valeur absolue.
   ///
   /// @param[in] nombre : Valeur dont on veut trouver la valeur absolue.
   ///
   /// @return float => la valeur absolue du nombre.
   ///
   ////////////////////////////////////////////////////////////////////////
   float ABS( float nombre )
   {
      return (SIGN(nombre) * nombre);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool DANS_INTERVALLE( double valeur, double borneMin, double borneMax ) 
   ///
   /// Cette fonction vérifie si une valeur est dans un intervalle.
   ///
   /// @param[in] valeur   : La valeur à vérifier.
   /// @param[in] borneMin : La borne inférieure de l'intervalle.
   /// @param[in] borneMax : La borne supérieure de l'intervalle.
   ///
   /// @return Vrai si la valeur est dans l'intervalle, faux autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool DANS_INTERVALLE( double valeur, double borneMin, double borneMax ) 
   {
      return (valeur >= borneMin) && (valeur <= borneMax);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool DANS_LIMITESXY( double x, double xMin, double xMax, double y, double yMin, double yMax )
   ///
   /// Cette fonction vérifie si un point 2d est dans un carré.
   ///
   /// @param[in] x    : Coordonnée x du point.
   /// @param[in] xMin : La borne inférieure en x.
   /// @param[in] xMax : La borne supérieure en x.
   /// @param[in] y    : Coordonnée y du point.
   /// @param[in] yMin : La borne inférieure en y.
   /// @param[in] yMax : La borne supérieure en y.
   ///
   /// @return Vrai si le point (x,y) est dans le carré, faux autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool DANS_LIMITESXY( double x, double xMin, double xMax,
                        double y, double yMin, double yMax ) 
   {
      return (DANS_INTERVALLE( x, xMin, xMax ) && DANS_INTERVALLE( y, yMin, yMax ));
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn double CUtilitaire::ecrasement( double a, double b )
   ///
   /// Cette fonction calcule le rapport d'écrasement pour un rectangle de
   /// longueur @a a et largeur @a b.
   ///   - si a = b, le rapport est 1
   ///   - le rapport est toujours positif et plus grand que 1.
   ///
   /// @param[in] a : Valeur à rapprocher.
   /// @param[in] b : Point limite du rapprochement.
   ///
   /// @return Le rapport @a r  de @a a /  @a b
   ///
   ////////////////////////////////////////////////////////////////////////
   double ecrasement( double a, double b )
   {
      double r = a / b;
      // Le rapport doit être toujours positif.
      if (r < 1.0)
         r = 1.0 / r;

       return r;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool fichierExiste( const std::string &nomDuFichier )
   ///
   /// Cette fonction vérifie si un fichier d'un certain nom donné existe.
   ///
   /// @param [in] nomDuFichier : Le nom du fichier dont on veut vérifier
   ///                            l'existence.
   ///
   /// @return Vrai si le fichier existe, faux autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool fichierExiste( const std::string& nomDuFichier )
   {
      std::ifstream ficin;
      ficin.open( nomDuFichier.c_str() );
      bool fail = ficin.fail();
      if ( !fail ) ficin.close();
      return !fail;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void appliquerMatrice( const Vecteur3& point, const double mat[])
   ///
   /// Cette fonction multiplie le point par une matrice de transformation
   /// donnée et retourne le résultat.
   ///
   /// @param [in] point : Le point à transformer.
   /// @param [in] mat   : La matrice de transformation.
   ///
   /// @return Le point transformé.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 appliquerMatrice( const Vecteur3& point, const double mat[] )
   {
      // Calculer la multiplication entre la matrice et le point.
      Vecteur3 pointTransforme(
         mat[0] * point[0] + mat[4] * point[1] + mat[ 8] * point[2] + mat[12],
         mat[1] * point[0] + mat[5] * point[1] + mat[ 9] * point[2] + mat[13],
         mat[2] * point[0] + mat[6] * point[1] + mat[10] * point[2] + mat[14]
      );
      const double w = mat[3] * point[0] + mat[7] * point[1] +
                       mat[11] * point[2] + mat[15];

      // Retour des coordonnées homogènes vers la 3D.
      pointTransforme /= w;

      return pointTransforme;
   }


}; // Fin de l'espace de nom utilitaire.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
