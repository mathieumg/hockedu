///////////////////////////////////////////////////////////////////////////
/// @file NoeudPoint.h
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-02-06
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "NoeudComposite.h"
#include "OnceVector.h"
#include <set>
#include "PositionSubject.h"

enum TypePosPoint{
    POSITION_HAUT_GAUCHE   , 
    POSITION_HAUT_MILIEU   , 
    POSITION_HAUT_DROITE   , 
    POSITION_MILIEU_GAUCHE , 
    POSITION_MILIEU_DROITE , 
    POSITION_BAS_GAUCHE    , 
    POSITION_BAS_MILIEU    , 
    POSITION_BAS_DROITE    ,
};
// #define LIMITE_IN 20
// #define LIMITE_OUT 150

typedef Vecteur3D<float*> Vecteur3pf;

typedef OnceVector<Vecteur3pf> GroupeTripleAdresseFloat;
typedef std::vector<Vecteur3i> GroupeCoord;

class NoeudMuretRelatif;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudPoint
/// @brief Classe qui représente les points déplacable de l'aire de jeu.
///
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudPoint : public NoeudComposite, public PositionSubject
{
public:
	friend NoeudMuretRelatif;
	/// Constructeur à partir du type du noeud.
	NoeudPoint( const std::string& typeNoeud, float coordX, float coordY, TypePosPoint typePosNoeud);
	/// Destructeur.
	~NoeudPoint();

	/// Affiche le cube.
	virtual void afficherConcret() const;
	/// Effectue l'animation
	virtual void animer( const float& temps );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);
	/// Accesseur de la longueur d'un cote du carre
	float obtenirLongueur() const;
	/// Accesseur du typePosNoeud
    TypePosPoint obtenirTypePosNoeud() const;
	/// Retourne la coordonee puisque la table est toujours a (0, 0, 0)
	Vecteur3 obtenirPositionAbsolue() const;
	/// Assigne la position relative du noeud et deplace le modele 3D de la table
	virtual void assignerPositionRelative( const Vecteur3& positionRelative);


	/// Accesseur de pointHorizontalSym_
	NoeudPoint* obtenirPointSym() const;
	/// Mutateur de pointHorizontalSym_
	void modifierPointSym(NoeudPoint* pointSym);
	/// Méthode de validation des déplacements
    bool validerDeplacement(const Vecteur3& pos, const Vecteur2& deplace, int axe);
	
	/// Retourne le rayon du noeud
	virtual float obtenirRayon() const;
	/// Méthode calculant le deplacement maximale d'un point de contrôle
	Vecteur3 calculerDeplacementMax(Vecteur3 posAbsActuel, Vecteur3 deplacement);

	/// Creation du noeud XML d'un point
	virtual XmlElement* creerNoeudXML();
	/// Initialisation du NoeudPoint à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);

	/// Retourne la liste
	const GroupeTripleAdresseFloat* obtenirListePointsAChanger() const;
	/// Modifie la liste
	void modifierListePointsAChanger(GroupeTripleAdresseFloat val) { listePointsAChanger_ = val; }

	Vecteur3 obtenirPositionInitiale() const { return positionInitiale_; }
	void modifierPositionInitiale(Vecteur3 val) { positionInitiale_ = val; }

    static const float DEFAULT_RADIUS;
private:
	/// Longueur d'un cote du carre représentant le point
	float longueurCote_;
	/// attribut de mémoire pour le noeud pour le type de noeud
	TypePosPoint typePosNoeud_;
	/// Pointeur sur le point symétrique
	NoeudPoint* pointSym_;
	/// Liste des points du modele de la table associes a ce noeud
	GroupeTripleAdresseFloat listePointsAChanger_;
	/// Position initiale du noeud
	Vecteur3 positionInitiale_;
	
};
///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
