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
/// @brief Classe qui repr�sente un groupe de noeud (pour facilit� la gestion de plusieurs noeuds en m�me temps).
///
/// @author Michael Ferris
/// @date 2012-03-01
///////////////////////////////////////////////////////////////////////////
class NoeudGroupe: public NoeudComposite
{
public:
    typedef NoeudComposite Super;
	/// Contructeur
    NoeudGroupe( const RazerKey typeEnfants = RAZER_KEY_NONE );
	/// Destructeur
	virtual ~NoeudGroupe(void);

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudGroupe*>(n);
        return !!n2 && 
            typeEnfants_ == n2->typeEnfants_ && 
            Super::equals(n);
    }
	/// Creation du noeud XML du Groupe
	virtual XmlElement* createXmlNode();
	/// Initialisation du NoeudGroupe � partir d'un element XML
	virtual bool initFromXml(const XmlElement* element);
	/// Accesseur de typeEnfants_
    RazerKey obtenirTypeEnfants( ) const
    {
        return typeEnfants_;
    }
private:
	/// Le type des �l�ments du groupe
	RazerKey typeEnfants_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

