///////////////////////////////////////////////////////////////////////////
/// @file NodeBonus.h
/// @author Michael Ferris
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once


#include "NoeudComposite.h"
class FactoryBonusModifier;
///////////////////////////////////////////////////////////////////////////
/// @class NodeBonus
/// @brief Classe qui représente un bonus aléatoire permettant d'affecter la partie
///
/// @author Michael Ferris
/// @date 2013-03-13
///////////////////////////////////////////////////////////////////////////
class NodeBonus : public NoeudAbstrait
{
public:
    typedef NoeudAbstrait Super;
    /// Constructeur à partir du type du noeud.
    NodeBonus(const std::string& typeNoeud);
    /// Destructeur.
    virtual ~NodeBonus();

    /// Affiche le bonus.
    virtual void renderReal() const;
    /// accessor to the key to retrieve the list and/or 3D Model
    virtual const std::string& get3DModelKey() const;
    /// Recreates everything needed for the game
    virtual void forceFullUpdate();
    /// node tick received when actually playing the game (simulation running)
    virtual void playTick(float temps);
    /// Anime le noeud.
    virtual void tick( const float& dt );

    /// Creates and apply a bonus from the puck
    void ExecuteBonus(class NoeudRondelle*);
    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
    virtual void renderOpenGLES() const;
    void ResetTimeLeft();

    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v);
private:
    float mSpawnTimeLeft;// in sec


    float mHeightAngle;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
