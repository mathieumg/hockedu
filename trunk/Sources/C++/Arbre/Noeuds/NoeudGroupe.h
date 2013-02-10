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
	/// Contructeur
	NoeudGroupe(std::string type, std::string typeEnfants = "");
	/// Destructeur
	~NoeudGroupe(void);

	/// Creation du noeud XML du Groupe
	virtual TiXmlElement* creerNoeudXML();
	/// Initialisation du NoeudGroupe � partir d'un element XML
	virtual bool initialiser(const TiXmlElement* element);
	/// Accesseur de typeEnfants_
	std::string obtenirTypeEnfants() const { return typeEnfants_; }
private:
	/// Le type des �l�ments du groupe
	std::string typeEnfants_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

