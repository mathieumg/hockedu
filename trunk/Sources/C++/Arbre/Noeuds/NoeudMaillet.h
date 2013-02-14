///////////////////////////////////////////////////////////////////////////
/// @file NoeudMaillet.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDMAILLET_H__
#define __ARBRE_NOEUDS_NOEUDMAILLET_H__


#include "NoeudAbstrait.h"
#include "MouseMoveObserver.h"
#include "ObjetAnimable.h"

enum DirectionMaillet{DIR_HAUT,DIR_BAS,DIR_GAUCHE,DIR_DROITE,NB_DIR};
class NoeudTable;
class NoeudRondelle;
class JoueurVirtuel;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudMaillet
/// @brief Classe qui représente un maillet de jeu.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudMaillet : public NoeudAbstrait, public MouseMoveObserver, public ObjetAnimable
{
public:
    /// Constructeur à partir du type du noeud.
    NoeudMaillet(const std::string& typeNoeud);
    /// Destructeur.
    ~NoeudMaillet();

    /// Effectue l'animation
    virtual void animer( const float& temps );
    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v);
    /// Cette fonction effectue le véritable rendu de l'objet.
    virtual void afficherConcret() const;
    /// Physique
    virtual void gestionCollision( const float& temps);
    /// Mise a Jour de la position de ce noeud
    virtual void majPosition( const float& temps );
    /// Repositionnement des modele pour enlever la penetration entre les noeuds
    virtual void ajusterEnfoncement();
    /// Ajustement de la vitesse des noeuds
    virtual void ajusterVitesse( const float& temps );
    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
    /// Builds the mouse joint when starting to play
    void buildMouseJoint();
    /// Free memory of the mouse joint when game is done
    void destroyMouseJoint();
    /// updates mouse joint when receiving a mousemove event
    virtual void updateObserver( class MouseMoveSubject* pSubject );
    /// applies physics behavior on b2Body before simulation
    void preSimulationActions();

    //////////////////////////////////////////////////////////////////////////
    /// Objet Animable interface
    virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
    //////////////////////////////////////////////////////////////////////////

    static unsigned int mailletExistant;

    static const float DEFAULT_RADIUS;
    /// Attributs
private:
	/// Velocite courante du maillet
	Vecteur3 velocite_;
	/// Vitesse maximale
	const float vitesse_;
	/// tableau indiquant dans quel direction ce deplace le maillet controlle par le clavier
	bool direction_[NB_DIR];
	/// Flag pour savoir qui a le controle de ce maillet
	bool estControleParClavier_;
	bool estControleParOrdinateur_;

	/// Conservation en memoire de la position de la osuris
	Vecteur3 posSouris_;
	/// Indique si ce maillet est controlle par le joueur de gauche
	bool estAGauche_;
	
	/// Vecteur d'enfoncement resultant de collisions
	Vecteur3 enfoncement_;

	/// Conservation en memoire de la position du maillet au debut de la partie
	Vecteur3 positionOriginale_;
		
	/// Ancienne position du maillet
	Vecteur3 anciennePos_;
	
	/// Indique s'il y a une collision avec la rondelle
	bool collisionAvecRondelle_;

	/// Le joueur qui possède ce maillet
	JoueurVirtuel* joueur_;
	
    /// joint controlling the mallet
    class b2MouseJoint* mMouseJoint;

/// Accesseurs
public:


	/// Mutateur de la direction du maillet controler par clavier
	void modifierDirection(bool active, DirectionMaillet dir);
	/// Permet d'indiquer au maillet s'il est controle par le clavier ou la souris
	void setKeyboardControlled(bool clavier);
	/// Assignation de la position de la souris pour que le maillet puisse la suivre
	void assignerPosSouris(Vecteur3 pos);
	/// Accesseur de velocite_
	Vecteur3 obtenirVelocite() const { return velocite_; }
	/// Modificateur de velocite_
	void modifierVelocite(Vecteur3 val) { velocite_ = val; }
	/// Accesseur de vitesseMax_
	const float obtenirVitesse() const { return vitesse_; }
	/// Accesseur de estControleParOrdinateur_
	bool obtenirEstControleParOrdinateur() const { return estControleParOrdinateur_; }
	/// Modificateur de estControleParOrdinateur_
	void setIsAI(bool val) { estControleParOrdinateur_ = val; }
	/// Accesseur et mutateur de positionOriginale
	Vecteur3 obtenirPositionOriginale() const { return positionOriginale_; }
	void modifierPositionOriginale(Vecteur3 val) { positionOriginale_ = val; }
	/// Accesseur de anciennePos_
	Vecteur3 obtenirAnciennePos() const { return anciennePos_; }
	/// Modificateur de anciennePos_
	void modifierAnciennePos(Vecteur3 val) { anciennePos_ = val; }
	/// Modificateur de l'indicateur de la position du joueur
	void setIsLeft(bool val) { estAGauche_ = val; }
	/// Accesseur de joueurGauche_
	bool estAGauche() const { return estAGauche_; }
	/// Accesseur de joueur_
	JoueurVirtuel* obtenirJoueur() const { return joueur_; }
	/// Modificateur de joueur_
	void setAIPlayer(JoueurVirtuel* val) { joueur_ = val; }



};

#endif // __ARBRE_NOEUDS_NOEUDMAILLET_H__

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
