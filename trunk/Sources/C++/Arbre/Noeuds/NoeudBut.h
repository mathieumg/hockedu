///////////////////////////////////////////////////////////////////////////
/// @file NoeudBut.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDBUT_H__
#define __ARBRE_NOEUDS_NOEUDBUT_H__

#include "NoeudComposite.h"

class NoeudMuretRelatif;
class NoeudPoint;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudBut
/// @brief Classe qui repr�sente un but dans lequel le joueur doit compter.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudBut : public NoeudComposite
{
public:
	friend NoeudMuretRelatif;
	/// Constructeur � partir du type du noeud.
	NoeudBut(const std::string& typeNoeud, int joueur, NoeudPoint * coinHaut, NoeudPoint * coinBas);
	/// Destructeur.
	~NoeudBut();

	/// Affiche le cube.
	virtual void afficherConcret() const;
	/// Effectue l'animation
	virtual void animer( const float& temps );
	/// Accueil un visiteur
	virtual void accueillirVisiteurNoeud( VisiteurNoeud& v);
	/// Pour savoir � quel joueur le but appartient
	int obtenirJoueur() const;
	/// Retourne la zone occupee par la table
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Retourne le rayon du noeud
	void updateLongueur(double facteurModificationEchelle = 1);
	/// Modifie le pointeur sur le but adverse
	void modifierButAdverse(NoeudBut* butAdverse);
	NoeudBut* obtenirButAdverse();

	/// Permet de connaitre la hauteur des buts pour savoir si la rondelle peut y passer
	double obtenirHauteurBut();

	/// Permet d'assigner les attribut n�cessaire � la collision
	virtual void assignerAttributVisiteurCollision(VisiteurCollision* v);

	/// Oermet d'obtenir le rayon du but
	virtual double obtenirRayon();

	/// Creation du noeud XML du but
	virtual TiXmlElement* creerNoeudXML();
	/// Initialisation du But � partir d'un element XML
	virtual bool initialiser(const TiXmlElement* element);

	static double longueurBut_;
	/// Accesseur de positionBas_
	inline Vecteur3 obtenirPositionBas() const { return positionBas_; }
	/// Accesseur de positionHaut_
	inline Vecteur3 obtenirPositionHaut() const { return positionHaut_; }
private:
	/// Attribut disant si le but est au joueur1 ou joueur 2 par un int, 1 ou 2
	int joueur_;
	/// Pointeur sur le coin en haut
	NoeudPoint* coinHaut_;
	/// Pointeur sur le coin en bas
	NoeudPoint* coinBas_;
	/// Longueur du but
	const double longueurButBase_;
	/// Pointeur sur le but adverse
	NoeudBut* butAdverse_;
	/// Position du point ext�rieur du but
	mutable Vecteur3 positionHaut_, positionBas_;
	
};


#endif // __ARBRE_NOEUDS_NOEUDBUT_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
