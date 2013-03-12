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

   /// Retourne le nom associé à l'usine
   inline const std::string& obtenirNom() const
   {
       return nom_;
   }
protected:
   /// Constructeur qui prend le nom associé à l'usine.
   UsineNoeud(const std::string& nom) : nom_(nom) {}

private:
   /// Le nom associé à l'usine
   std::string nom_;
};

#define DEFAULT_NODE_FACTORY_DECLARATION(FactoryName)       \
class Usine## FactoryName : public UsineNoeud              \
{                                                           \
public:                                                     \
    Usine## FactoryName(const std::string& nom);    \
    virtual NoeudAbstrait* creerNoeud() const;              \
};

#define DEFAULT_NODE_FACTORY_IMPLEMENTATION(FactoryName)                                   \
    Usine## FactoryName::Usine## FactoryName(const std::string& nom):                    \
    UsineNoeud(nom){}                                                                      \
\
    NoeudAbstrait* Usine## FactoryName::creerNoeud() const                                \
    {  return new FactoryName(obtenirNom());  }


#define NODE_FACTORY_WITH_LIMIT_DECLARATION(FactoryName,limit) \
class Usine## FactoryName : public UsineNoeud                 \
{                                                              \
public:                                                        \
    Usine## FactoryName(const std::string& nom,unsigned int maxNode = limit);              \
    virtual NoeudAbstrait* creerNoeud() const;                 \
private:                                                       \
    mutable unsigned int mNbNodeCreated;                       \
    const unsigned int MAX_NODE;                               \
};

#define NODE_FACTORY_WITH_LIMIT_IMPLEMENTATION(FactoryName)                  \
    Usine## FactoryName::Usine## FactoryName(const std::string& nom,unsigned int maxNode):      \
    UsineNoeud(nom),mNbNodeCreated(0),MAX_NODE(maxNode){}                    \
                                                                             \
    NoeudAbstrait* Usine## FactoryName::creerNoeud() const                  \
    {                                                                        \
        if(mNbNodeCreated < MAX_NODE)                                        \
        {                                                                    \
            return new FactoryName(obtenirNom(),mNbNodeCreated,MAX_NODE);  \
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
NODE_FACTORY_WITH_LIMIT_DECLARATION(NoeudMaillet,2);
NODE_FACTORY_WITH_LIMIT_DECLARATION(NoeudRondelle,1);


#endif // __ARBRE_USINES_USINENOEUD_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
