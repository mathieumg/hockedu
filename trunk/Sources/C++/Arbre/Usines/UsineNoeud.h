///////////////////////////////////////////////////////////////////////////////
/// @file UsineNoeud.h
/// @author Martin Bisson
/// @date 2007-01-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_USINENOEUD_H__
#define __ARBRE_USINES_USINENOEUD_H__


#include <string>


class NoeudAbstrait;


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeud
/// @brief Classe de base abstraite des usines qui seront utilis�es pour
///        cr�er les diff�rents noeuds de l'arbre de rendu.
///
/// @author Martin Bisson
/// @date 2007-01-28
///////////////////////////////////////////////////////////////////////////
class UsineNoeud
{
public:
   /// Destructeur vide d�clar� virtuel pour les classes d�riv�es.
   inline virtual ~UsineNoeud() {}

   /// Fonction � surcharger pour la cr�ation d'un noeud.
   virtual NoeudAbstrait* creerNoeud() const = 0;

   /// Retourne le nom associ� � l'usine
   inline const std::string& obtenirNom() const;


protected:
   /// Constructeur qui prend le nom associ� � l'usine.
   UsineNoeud(const std::string& nom) : nom_(nom) {}


private:
   /// Le nom associ� � l'usine
   std::string nom_;


};




////////////////////////////////////////////////////////////////////////
///
/// @fn inline const std::string& UsineNoeud::obtenirNom() const
///
/// Cette fonction retourne une cha�ne repr�sentante le nom associ� �
/// l'usine.
///
/// @return Le nom associ� � l'usine.
///
////////////////////////////////////////////////////////////////////////
inline const std::string& UsineNoeud::obtenirNom() const
{
   return nom_;
}


#endif // __ARBRE_USINES_USINENOEUD_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
