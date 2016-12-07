///////////////////////////////////////////////////////////////////////////
/// @file NoeudBut.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDBUT_H__
#define __ARBRE_NOEUDS_NOEUDBUT_H__

#include "NoeudComposite.h"
#include "PositionObserver.h"

class NodeRinkBoards;
class NoeudPoint;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudBut
/// @brief Classe qui représente un but dans lequel le joueur doit compter.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudBut : public NoeudComposite, public PositionSubject
{
public:
    typedef NoeudComposite Super;
    friend NodeRinkBoards;
	/// Constructeur à partir du type du noeud.
	NoeudBut(int joueur, NoeudPoint * coinHaut, NoeudPoint * coinBas, NoeudComposite* pParent = NULL);
	/// Destructeur.
	virtual ~NoeudBut();

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudBut*>(n);
        return !!n2 && 
            mBottomAngle == n2->mBottomAngle && 
            mTopAngle == n2->mTopAngle && 
            mBottomPosition == n2->mBottomPosition && 
            mTopPosition == n2->mTopPosition && 
            mGoalLength == n2->mGoalLength && 
            joueur_ == n2->joueur_ && 
            Super::equals(n);
    }
	/// Affiche le cube.
	virtual void renderReal() const;
	/// Effectue l'animation
	virtual void tick( const float& temps );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);
	/// Pour savoir à quel joueur le but appartient
	int obtenirJoueur() const;
	/// Retourne la zone occupee par la table
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Retourne le rayon du noeud
	void updateLongueur(float facteurModificationEchelle = 1);
	/// Modifie le pointeur sur le but adverse
	void modifierButAdverse(NoeudBut* butAdverse);
	NoeudBut* obtenirButAdverse();

	/// Permet de connaitre la hauteur des buts pour savoir si la rondelle peut y passer
	float obtenirHauteurBut();

	/// Permet d'assigner les attribut nécessaire à la collision
	virtual void setCollisionVisitorAttributes(VisiteurCollision* v);

	/// Permet d'obtenir le rayon du but
	virtual float getRadius();

	/// Creation du noeud XML du but
	virtual XmlElement* createXmlNode();
	/// Initialisation du But à partir d'un element XML
	virtual bool initFromXml(const XmlElement* element);

	/// Accesseur de mBottomPosition
	inline Vecteur3 obtenirPositionBas() const { return mBottomPosition; }
	/// Accesseur de mTopPosition
	inline Vecteur3 obtenirPositionHaut() const { return mTopPosition; }

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
    /// Recreates everything needed for the game
    virtual void forceFullUpdate();

    /// Update the physics body for the puck catcher according with the radius of the puck
    void updatePuckCatcher(float puckRadius);
    virtual const Vecteur3& getPosition() const {return mParent->getPosition();}

    /// Libere la memoire de l'objet Box2D
    virtual void clearPhysicsBody();

    /// Gets the length of the current goal
    inline float getLength() const {return mGoalLength;}

    static const Vecteur3 DEFAULT_SIZE;
    const Vecteur3 getBottomPosition() const { return mBottomPosition; }
    const Vecteur3 getTopPosition() const { return mTopPosition; }
    const float getBottomAngle() const { return mBottomAngle; }
    const float getTopAngle() const { return mTopAngle; }
private:
	/// Attribut disant si le but est au joueur1 ou joueur 2 par un int, 1 ou 2
	int joueur_;
    /// Pointeur sur le coin en haut
	NoeudPoint* coinHaut_;
	/// Pointeur sur le coin en bas
	NoeudPoint* coinBas_;
    /// Angle des 2 composantes du but en degre
    float mBottomAngle,mTopAngle;
	/// Pointeur sur le but adverse
	NoeudBut* butAdverse_;
	/// Position du point extérieur du but
    mutable Vecteur3 mTopPosition, mBottomPosition;
    /// Longueur du but
    float mGoalLength;

    class b2Body* mPuckCatcher;
    float mCachedPuckRadius;
};


#endif // __ARBRE_NOEUDS_NOEUDBUT_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
