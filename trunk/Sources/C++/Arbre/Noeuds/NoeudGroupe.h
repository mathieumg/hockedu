///////////////////////////////////////////////////////////////////////////
/// @file NoeudGroupe.h
/// @author Michael Ferris
/// @date 2012-03-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////

#pragma once
#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudGroupe
/// @brief Classe qui représente un groupe de noeud (pour facilité la gestion de plusieurs noeuds en même temps).
///
/// @author Michael Ferris
/// @date 2012-03-01
///////////////////////////////////////////////////////////////////////////
class NoeudGroupe: public NoeudComposite
{
public:
    typedef NoeudComposite Super;
	/// Contructeur
	NoeudGroupe(std::string type, std::string typeEnfants = "");
	/// Destructeur
	~NoeudGroupe(void);

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudGroupe*>(n);
        return !!n2 && 
            typeEnfants_ == n2->typeEnfants_ && 
            Super::equals(n);
    }
	/// Creation du noeud XML du Groupe
	virtual XmlElement* creerNoeudXML();
	/// Initialisation du NoeudGroupe à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
	/// Accesseur de typeEnfants_
	std::string obtenirTypeEnfants() const { return typeEnfants_; }
private:
	/// Le type des éléments du groupe
	std::string typeEnfants_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

