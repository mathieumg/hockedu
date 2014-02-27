///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAccelerateur.h
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudAccelerateur
/// @brief Classe qui représente un accélérateur.
///
/// @author Samuel Ledoux
/// @date 2012-01-27
///////////////////////////////////////////////////////////////////////////
class NoeudAccelerateur : public NoeudAbstrait
{
public:
    typedef NoeudAbstrait Super;

	/// Constructeur à partir du type du noeud.
	NoeudAccelerateur();
	/// Destructeur.
	virtual ~NoeudAccelerateur();

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudAccelerateur*>(n);
        return !!n2 && 
            bonusAccel_ == n2->bonusAccel_ && 
            activer_ == n2->activer_ && 
            Super::equals(n);
    }
	/// Affiche l'accelerateur.
	virtual void renderReal() const;
    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilisé sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
	/// Effectue l'animation de l'accelerateur.
	virtual void tick( const float& temps );

	virtual void acceptVisitor( VisiteurNoeud& v );

	/// Accesseur de bonusAccel_
	float obtenirBonusAccel() const { return bonusAccel_; }
	void modifierBonusAccel(float val) { bonusAccel_ = val; }

	/// Creation du noeud XML de l'accelerateur
	virtual XmlElement* createXmlNode();
	/// Initialisation du NoeudAccelerateur à partir d'un element XML
	virtual bool initFromXml(const XmlElement* element);

	/// Accesseur de activer_
	bool IsBoostActive() const { return activer_; }
	/// Modificateur de activer_
	void ActivateBoost(bool val);

#if MANUAL_PHYSICS_DETECTION
    /// Application de la physique de la rondelle
	virtual void collisionDetection( const float& temps );
#endif

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();

    /// rayon par défaut de l'accélérateur
    static const float DEFAULT_RADIUS;
private:
	/// Acceleration fournie par les bonus accelerateurs
	float bonusAccel_;
	bool activer_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////