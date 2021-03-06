///////////////////////////////////////////////////////////////////////////
/// @file NoeudRondelle.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDRONDELLE_H__
#define __ARBRE_NOEUDS_NOEUDRONDELLE_H__
#pragma warning( disable : 4290 )

#include "NoeudComposite.h"
#include "ExceptionJeu.h"
#include "RunnableBreaker.h"
class NoeudTable;
class NoeudMaillet;

enum PuckFlags
{
    /// Indicates if a collision has benn detected in this frame
    PUCK_COLLISION_DETECTED,

    NB_PUCK_FLAGS
};

///////////////////////////////////////////////////////////////////////////
/// @class NoeudRondelle
/// @brief Classe qui repr�sente une rondelle du jeu.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////// 
class NoeudRondelle : public NoeudAbstrait , public RunnableBreaker
{
public:
    typedef NoeudAbstrait Super;
    friend class UsineNoeudRondelle;
private:
    /// Constructeur priv� pour forcer a passer par l'usine
    /// Constructeur � partir du type du noeud.
    NoeudRondelle(unsigned int& puckCreated, unsigned int puckLimit);
public:
    /// Destructeur.
    virtual ~NoeudRondelle();


    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudRondelle*>(n);
        return !!n2 && Super::equals(n);
    }
    /// Affiche le cube.
    virtual void renderReal() const;
    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilis� sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
    /// Effectue l'animation
    virtual void tick( const float& temps );
    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v);

#if MANUAL_PHYSICS_DETECTION
    /// Application de la physique de la rondelle
    virtual void collisionDetection( const float& temps );
    /// Mise a Jour de la position de ce noeud
    virtual void positionUpdate( const float& temps );
    /// Repositionnement des modele pour enlever la penetration entre les noeuds
    virtual void fixOverlap();
    /// Ajustement de la vitesse des noeuds
    virtual void fixSpeed( const float& temps );
#endif

    /// Accesseur de mVelocite
    Vecteur3 obtenirVelocite() const;
    void modifierVelocite(const Vecteur3& val);

    /// Accesseurs pour la modification
    //float obtenirAngle() const {return angle_;}
    float obtenirVitesseRotation() const;

    /// Modificateurs pour la rotation
    //void modifierAngle(const float angle) {angle_ = angle;}
    void modifierVitesseRotation(const float vitesse);

    /// Accesseur et mutateur de positionOriginale
    Vecteur3 obtenirPositionOriginale() const { return positionOriginale_; }
    void modifierPositionOriginale(const Vecteur3& val) { positionOriginale_ = val; }

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
    
    static const float DEFAULT_RADIUS;
private:
#if MANUAL_PHYSICS_DETECTION
    /// Velocite courante de la rondelle
    Vecteur3 mVelocite;
    /// Coefficient de friction resenti par la table
    float mCoefFriction;
    /// Vitesse de rotation
    float mVitesseRotation;
    /// Sauvegarde de la position de la rondelle avant la MAJ
    Vecteur3 anciennePos_;

    /// Vecteur d'enfoncement resultant de collisions
    Vecteur3 enfoncement_;
    Vecteur3 vitesseResultante_;
    bool collision_;
    /// Conservation en memoire du pointeur de la table
    NoeudTable* table_;

    /// Variables pour la gestion du vent
    float coeffVent_;
    float puissanceVent_;
    float coeffDirectVent_;

    /// Acceleration de la rondelle passant par un accelerateur
    float bonusAccelResultant_;
#endif
    /// Conservation en memoire de la position de la rondelle leur du debut de la partie
    Vecteur3 positionOriginale_;

    Flags<int,NB_PUCK_FLAGS> mFlags;

    /// reference to the factory's counter of puck instances
    unsigned int& mNbPuckCreated;
    NoeudMaillet* mLastHittingMallet;
public:
    /// Accessors of mLastHittingMallet
    inline NoeudMaillet* getLastHittingMallet() const { return mLastHittingMallet; }
    inline void setLastHittingMallet(NoeudMaillet* pVal) { mLastHittingMallet = pVal; }


    /// Accessors of mCollisionDetected
    inline bool IsCollisionDetected() const { return mFlags.IsFlagSet(PUCK_COLLISION_DETECTED); }
    inline void setCollisionDetected(bool pVal) { mFlags.SetFlag(pVal,PUCK_COLLISION_DETECTED); }
};


#endif // __ARBRE_NOEUDS_NOEUDRONDELLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
