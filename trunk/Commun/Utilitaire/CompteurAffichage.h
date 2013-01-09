//////////////////////////////////////////////////////////////////////////////
/// @file CompteurAffichage.h
/// @author Martin Bisson
/// @date 2007-03-09
/// @version 1.0 
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_COMPTEURAFFICHAGE_H__
#define __UTILITAIRE_COMPTEURAFFICHAGE_H__


namespace utilitaire {
   

   ///////////////////////////////////////////////////////////////////////////
   /// @class CompteurAffichage
   /// @brief Classe qui gère le compte des affichages par secondes ("FPS").
   ///
   /// @author Martin Bisson
   /// @date 2007-03-09
   ///////////////////////////////////////////////////////////////////////////
   class CompteurAffichage
   {
   public:
      /// Obtient l'instance unique de la classe.
      static CompteurAffichage* obtenirInstance();
      /// Libère l'instance unique de la classe.
      static void libererInstance();

      /// Obtient le dernier nombre calculé d'affichages par seconde.
      inline int obtenirAffichagesSeconde() const;

      /// Indique qu'un affichage vient de se produire.
      void signalerAffichage();
      /// Réinitialise le compteur d'affichage.
      void reinitialiser();

   private:
      /// Constructeur par défaut.
      CompteurAffichage();
      /// Destructeur.
      ~CompteurAffichage();
      /// Constructeur copie.  Déclaré mais non défini pour éviter qu'il soit
      /// généré par le compilateur.
      CompteurAffichage(const CompteurAffichage&);
      /// Opérateur d'assignation.  Déclaré mais non défini pour éviter qu'il
      /// soit généré par le compilateur.
      CompteurAffichage& operator =(const CompteurAffichage&);

      /// Pointeur vers l'instance unique de la classe.
      static CompteurAffichage* instance_;

      /// Nombre de millisecondes entre les rafraichissements du nombre d'affichages.
      static const int TEMPS_RAFRAICHISSEMENT = 1000;

      /// Nombre d'affichages dans la dernière seconde.
      int nbAffichagesSeconde_;

      /// Dernière lecture du temps.
      int derniereLecture_;

      /// Compte des affichages.
      int compte_;

   };




   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline int CompteurAffichage::obtenirAffichagesSeconde() const
   ///
   /// Cette fonction retourne le dernier nombre d'affichages par seconde
   /// calculé par le compteur.
   ///
   /// @return Le nombre d'affichages par seconce le plus récent.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline int CompteurAffichage::obtenirAffichagesSeconde() const
   {
      return nbAffichagesSeconde_;
   }


} // Fin de l'espace de nom utilitaire.


#endif // __UTILITAIRE_COMPTEURAFFICHAGE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
