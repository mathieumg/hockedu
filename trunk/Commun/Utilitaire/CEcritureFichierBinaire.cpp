////////////////////////////////////////////////////////////////////////////////////
/// @file CEcritureFichierBinaire.cpp
/// @author DGI
/// @date 2005-10-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "CEcritureFichierBinaire.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn CEcritureFichierBinaire::CEcritureFichierBinaire()
///
/// Constructeur par défaut qui ne fait rien.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire::CEcritureFichierBinaire()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn CEcritureFichierBinaire::CEcritureFichierBinaire(const char* nomFichier, openmode mode)
///
/// Constructeur par paramètres: ouvre un fichier binaire.
///
/// @param[in] nomFichier : Le nom du fichier.
/// @param[in] mode       : Le mode.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire::CEcritureFichierBinaire(const char* nomFichier, openmode mode)
   : std::ofstream( nomFichier, mode )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const std::string& s )
///
/// Surcharge de l'opérateur < pour une variable de type @a std::string.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] s   : La variable de type @a std::string.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const std::string& s )
{
   out < int ( s.size() );
   out.write( s.c_str(), sizeof ( char ) * int ( s.size() ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const float& x )
///
/// Surcharge de l'opérateur < pour une variable de type @a float.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] x   : La variable de type @a float.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const float& x )
{
   out.write( ( char* ) &x, sizeof ( float ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const double& x )
///
/// Surcharge de l'opérateur < pour une variable de type @a float.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] x   : La variable de type @a float.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const double& x )
{
   out.write( ( char* ) &x, sizeof ( double ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const int& x )
///
/// Surcharge de l'opérateur < pour une variable de type @a int.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] x   : La variable de type @a int.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const int& x )
{
   out.write( ( char* ) &x, sizeof ( int ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const unsigned int& x )
///
/// Surcharge de l'opérateur < pour une variable de type @a unsigned @a int.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] x   : La variable de type @a unsigned @a int.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const unsigned int& x )
{
   out.write( ( char* ) &x, sizeof ( unsigned int ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const char& x )
///
/// Surcharge de l'opérateur < pour une variable de type @a char.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] x   : La variable de type @a char.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const char& x )
{
   out.write( ( char* ) &x, sizeof ( char ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator< ( CEcritureFichierBinaire& out, const bool& x )
///
/// Surcharge de l'opérateur < pour une variable de type @a bool.
///
/// @param[in] out : L'objet représentant le fichier binaire.
/// @param[in] x   : La variable de type @a bool.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const bool& x )
{
   out.write( ( char* ) &x, sizeof ( bool ) );
   return out;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn CEcritureFichierBinaire::null(int n)
///
/// Cette fonction permet d'insérer des caractèrs nuls (0) dans le fichier
/// binaire.
///
/// @param[in] n : Le nombre de caractères vides à insérer.
///
///  @return Aucune
///
////////////////////////////////////////////////////////////////////////
void CEcritureFichierBinaire::null(int n)
{
   char* buffer = new char[n];
   memset( buffer, 0, n );
   write( buffer, n );
   delete[] buffer;
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
