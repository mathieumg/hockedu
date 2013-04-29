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
#include "ObjetAnimable.h"
#include "RunnableBreaker.h"
#include <vector>

enum DirectionMaillet{DIR_HAUT,DIR_BAS,DIR_GAUCHE,DIR_DROITE,NB_DIR};
enum MalletSide{MALLET_SIDE_LEFT,MALLET_SIDE_RIGHT};
class NoeudTable;
class NoeudRondelle;
class PlayerComputer;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudMaillet
/// @brief Classe qui repr�sente un maillet de jeu.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudMaillet : public NoeudAbstrait, public ObjetAnimable, public RunnableBreaker
{
public:
    typedef NoeudAbstrait Super;
    friend class UsineNoeudMaillet;
private:
    /// Constructeur priv� pour forcer a passer par l'usine
    /// Constructeur � partir du type du noeud.
    NoeudMaillet(const std::string& typeNoeud, unsigned int& malletCreated, unsigned int malletLimit);
public:
    /// Destructeur.
    virtual ~NoeudMaillet();

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudMaillet*>(n);
        return !!n2 && 
            estControleParClavier_ == n2->estControleParClavier_ && 
            estControleParOrdinateur_ == n2->estControleParOrdinateur_ && 
            estControleParNetwork_ == n2->estControleParNetwork_ && 
            estAGauche_ == n2->estAGauche_ && 
            Super::equals(n);
    }
    /// Effectue l'animation
    virtual void tick( const float& temps );
    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v);
    /// Cette fonction effectue le v�ritable rendu de l'objet.
    virtual void renderReal() const;
    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilis� sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
    /// Physique

#if MANUAL_PHYSICS_DETECTION
    virtual void collisionDetection( const float& temps);
    /// Mise a Jour de la position de ce noeud
    virtual void positionUpdate( const float& temps );
    /// Repositionnement des modele pour enlever la penetration entre les noeuds
    virtual void fixOverlap();
    /// Ajustement de la vitesse des noeuds
    virtual void fixSpeed( const float& temps );
#endif

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
    /// Builds the mouse joint when starting to play
    void buildMouseJoint(bool pIsNetworkControlled = false);
    /// Free memory of the mouse joint when game is done
    void destroyMouseJoint();
    /// node tick received when actually playing the game (simulation running)
    virtual void playTick(float temps);

    inline bool estControleParNetwork() const { return estControleParNetwork_; }

    //////////////////////////////////////////////////////////////////////////
    /// Objet Animable interface
    virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
    //////////////////////////////////////////////////////////////////////////

    static const float DEFAULT_RADIUS;
    /// Attributs
private:
	MalletSide malletSide_;
	/// Velocite courante du maillet
	Vecteur3 velocite_;
	/// Vitesse maximale
	const float vitesse_;
	/// tableau indiquant dans quel direction ce deplace le maillet controlle par le clavier
	bool direction_[NB_DIR];
	/// Flag pour savoir qui a le controle de ce maillet
	bool estControleParClavier_;
	bool estControleParOrdinateur_;
    bool estControleParNetwork_;

	/// Conservation en memoire de la position de destination
	Vecteur3 mTargetDestination;
    
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

	/// Le joueur qui poss�de ce maillet
	SPPlayerAbstract joueur_;
	
    /// reference to the factory's counter of mallet instances
    unsigned int& mNbMalletCreated;
    
#if BOX2D_PLAY
    /// joint controlling the mallet
    class b2MouseJoint* mMouseJoint;
    b2Body* mMouseBody;
#endif



/// Accesseurs
public:
	/// Accesseur de malletSide_
	MalletSide getMalletSide() const { return malletSide_; }
	void setMalletSide(MalletSide val) { malletSide_ = val; }
	/// Mutateur de la direction du maillet controler par clavier
	void modifierDirection(bool active, DirectionMaillet dir);
	/// Permet d'indiquer au maillet s'il est controle par le clavier ou la souris
	void setKeyboardControlled(bool clavier);
	/// Assignation de la position de la souris pour que le maillet puisse la suivre
	void setTargetDestination(const Vecteur3& pos, bool forceUpdateMouseJoint = false);
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
	SPPlayerAbstract obtenirJoueur() const { return joueur_; }
	/// Modificateur de joueur_
	void setPlayer(SPPlayerAbstract val) { joueur_ = val; }
    inline void setIsNetworkPlayer(const bool pIsNetworkPlayer) {estControleParNetwork_ = pIsNetworkPlayer;}


//#if MAT_DEBUG_
//    static std::vector<Vecteur3> mListePointsDebug;


//#endif


};

#endif // __ARBRE_NOEUDS_NOEUDMAILLET_H__

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
