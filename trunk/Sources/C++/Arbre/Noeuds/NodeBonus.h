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

    /// vertexArray size must be 81, colorArray size must be 108
    static void getCubeColorVertexArrays(float* vertexArray, float* colorArray);

    virtual void updateMatrice();

    void displayBonus(BonusType pBonusType);

    inline BonusType getBonusType() const {return mBonusType;}

private:
    float mSpawnTimeDelaiTotal;// in sec
    bool mCounting;
    float mBeginTime;


    BonusType mBonusType;

    float mHeightAngle;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
