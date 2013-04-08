///////////////////////////////////////////////////////////////////////////////
/// @file Utilitaire.cpp
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#if BOX2D_INTEGRATED
#include <Box2D/Box2D.h>
#endif

#if WIN32
#define _WINSOCKAPI_
#include <Windows.h>
#endif

#include "Utilitaire.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdarg.h>

namespace utilitaire {

    DisplayMessageCallback mDisplayMessageCallback = NULL;

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void afficherErreur(const std::string& message)
   ///
   /// Cette fonction affiche une message d'erreur.
   ///
   /// @param[in] message : La cha�ne d�crivant l'erreur.
   ///
   /// @return Aucune
   ///
   ////////////////////////////////////////////////////////////////////////
   void afficherErreur(const std::string& message)
   {
       if(mDisplayMessageCallback)
       {
           mDisplayMessageCallback(message.c_str());
       }
       else
       {
#ifdef WIN32
           MessageBoxA(0, message.c_str(), "Erreur", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#else
           std::cerr << message << std::endl;
#endif
       }
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool EGAL_ZERO(float nombre)
   ///
   /// Fonction pour savoir si un float est �gal � z�ro.
   ///
   /// @param[in] nombre : Valeur � v�rifier.
   ///
   /// @return true si nombre est �gal a 0, false autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool EGAL_ZERO(float nombre)
   {
      return (nombre < EPSILON) && (-nombre < EPSILON);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float RAD_TO_DEG(float angleRad)
   ///
   /// Cette fonction convertit les angles de radians en degr�s.
   ///
   /// @param[in] angleRad : L'angle en radians � convertir.
   ///
   /// @return L'angle converti en degr�s
   ///
   ////////////////////////////////////////////////////////////////////////
   float RAD_TO_DEG(float angleRad)
   {
      return angleRad * (float)57.2957795130823208767981548141052;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float DEG_TO_RAD(float angleDeg)
   ///
   /// Cette fonction convertit les angles de degr�s en radians.
   ///
   /// @param[in] angleDeg : L'angle en degr�s � convertir.
   ///
   /// @return L'angle converti en radians.
   ///
   ////////////////////////////////////////////////////////////////////////
   float DEG_TO_RAD(float angleDeg)
   {
      return angleDeg * (float)0.0174532925199432957692369076848861;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float KMH_TO_MS( float kmh )
   ///
   /// Fonction de conversion des km/h en m/s.
   ///
   /// @param[in] kmh : Valeur en kilom�tres / heure.
   ///
   /// @return float => vitesse en m�tres par seconde.
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
   /// @param[in] ms : Valeur en m�tres / seconde.
   ///
   /// @return float => vitesse en kilom�tres / heure.
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
   /// Fonction pour conna�tre le signe d'un nombre.
   ///
   /// @param[in] nombre : Valeur dont on veut trouver le signe.
   ///
   /// @return int => 1 si positif, -1 si n�gatif, 0 si = 0.
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
   float ABSOLUTE( float nombre )
   {
      return (SIGN(nombre) * nombre);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool DANS_INTERVALLE( float valeur, float borneMin, float borneMax )
   ///
   /// Cette fonction v�rifie si une valeur est dans un intervalle.
   ///
   /// @param[in] valeur   : La valeur � v�rifier.
   /// @param[in] borneMin : La borne inf�rieure de l'intervalle.
   /// @param[in] borneMax : La borne sup�rieure de l'intervalle.
   ///
   /// @return Vrai si la valeur est dans l'intervalle, faux autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool DANS_INTERVALLE( float valeur, float borneMin, float borneMax )
   {
      return (valeur >= borneMin) && (valeur <= borneMax);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool DANS_LIMITESXY( float x, float xMin, float xMax, float y, float yMin, float yMax )
   ///
   /// Cette fonction v�rifie si un point 2d est dans un carr�.
   ///
   /// @param[in] x    : Coordonn�e x du point.
   /// @param[in] xMin : La borne inf�rieure en x.
   /// @param[in] xMax : La borne sup�rieure en x.
   /// @param[in] y    : Coordonn�e y du point.
   /// @param[in] yMin : La borne inf�rieure en y.
   /// @param[in] yMax : La borne sup�rieure en y.
   ///
   /// @return Vrai si le point (x,y) est dans le carr�, faux autrement.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool DANS_LIMITESXY( float x, float xMin, float xMax,
                        float y, float yMin, float yMax )
   {
      return (DANS_INTERVALLE( x, xMin, xMax ) && DANS_INTERVALLE( y, yMin, yMax ));
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float CUtilitaire::ecrasement( float a, float b )
   ///
   /// Cette fonction calcule le rapport d'�crasement pour un rectangle de
   /// longueur @a a et largeur @a b.
   ///   - si a = b, le rapport est 1
   ///   - le rapport est toujours positif et plus grand que 1.
   ///
   /// @param[in] a : Valeur � rapprocher.
   /// @param[in] b : Point limite du rapprochement.
   ///
   /// @return Le rapport @a r  de @a a /  @a b
   ///
   ////////////////////////////////////////////////////////////////////////
   float ecrasement( float a, float b )
   {
      float r = a / b;
      // Le rapport doit �tre toujours positif.
      if (r < 1.0f)
         r = 1.0f / r;

       return r;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool fichierExiste( const std::string &nomDuFichier )
   ///
   /// Cette fonction v�rifie si un fichier d'un certain nom donn� existe.
   ///
   /// @param [in] nomDuFichier : Le nom du fichier dont on veut v�rifier
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
   /// @fn void appliquerMatrice( const Vecteur3& point, const float mat[])
   ///
   /// Cette fonction multiplie le point par une matrice de transformation
   /// donn�e et retourne le r�sultat.
   ///
   /// @param [in] point : Le point � transformer.
   /// @param [in] mat   : La matrice de transformation.
   ///
   /// @return Le point transform�.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 appliquerMatrice( const Vecteur3& point, const float mat[] )
   {
      // Calculer la multiplication entre la matrice et le point.
      Vecteur3 pointTransforme(
         mat[0] * point[0] + mat[4] * point[1] + mat[ 8] * point[2] + mat[12],
         mat[1] * point[0] + mat[5] * point[1] + mat[ 9] * point[2] + mat[13],
         mat[2] * point[0] + mat[6] * point[1] + mat[10] * point[2] + mat[14]
      );
      const float w = mat[3] * point[0] + mat[7] * point[1] +
                       mat[11] * point[2] + mat[15];

      // Retour des coordonn�es homog�nes vers la 3D.
      pointTransforme /= w;

      return pointTransforme;
   }

#if BOX2D_INTEGRATED
   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VEC3_TO_B2VEC( const Vecteur3& pVector, class b2Vec2& pB2vector )
   ///
   /// /*Description*/
   ///
   /// @param[in] const Vecteur3 & pVector
   /// @param[in] class b2Vec2 & pB2vector
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   void VEC3_TO_B2VEC( const Vecteur3& pVector, b2Vec2& pB2vector )
   {

       pB2vector.x = (float32)pVector[VX]*ratioWorldToBox2D;
       pB2vector.y = (float32)pVector[VY]*ratioWorldToBox2D;
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void B2VEC_TO_VEC3( Vecteur3& pVector, const struct b2Vec2& pB2vector )
   ///
   /// /*Description*/
   ///
   /// @param[in] Vecteur3 & pVector
   /// @param[in] const struct b2Vec2 & pB2vector
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   void B2VEC_TO_VEC3( Vecteur3& pVector, const struct b2Vec2& pB2vector )
   {
       pVector[VX] = pB2vector.x/ratioWorldToBox2D;
       pVector[VY] = pB2vector.y/ratioWorldToBox2D;
   }
#endif

}; // Fin de l'espace de nom utilitaire.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
