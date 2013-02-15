///////////////////////////////////////////////////////////////////////////////
/// @file ZoneEdition.h
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once

class XmlElement;
class TerrainTest;


///////////////////////////////////////////////////////////////////////////
/// @class ZoneEdition
/// @brief Objet contenant les propriété de la zone d'édition d'un terrain
///
/// @author Michael Ferris
/// @date 2012-03-19
///////////////////////////////////////////////////////////////////////////
class ZoneEdition
{
public:
	friend TerrainTest;
	ZoneEdition();

	/// Permet de modifier les limites de la zone d'edition de facon relative à une table
	void modifierLimitesExt(const float* boiteEnglobantTable, const float& longueur,const float& largeur);

	/// Creation du noeud XML de la zone d'édition
	XmlElement* creerNoeudXML();
	/// Initialistion de la zone d'édition à partir d'un noeud XML
	bool initialisationXML(const XmlElement*);
	/// Remet la zone d'edition a son etat par defaut
	void reinitialiser();
	/// Effectue l'affichage de la zone d'edition
	void afficher();
private:
	/// Attribut de la zone d'édition
	float limiteIntLongueur_;
	float limiteIntLargeur_;
	float limiteExtLargeur_; 
	float limiteExtLongueur_;
	static const float DEFAUT_LIMITE_INT_LONGUEUR;
	static const float DEFAUT_LIMITE_INT_LARGEUR;
	static const float DEFAUT_LIMITE_EXT_LARGEUR; 
	static const float DEFAUT_LIMITE_EXT_LONGUEUR;

/// Accesseurs
public:
	/// Accesseur de limiteIntLongueur_
	float obtenirLimiteIntLongueur() const { return limiteIntLongueur_; }
	/// Modificateur de limiteIntLongueur_
	void modifierLimiteIntLongueur(float val) { limiteIntLongueur_ = val; }
	/// Accesseur de limiteIntLargeur_
	float obtenirLimiteIntLargeur() const { return limiteIntLargeur_; }
	/// Modificateur de limiteIntLargeur_
	void modifierLimiteIntLargeur(float val) { limiteIntLargeur_ = val; }
	/// Accesseur de limiteExtLargeur_
	float obtenirLimiteExtLargeur() const { return limiteExtLargeur_; }
	/// Modificateur de limiteExtLargeur_
	void modifierLimiteExtLargeur(float val) { limiteExtLargeur_ = val; }
	/// Accesseur de limiteExtLongueur_
	float obtenirLimiteExtLongueur() const { return limiteExtLongueur_; }
	/// Modificateur de limiteExtLongueur_
	void modifierLimiteExtLongueur(float val) { limiteExtLongueur_ = val; }
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




