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
#include "PositionObserver.h"

// Surtout ne pas toucher a la position des elements dans l'enum
/// ils représentent des positions dans un array
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

class NodeRinkBoards;

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
    typedef NoeudComposite Super;

	friend NodeRinkBoards;
	/// Constructeur à partir du type du noeud.
	NoeudPoint( const std::string& typeNoeud, float coordX, float coordY, TypePosPoint typePosNoeud);
	/// Destructeur.
	virtual ~NoeudPoint();

	/// Affiche le cube.
	virtual void renderReal() const;
    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilisé sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
	/// Effectue l'animation
	virtual void tick( const float& temps );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);
	/// Accesseur du typePosNoeud
    TypePosPoint obtenirTypePosNoeud() const;
	/// Retourne la coordonee puisque la table est toujours a (0, 0, 0)
	const Vecteur3&  getPosition() const;
	/// Assigne la position relative du noeud et deplace le modele 3D de la table
	virtual void setPosition( const Vecteur3& positionRelative);

    /// moves 3D model's vertex to the targetLocation
    void move3DModel( const Vecteur3& targetLocation );

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();

	/// Accesseur de pointHorizontalSym_
	NoeudPoint* obtenirPointSym() const;
	/// Mutateur de pointHorizontalSym_
	void modifierPointSym(NoeudPoint* pointSym);
	/// Méthode de validation des déplacements
    bool validerDeplacement(const Vecteur3& pos, const Vecteur2& deplace, int axe);
	
	/// Méthode calculant le deplacement maximale d'un point de contrôle
	Vecteur3 calculerDeplacementMax(Vecteur3 posAbsActuel, Vecteur3 deplacement);

	/// Creation du noeud XML d'un point
	virtual XmlElement* createXmlNode();
	/// Initialisation du NoeudPoint à partir d'un element XML
	virtual bool initFromXml(const XmlElement* element);

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
