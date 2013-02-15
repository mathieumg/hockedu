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
#include "NoeudGroupe.h"
#include "NoeudAccelerateur.h"
#include "NoeudMaillet.h"
#include "NoeudMuret.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"

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

#define CreateNodeFactory(FactoryName)                                       \
class Usine##FactoryName## : public UsineNoeud                               \
{                                                                            \
public:                                                                      \
    inline Usine##FactoryName##(const std::string& nom):UsineNoeud(nom){}    \
    virtual NoeudAbstrait* creerNoeud() const                                \
    {  return new FactoryName##(obtenirNom());  }                            \
};                                                                           \

CreateNodeFactory(NoeudGroupe);
CreateNodeFactory(NoeudAccelerateur);
CreateNodeFactory(NoeudMuret);
CreateNodeFactory(NoeudPortail);
CreateNodeFactory(NoeudTable);

class UsineNoeudMaillet : public UsineNoeud
{
public:
    /// Constructeur par param�tres.
    inline UsineNoeudMaillet(const std::string& nom):UsineNoeud(nom){}

    /// Fonction � surcharger pour la cr�ation d'un noeud.
    inline virtual NoeudAbstrait* creerNoeud() const
    {
        if(UsineNoeudMaillet::bypassLimitePourTest || NoeudMaillet::mailletExistant < 2)
        {
            return new NoeudMaillet(obtenirNom());
        }
        return 0;
    }

    static bool bypassLimitePourTest; //D�finie dans NoeudMaillet.cpp
};

class UsineNoeudRondelle : public UsineNoeud
{
public:
    /// Constructeur par param�tres.
    inline UsineNoeudRondelle(const std::string& nom):UsineNoeud(nom){}

    /// Fonction � surcharger pour la cr�ation d'un noeud.
    inline virtual NoeudAbstrait* creerNoeud() const
    {
        if(UsineNoeudRondelle::bypassLimitePourTest || NoeudRondelle::rondellesPresentes < 1)
        {
            return new NoeudRondelle(obtenirNom());
        }
        return 0;
    }

    static bool bypassLimitePourTest; //D�finie dans NoeudMaillet.cpp
};


#endif // __ARBRE_USINES_USINENOEUD_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
