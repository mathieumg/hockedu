////////////////////////////////////////////////////////////////////////////////////
/// @file CEcritureFichierBinaire.h
/// @author DGI
/// @date 2005-10-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_CECRITUREFICHIERBINAIRE_H__
#define __UTILITAIRE_CECRITUREFICHIERBINAIRE_H__


///////////////////////////////////////////////////////////////////////////
/// @class CEcritureFichierBinaire
/// @brief Cette classe contient des m�thodes < permettant d'�crire
///        dans un fichier binaire des variables string, float,
///        float, int, unsigned int, char, bool.
///
/// @author DGI-2990
/// @date 2005-10-15
///////////////////////////////////////////////////////////////////////////
class CEcritureFichierBinaire: public std::ofstream
{
public:
   /// Constructeur par d�faut.
   CEcritureFichierBinaire();
   /// Constructeur par param�tre.
   CEcritureFichierBinaire(
      const char* nomFichier, openmode mode = std::ios::out | std::ios::binary
      );

   /// Surcharge de l'op�rateur pour le type @em std::string.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const std::string& s );
   /// Surcharge de l'op�rateur pour le type @em float.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const float& x );
   /// Surcharge de l'op�rateur pour le type @em float.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const float& x );
   /// Surcharge de l'op�rateur pour le type @em int.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const int& x );
   /// Surcharge de l'op�rateur pour le type @em unsigned @em int.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const unsigned int& x );
   /// Surcharge de l'op�rateur pour le type @em char.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const char& x );
   /// Surcharge de l'op�rateur pour le type @em bool.
   friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const bool& x );

   /// Fonction pour ins�rer des caract�res vides dans le fichier.
   void null(int n);

};


#endif // __UTILITAIRE_CECRITUREFICHIERBINAIRE_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
