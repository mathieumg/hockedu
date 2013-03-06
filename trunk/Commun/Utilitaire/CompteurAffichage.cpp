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
   /// classe.  Si cette instance n'a pas �t� cr��e, elle la cr�e.  Cette
   /// cr�ation n'est toutefois pas n�cessairement "thread-safe", car
   /// aucun verrou n'est pris entre le test pour savoir si l'instance
   /// existe et le moment de sa cr�ation.
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
   /// Cette fonction lib�re l'instance unique de cette classe.
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
   /// Cette fonction effectue le traitement n�cessaire lorsqu'un affichage
   /// est signal�e, c'est-�-dire qu'elle incr�mente le compte et v�rifie
   /// si la limite de temps pour la mise � jour est d�pass�e.
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
   /// Cette fonction r�initialise le compteur d'affichage � son �tat
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
   /// Ce constructeur par d�faut ne fait qu'initialiser les membres de
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
   /// Ce destructeur ne fait rien, mais est quand m�me d�clar� afin de
   /// pouvoir �tre d�clar� priv� pour l'implantation du patron singleton.
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
