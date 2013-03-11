//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurCollision.h
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"
#include "AideCollision.h"
#include "RazerGameTypeDef.h"

/// Enum contenant le type boite de collision à vérifier
enum TypeCollision{SEGMENT, CERCLE};
typedef std::vector<aidecollision::DetailsCollision> ConteneurDetailsCollision;

class VisitorQueryCallBack;


///////////////////////////////////////////////////////////////////////////
/// @class VisiteurCollision
/// @brief Visiteur pour les test de collision entre les objets
///
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
///////////////////////////////////////////////////////////////////////////
class VisiteurCollision :
	public VisiteurNoeud
{
public:
    friend VisitorQueryCallBack;
	/// Constructeur par paramètre
	VisiteurCollision(NoeudAbstrait* noeudAVerifier, bool flag = true);
	/// Constructeur par paramètre
	VisiteurCollision(Vecteur2 position, bool flag = true, float rayon = 5);
	/// Destructeur
	~VisiteurCollision(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NodeWallAbstract* noeud );
	/// Visitation d'un noeud but
	virtual void visiterNoeudBut( NoeudBut* noeud );
	/// Visitation d'un noeud maillet
	virtual void visiterNoeudMaillet( NoeudMaillet* noeud );
	/// Visitation d'un noeud portail
	virtual void visiterNoeudPortail( NoeudPortail* noeud );
	/// Visitation d'un noeud rondelle
	virtual void visiterNoeudRondelle( NoeudRondelle* noeud );
	/// Visitation d'un noeud table
	virtual void visiterNoeudTable( NoeudTable* noeud );
	/// Visitation d'un noeud point
	virtual void visiterNoeudPoint( NoeudPoint* noeud );
	/// Visitation d'un noeud accélérateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
    /// Visitation d'un noeud point de control
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );

	/// Algorithme de détection de collision entre 2 cercle
	void detectionCollisionCercleCercle( NoeudAbstrait* noeud );
	/// Remet les paramètre du visiteur aux valeurs de départ pour une prochaine visitation
	void reinitialiser();
	/// Algorithme de détection de collision entre 1 cercle et un segment
	void detectionCollisionCercleSegment( NoeudAbstrait* noeud );
	/// Retourne le resultat (true s'il y a une collision)
	bool collisionPresente() const;
	/// Accesseur de la liste de collision
	void obtenirListeCollision(ConteneurNoeuds &listeDeRetour);
	/// Accesseur de coin2
	Vecteur3 obtenirCoin2() const { return coin2_; }
	void modifierCoin2(Vecteur3 val) { coin2_ = val; }
	/// Accesseur de coin1
	Vecteur3 obtenirCoin1() const { return coin1_; }
	void modifierCoin1(Vecteur3 val) { coin1_ = val; }
	/// Accesseur de typeCollision_
	TypeCollision obtenirTypeCollision() const { return typeCollision_; }
	void modifierTypeCollision(TypeCollision val) { typeCollision_ = val; }
	/// Accesseur de rayonAVerifier_
	float obtenirRayonAVerifier() const { return rayonAVerifier_; }
	void modifierRayonAVerifier(float val) { rayonAVerifier_ = val; }
	/// Accesseur de conteneurDetailsCollision
	inline ConteneurDetailsCollision obtenirConteneurDetailsCollision() const { return conteneurDetailsCollision; }
	/// Permet de connaitre le nombre de noeud en collision detecter par le visiteur
	unsigned int obtenirNbrNoeudEncollision();

    /// Accessors of noeudAVerifier_
    inline void setNoeudAVerifier(NoeudAbstrait* pVal) { noeudAVerifier_ = pVal; }
private:
	/// Attributs
	NoeudAbstrait* noeudAVerifier_;


    /// Position de l'élément à vérifier
	Vecteur2 positionAVerifier_;
	/// Rayon du cercle à vérifier
	float rayonAVerifier_;
	/// Ensemble contenant tous les noeuds en collision avec le noeud. Le premier noeud est celui de base pour effectuer les tests
	ConteneurNoeuds noeudsCollision_;
	/// Résultat indiquant s'il y à une collision ou non
	bool collision_;
	/// Indicateur du type de boite de collision du noeud à vérifier
	TypeCollision typeCollision_;
	/// Coin pour la détection de collision par segment
	Vecteur3 coin1_,coin2_;
	/// Indicateur pour savoir si on surligne les
	bool flag_;
	/// Informations sur les collisions detecte, en lien direct avec le conteneur de noeuds en collisions
	ConteneurDetailsCollision conteneurDetailsCollision;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

