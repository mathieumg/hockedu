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
class NoeudTable;
///////////////////////////////////////////////////////////////////////////
/// @class NoeudRondelle
/// @brief Classe qui représente une rondelle du jeu.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////// 
class NoeudRondelle : public NoeudAbstrait , public ObjetAnimable
{
public:
    typedef NoeudAbstrait Super;
    friend class UsineNoeudRondelle;
private:
    /// Constructeur privé pour forcer a passer par l'usine
    /// Constructeur à partir du type du noeud.
    NoeudRondelle(const std::string& typeNoeud, unsigned int& puckCreated, unsigned int puckLimit);
public:
    /// Destructeur.
    ~NoeudRondelle();

    /// Affiche le cube.
    virtual void afficherConcret() const;
    /// Effectue l'animation
    virtual void animer( const float& temps );
    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v);

    /// Application de la physique de la rondelle
    virtual void gestionCollision( const float& temps );
    /// Mise a Jour de la position de ce noeud
    virtual void majPosition( const float& temps );
    /// Repositionnement des modele pour enlever la penetration entre les noeuds
    virtual void ajusterEnfoncement();
    /// Ajustement de la vitesse des noeuds
    virtual void ajusterVitesse( const float& temps );

    /// Accesseur de mVelocite
    Vecteur3 obtenirVelocite() const;
    void modifierVelocite(Vecteur3 val) { mVelocite = val; }

    /// Accesseurs pour la modification
    //float obtenirAngle() const {return angle_;}
    float obtenirVitesseRotation() const {return mVitesseRotation;}

    /// Modificateurs pour la rotation
    //void modifierAngle(const float angle) {angle_ = angle;}
    void modifierVitesseRotation(const float vitesse) {mVitesseRotation = vitesse;}

    void validerPropriteteTablePourJeu() throw(ExceptionJeu);

    /// Accesseur et mutateur de positionOriginale
    Vecteur3 obtenirPositionOriginale() const { return positionOriginale_; }
    void modifierPositionOriginale(Vecteur3 val) { positionOriginale_ = val; }

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();

    /// Objet Animable interface
    virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
    //////////////////////////////////////////////////////////////////////////

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
};


#endif // __ARBRE_NOEUDS_NOEUDRONDELLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
