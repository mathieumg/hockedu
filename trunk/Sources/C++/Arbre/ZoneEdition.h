///////////////////////////////////////////////////////////////////////////////
/// @file ZoneEdition.h
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once

class TiXmlElement;
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
	void modifierLimitesExt(const double* boiteEnglobantTable, const double& longueur,const double& largeur);

	/// Creation du noeud XML de la zone d'édition
	TiXmlElement* creerNoeudXML();
	/// Initialistion de la zone d'édition à partir d'un noeud XML
	bool initialisationXML(const TiXmlElement*);
	/// Remet la zone d'edition a son etat par defaut
	void reinitialiser();
	/// Effectue l'affichage de la zone d'edition
	void afficher();
private:
	/// Attribut de la zone d'édition
	double limiteIntLongueur_;
	double limiteIntLargeur_;
	double limiteExtLargeur_; 
	double limiteExtLongueur_;
	static const double DEFAUT_LIMITE_INT_LONGUEUR;
	static const double DEFAUT_LIMITE_INT_LARGEUR;
	static const double DEFAUT_LIMITE_EXT_LARGEUR; 
	static const double DEFAUT_LIMITE_EXT_LONGUEUR;

/// Accesseurs
public:
	/// Accesseur de limiteIntLongueur_
	double obtenirLimiteIntLongueur() const { return limiteIntLongueur_; }
	/// Modificateur de limiteIntLongueur_
	void modifierLimiteIntLongueur(double val) { limiteIntLongueur_ = val; }
	/// Accesseur de limiteIntLargeur_
	double obtenirLimiteIntLargeur() const { return limiteIntLargeur_; }
	/// Modificateur de limiteIntLargeur_
	void modifierLimiteIntLargeur(double val) { limiteIntLargeur_ = val; }
	/// Accesseur de limiteExtLargeur_
	double obtenirLimiteExtLargeur() const { return limiteExtLargeur_; }
	/// Modificateur de limiteExtLargeur_
	void modifierLimiteExtLargeur(double val) { limiteExtLargeur_ = val; }
	/// Accesseur de limiteExtLongueur_
	double obtenirLimiteExtLongueur() const { return limiteExtLongueur_; }
	/// Modificateur de limiteExtLongueur_
	void modifierLimiteExtLongueur(double val) { limiteExtLongueur_ = val; }
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




