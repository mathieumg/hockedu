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
#include "EditionEventManager.h"

class NoeudAbstrait;

///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeud
/// @brief Classe de base abstraite des usines qui seront utilisées pour
///        créer les différents noeuds de l'arbre de rendu.
///
/// @author Martin Bisson
/// @date 2007-01-28
///////////////////////////////////////////////////////////////////////////
class UsineNoeud
{
public:
   /// Destructeur vide déclaré virtuel pour les classes dérivées.
   virtual ~UsineNoeud() {}

   /// Fonction à surcharger pour la création d'un noeud.
   virtual NoeudAbstrait* creerNoeud() const = 0;

protected:
   /// Constructeur qui prend le nom associé à l'usine.
   UsineNoeud(){}

};

#define DEFAULT_NODE_FACTORY_DECLARATION(FactoryName)       \
class Usine## FactoryName : public UsineNoeud              \
{                                                           \
public:                                                     \
    Usine## FactoryName();    \
    virtual NoeudAbstrait* creerNoeud() const;              \
};

#define DEFAULT_NODE_FACTORY_IMPLEMENTATION(FactoryName)                                   \
    Usine## FactoryName::Usine## FactoryName():                    \
    UsineNoeud(){}                                                                      \
\
    NoeudAbstrait* Usine## FactoryName::creerNoeud() const                                \
    {  return new FactoryName();  }


#define NODE_FACTORY_WITH_LIMIT_DECLARATION(FactoryName,limit) \
class Usine## FactoryName : public UsineNoeud                 \
{                                                              \
public:                                                        \
    Usine## FactoryName(unsigned int maxNode = limit);              \
    virtual NoeudAbstrait* creerNoeud() const;                 \
private:                                                       \
    mutable unsigned int mNbNodeCreated;                       \
    const unsigned int MAX_NODE;                               \
};

#define NODE_FACTORY_WITH_LIMIT_IMPLEMENTATION(FactoryName)                  \
    Usine## FactoryName::Usine## FactoryName(unsigned int maxNode):      \
    UsineNoeud(),mNbNodeCreated(0),MAX_NODE(maxNode){}                    \
                                                                             \
    NoeudAbstrait* Usine## FactoryName::creerNoeud() const                  \
    {                                                                        \
        if(mNbNodeCreated < MAX_NODE)                                        \
        {                                                                    \
            return new FactoryName(mNbNodeCreated,MAX_NODE);  \
        }                                                                    \
        return NULL;                                                         \
    }                                                                        \

DEFAULT_NODE_FACTORY_DECLARATION(NoeudGroupe);
DEFAULT_NODE_FACTORY_DECLARATION(NoeudAccelerateur);
DEFAULT_NODE_FACTORY_DECLARATION(NoeudMuret);
DEFAULT_NODE_FACTORY_DECLARATION(NoeudPortail);
DEFAULT_NODE_FACTORY_DECLARATION(NoeudTable);
DEFAULT_NODE_FACTORY_DECLARATION(NodeWallEdition);
DEFAULT_NODE_FACTORY_DECLARATION(NodeControlPoint);
DEFAULT_NODE_FACTORY_DECLARATION(NodePolygone);
DEFAULT_NODE_FACTORY_DECLARATION(NodeBonus);
NODE_FACTORY_WITH_LIMIT_DECLARATION(NoeudMaillet,EditionEventManager::mEditionLimitMallet);
NODE_FACTORY_WITH_LIMIT_DECLARATION(NoeudRondelle,EditionEventManager::mEditionLimitPucks);


#endif // __ARBRE_USINES_USINENOEUD_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
