////////////////////////////////////////////////////////////////////////////////////
/// @file CLectureFichierBinaire.h
/// @author DGI
/// @date 2005-10-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_CLECTUREFICHIERBINAIRE_H__
#define __UTILITAIRE_CLECTUREFICHIERBINAIRE_H__

///////////////////////////////////////////////////////////////////////////
/// @class CLectureFichierBinaire
/// @brief cette classe contient des méthodes > permettant de lire
///        dans un fichier binaire des variables de types string,
///        double, float, int, unsigned int, char, et bool.
///
/// @author DGI-2990
/// @date 2005-10-15
///////////////////////////////////////////////////////////////////////////
class CLectureFichierBinaire: public std::ifstream
{
public:
   /// Constructeur par défaut.
   CLectureFichierBinaire();
   /// Constructeur par paramètre.
   CLectureFichierBinaire(
      const char* nomFichier, openmode mode = std::ios::in | std::ios::binary
      );

   /// Surcharge de l'opérateur pour le type @em std::string.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, std::string& s );
   /// Surcharge de l'opérateur pour le type @em double.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, double& f );
   /// Surcharge de l'opérateur pour le type @em float.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, float& f );
   /// Surcharge de l'opérateur pour le type @em int.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, int& f );
   /// Surcharge de l'opérateur pour le type @em unsigned @em int.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, unsigned int& f );
   /// Surcharge de l'opérateur pour le type @em char.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, char& f );
   /// Surcharge de l'opérateur pour le type @em bool.
   friend CLectureFichierBinaire& operator> ( CLectureFichierBinaire& in, bool& f );

};


#endif // __UTILITAIRE_CLECTUREFICHIERBINAIRE_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
