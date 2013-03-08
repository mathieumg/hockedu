//////////////////////////////////////////////////////////////////////////////
/// @file CompteurAffichage.cpp
/// @author Martin Bisson
/// @date 2007-03-09
/// @version 1.0 
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "CompteurAffichage.h"
#if WIN32
#define _WINSOCKAPI_
#include <windows.h>
#endif

namespace utilitaire {


   /// Pointeur vers l'instance unique de la classe.
   CompteurAffichage* CompteurAffichage::instance_ = 0;


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn CompteurAffichage* CompteurAffichage::obtenirInstance()
   ///
   /// Cette fonction retourne un pointeur vers l'instance unique de la
   /// classe.  Si cette instance n'a pas été créée, elle la crée.  Cette
   /// création n'est toutefois pas nécessairement "thread-safe", car
   /// aucun verrou n'est pris entre le test pour savoir si l'instance
   /// existe et le moment de sa création.
   ///
   /// @return Un pointeur vers l'instance unique de cette classe.
   ///
   ////////////////////////////////////////////////////////////////////////
   CompteurAffichage* CompteurAffichage::obtenirInstance()
   {
      if (instance_ == 0)
         instance_ = new CompteurAffichage;

      return instance_;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void CompteurAffichage::libererInstance()
   ///
   /// Cette fonction libère l'instance unique de cette classe.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void CompteurAffichage::libererInstance()
   {
      delete instance_;
      instance_ = 0;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void CompteurAffichage::signalerAffichage()
   ///
   /// Cette fonction effectue le traitement nécessaire lorsqu'un affichage
   /// est signalée, c'est-à-dire qu'elle incrémente le compte et vérifie
   /// si la limite de temps pour la mise à jour est dépassée.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void CompteurAffichage::signalerAffichage()
   {
      compte_++;

      const int difference = GetTickCount() - derniereLecture_;
      if (difference >= TEMPS_RAFRAICHISSEMENT) {
         const float compte = (1000.0f * compte_) / ((float) difference);

         nbAffichagesSeconde_ = static_cast<int>(compte);
         derniereLecture_    += difference;
         compte_              = 0;
      }
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void CompteurAffichage::reinitialiser()
   ///
   /// Cette fonction réinitialise le compteur d'affichage à son état
   /// initiale.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void CompteurAffichage::reinitialiser()
   {
      nbAffichagesSeconde_ = 0;
      derniereLecture_     = GetTickCount();
      compte_              = 0;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn CompteurAffichage::CompteurAffichage()
   ///
   /// Ce constructeur par défaut ne fait qu'initialiser les membres de
   /// du compteur.
   ///
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   CompteurAffichage::CompteurAffichage()
   {
      reinitialiser();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn CompteurAffichage::~CompteurAffichage()
   ///
   /// Ce destructeur ne fait rien, mais est quand même déclaré afin de
   /// pouvoir être déclaré privé pour l'implantation du patron singleton.
   ///
   /// @return Aucune (destructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   CompteurAffichage::~CompteurAffichage()
   {
   }


} // Fin de l'espace de nom utilitaire.


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
