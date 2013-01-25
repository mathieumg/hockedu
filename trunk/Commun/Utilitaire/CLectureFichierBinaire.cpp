////////////////////////////////////////////////////////////////////////////////////
/// @file CLectureFichierBinaire.cpp
/// @author DGI
/// @date 2005-10-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "CLectureFichierBinaire.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn CLectureFichierBinaire::CLectureFichierBinaire()
///
/// Constructeur par défaut qui ne fait rien.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire::CLectureFichierBinaire()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn CLectureFichierBinaire::CLectureFichierBinaire(const char* nomFichier, openmode mode)
///
/// Constructeur par paramètres: ouvre un fichier binaire.
///
/// @param[in] nomFichier : Le nom du fichier.
/// @param[in] mode       : Le mode.
///
/// @return Aucune
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire::CLectureFichierBinaire (const char* nomFichier, openmode mode)
   : std::ifstream ( nomFichier, mode )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, std::string& s)
///
/// Surcharge de l'opérateur > pour une variable de type @a std::string.
///
/// @param[in]  in : L'objet représentant le fichier binaire.
/// @param[out] s  : La variable de type @a std:string.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, std::string& s )
{
   int size;
   in > size;

   char* ptr = new char[size + 1];

   in.read( ptr, sizeof ( char ) *size );
   ptr[size] = 0;

   s = ptr;

   delete[] ptr;

   return in;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, float& f)
///
/// Surcharge de l'opérateur >  pour une variable de type @a float.
///
/// @param[in]  in : objet représentant le fichier binaire.
/// @param[out] f  : La variable de type @a float
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, float& f )
{
   in.read( ( char* ) &f, sizeof ( float ) );
   return in;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, double& f)
///
/// Surcharge de l'opérateur > pour une variable de type @a float.
///
/// @param[in]  in : L'objet représentant le fichier binaire.
/// @param[out] f  : La variable de type @a float.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, double& f )
{
   in.read( ( char* ) &f, sizeof ( double ) );
   return in;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, int& f)
///
/// Surcharge de l'opérateur > pour une variable de type @a int.
///
/// @param[in]  in : L'objet représentant le fichier binaire.
/// @param[out] f  : La variable de type @a int.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, int& f )
{
   in.read( ( char* ) &f, sizeof ( int ) );
   return in;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, unsigned int& f)
///
/// Surcharge de l'opérateur > pour une variable de type @a unsigned @a int.
///
/// @param[in]  in : L'objet représentant le fichier binaire.
/// @param[out] f  : La variable de type @a unsigned @a int.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, unsigned int& f )
{
   in.read( ( char* ) &f, sizeof ( unsigned int ) );
   return in;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, char& f)
///
/// Surcharge de l'opérateur > pour une variable de type @a char.
///
/// @param[in]  in : L'objet représentant le fichier binaire.
/// @param[out] f  : La variable de type @a char.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, char& f )
{
   in.read( ( char* ) &f, sizeof ( char ) );
   return in;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn operator> ( CLectureFichierBinaire& in, bool& f)
///
/// Surcharge de l'opérateur > pour une variable de type @a bool.
///
/// @param[in]  in : L'objet représentant le fichier binaire.
/// @param[out] f  : La variable de type @a bool.
///
/// @return L'objet représentant le fichier binaire.
///
////////////////////////////////////////////////////////////////////////
CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, bool& f )
{
   in.read( ( char* ) &f, sizeof ( bool ) );
   return in;
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
