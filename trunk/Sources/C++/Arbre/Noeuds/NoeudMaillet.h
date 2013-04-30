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
/// @brief Classe qui représente un maillet de jeu.
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
    /// Constructeur privé pour forcer a passer par l'usine
    /// Constructeur à partir du type du noeud.
    NoeudMaillet(const std::string& typeNoeud, unsigned int& malletCreated, unsigned int malletLimit);
public:
    /// Destructeur.
    virtual ~NoeudMaillet();

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudMaillet*>(n);
        return !!n2 && 
            Super::equals(n);
    }
    /// Effectue l'animation
    virtual void tick( const float& temps );

    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v);

    /// Cette fonction effectue le véritable rendu de l'objet.
    virtual void renderReal() const;

    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilisé sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
 
    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
    /// Builds the mouse joint when starting to play
    void buildMouseJoint();
    /// Free memory of the mouse joint when game is done
    void destroyMouseJoint();
    /// node tick received when actually playing the game (simulation running)
    virtual void playTick(float temps);

    //////////////////////////////////////////////////////////////////////////
    /// Objet Animable interface
    virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
    //////////////////////////////////////////////////////////////////////////

    static const float DEFAULT_RADIUS;
    /// Fields
private:
	MalletSide malletSide_;

	/// Conservation en memoire de la position du maillet au debut de la partie pour pouvoir le replacer apres un but
	Vecteur3 mOriginalPosition;
		
	/// Le joueur qui possède ce maillet
	SPPlayerAbstract mPlayer;
	
    /// reference to the factory's counter of mallet instances
    unsigned int& mNbMalletCreated;
    
#if BOX2D_PLAY
    /// joint controlling the mallet
    class b2MouseJoint* mMouseJoint;
    b2Body* mMouseBody;
#endif

/// Accessors
public:
	/// Accesseur de malletSide_
	MalletSide getMalletSide() const { return malletSide_; }
	void setMalletSide(MalletSide val) { malletSide_ = val; }
	bool isLeftSide() const { return malletSide_ == MALLET_SIDE_LEFT; }

	/// Assignation de la position que le maillet doit tenter d'atteindre
	void setTargetDestination(const Vecteur3& pos);

	/// Accesseur et mutateur de positionOriginale
	Vecteur3 getOriginalPosition() const { return mOriginalPosition; }
	void setOriginalPosition(Vecteur3 val) { mOriginalPosition = val; }
	
	/// Accesseur de joueur_
	SPPlayerAbstract getPlayer() const { return mPlayer; }
	/// only allow player to modify this to make sure the pointer is coherent and valid
	void setPlayer(SPPlayerAbstract val) { mPlayer = val; }

};

#endif // __ARBRE_NOEUDS_NOEUDMAILLET_H__

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
