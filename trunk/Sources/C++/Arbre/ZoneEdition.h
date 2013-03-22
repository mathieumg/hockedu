///////////////////////////////////////////////////////////////////////////////
/// @file ZoneEdition.h
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "XMLUtils.h"

class TerrainTest;
class Terrain;

///////////////////////////////////////////////////////////////////////////
/// @class ZoneEdition
/// @brief Objet contenant les propriété de la zone d'édition d'un terrain
///
/// @author Michael Ferris
/// @date 2012-03-19
///////////////////////////////////////////////////////////////////////////
class ZoneEdition
{
public:
	friend TerrainTest;
	ZoneEdition(Terrain* owner);
    ~ZoneEdition();

	/// Permet de modifier les limites de la zone d'edition de facon relative à une table
	void modifierLimitesExt(const float* boiteEnglobantTable, const float& longueur,const float& largeur);

	/// Creation du noeud XML de la zone d'édition
	XmlElement* creerNoeudXML();
	/// Initialistion de la zone d'édition à partir d'un noeud XML
	bool initialisationXML(const XmlElement*);
	/// Remet la zone d'edition a son etat par defaut
	void reinitialiser();
	/// Effectue l'affichage de la zone d'edition
	void afficher();

private:
	/// Attribut de la zone d'édition
	float mLimitIntX;
	float mLimitExtX;

	float mLimitIntY;
	float mLimitExtY; 

    Terrain* mOwner;

#if BOX2D_INTEGRATED
    class b2Body* mPhysicsBody;
    void clearPhysicsBody();
#endif
    void rebuild();

/// Accesseurs
public:
    static const float DEFAUT_LIMITE_INT_X;
    static const float DEFAUT_LIMITE_INT_Y;
    static const float DEFAUT_LIMITE_EXT_Y; 
    static const float DEFAUT_LIMITE_EXT_X;

	/// Accesseur de mLimitIntX
	inline float obtenirLimiteIntLongueur() const { return mLimitIntX; }
	/// Modificateur de mLimitIntX
	inline void modifierLimiteIntLongueur(float val) { mLimitIntX = val; rebuild(); }
	/// Accesseur de mLimitIntY
	inline float obtenirLimiteIntLargeur() const { return mLimitIntY; }
	/// Modificateur de mLimitIntY
	inline void modifierLimiteIntLargeur(float val) { mLimitIntY = val; rebuild(); }
	/// Accesseur de mLimitExtY
	inline float obtenirLimiteExtLargeur() const { return mLimitExtY; }
	/// Modificateur de mLimitExtY
	inline void modifierLimiteExtLargeur(float val) { mLimitExtY = val; rebuild(); }
	/// Accesseur de mLimitExtX
	inline float obtenirLimiteExtLongueur() const { return mLimitExtX; }
	/// Modificateur de mLimitExtX
	inline void modifierLimiteExtLongueur(float val) { mLimitExtX = val; rebuild(); }

    /// checks if both zone are the same
    bool equals( ZoneEdition * zone );
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




