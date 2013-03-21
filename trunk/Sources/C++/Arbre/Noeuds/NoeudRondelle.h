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
#include "ObjetAnimable.h"
#include "ExceptionJeu.h"
#include "RunnableBreaker.h"
class NoeudTable;
class NoeudMaillet;
///////////////////////////////////////////////////////////////////////////
/// @class NoeudRondelle
/// @brief Classe qui repr�sente une rondelle du jeu.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////// 
class NoeudRondelle : public NoeudAbstrait , public ObjetAnimable, public RunnableBreaker
{
public:
    typedef NoeudAbstrait Super;
    friend class UsineNoeudRondelle;
private:
    /// Constructeur priv� pour forcer a passer par l'usine
    /// Constructeur � partir du type du noeud.
    NoeudRondelle(const std::string& typeNoeud, unsigned int& puckCreated, unsigned int puckLimit);
public:
    /// Destructeur.
    ~NoeudRondelle();


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

    /// Application de la physique de la rondelle
    virtual void collisionDetection( const float& temps );
    /// Mise a Jour de la position de ce noeud
    virtual void positionUpdate( const float& temps );
    /// Repositionnement des modele pour enlever la penetration entre les noeuds
    virtual void fixOverlap();
    /// Ajustement de la vitesse des noeuds
    virtual void fixSpeed( const float& temps );

    /// Accesseur de mVelocite
    Vecteur3 obtenirVelocite() const;
    void modifierVelocite(const Vecteur3& val);

    /// Accesseurs pour la modification
    //float obtenirAngle() const {return angle_;}
    float obtenirVitesseRotation() const;

    /// Modificateurs pour la rotation
    //void modifierAngle(const float angle) {angle_ = angle;}
    void modifierVitesseRotation(const float vitesse);

    void validerPropriteteTablePourJeu() ;

    /// Accesseur et mutateur de positionOriginale
    Vecteur3 obtenirPositionOriginale() const { return positionOriginale_; }
    void modifierPositionOriginale(Vecteur3 val) { positionOriginale_ = val; }

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();

    /// Objet Animable interface
    virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
    //////////////////////////////////////////////////////////////////////////

    virtual const std::string& get3DModelKey() const;

    static const float DEFAULT_RADIUS;
private:
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
    /// Conservation en memoire de la position de la rondelle leur du debut de la partie
    Vecteur3 positionOriginale_;
	
    /// Variables pour la gestion du vent
    float coeffVent_;
    float puissanceVent_;
    float coeffDirectVent_;

    /// Acceleration de la rondelle passant par un accelerateur
    float bonusAccelResultant_;

    /// reference to the factory's counter of puck instances
    unsigned int& mNbPuckCreated;
    NoeudMaillet* mLastHittingMallet;

public:
    /// Accessors of mLastHittingMallet
    inline NoeudMaillet* getLastHittingMallet() const { return mLastHittingMallet; }
    inline void setLastHittingMallet(NoeudMaillet* pVal) { mLastHittingMallet = pVal; }

};


#endif // __ARBRE_NOEUDS_NOEUDRONDELLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
