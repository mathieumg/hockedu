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
#include "BoundingBox.h"

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

    void rebuild();
private:
	/// Attribut de la zone d'édition
	float mLimitIntX;
	float mLimitExtX;

	float mLimitIntY;
	float mLimitExtY; 

    /// Aligned-Axis bounding box
    BoundingBox mAABBExt;
    BoundingBox mAABBInt;

    Terrain* mOwner;

    float* mRenderVertices;
    int mVerticesCount;

#if BOX2D_INTEGRATED
    class b2Body* mPhysicsBody;
    void clearPhysicsBody();
#endif

/// Accesseurs
public:
    static const float DEFAUT_LIMITE_INT_X;
    static const float DEFAUT_LIMITE_INT_Y;
    static const float DEFAUT_LIMITE_EXT_Y; 
    static const float DEFAUT_LIMITE_EXT_X;

	/// Accesseur de mLimitIntX
	inline float obtenirLimiteIntX() const { return mLimitIntX; }
	/// Modificateur de mLimitIntX
	inline void modifierLimiteIntX(float val) { mLimitIntX = val; rebuild(); }
	/// Accesseur de mLimitIntY
	inline float obtenirLimiteIntY() const { return mLimitIntY; }
	/// Modificateur de mLimitIntY
	inline void modifierLimiteIntY(float val) { mLimitIntY = val; rebuild(); }
	/// Accesseur de mLimitExtY
	inline float obtenirLimiteExtY() const { return mLimitExtY; }
	/// Modificateur de mLimitExtY
	inline void modifierLimiteExtY(float val) { mLimitExtY = val; rebuild(); }
	/// Accesseur de mLimitExtX
	inline float obtenirLimiteExtX() const { return mLimitExtX; }
	/// Modificateur de mLimitExtX
	inline void modifierLimiteExtX(float val) { mLimitExtX = val; rebuild(); }

    /// Accessors of mAABBExt
    inline const BoundingBox& getAABBExt() const { return mAABBExt; }
    /// Accessors of mAABBInt
    inline BoundingBox getAABBInt() const { return mAABBInt; }

    /// checks if both zone are the same
    bool equals( ZoneEdition * zone );
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




