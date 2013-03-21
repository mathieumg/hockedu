///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAbstrait.h
/// @author DGI-INF2990, Michael Ferris, Samuel Ledoux, Mathieu Parent
/// @date 2007-01-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDABSTRAIT_H__
#define __ARBRE_NOEUDS_NOEUDABSTRAIT_H__

#define _USE_MATH_DEFINES
#include "RazerGameUtilities.h"
#include <math.h>
#define _WINSOCKAPI_
#if WIN32
#include <windows.h>
#include "glew.h"
#elif __APPLE__
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#include "Vecteur.h"
#include "ExceptionJeu.h"
#include "Flags.h"
#include "RazerGameTypeDef.h"

//Foward Declaration
class NoeudComposite;
class VisiteurNoeud;
class VisiteurCollision;
class VisiteurNoeudTest;
class Modele3D;
class Terrain;
class BonusModifierAbstract;

// max of 16 categories because box2D flag have only 16 bits
enum PhysicsCategory
{
    CATEGORY_NONE     = 0x0000,
    CATEGORY_BOUNDARY = 0x0001,
    CATEGORY_PUCK     = 0x0002,
    CATEGORY_MALLET   = 0x0004,
    CATEGORY_PORTAL   = 0x0008,
    CATEGORY_BOOST    = 0x0010,
    CATEGORY_BONUS    = 0x0020,
    CATEGORY_WALL     = 0x0040,
    CATEGORY_GOALIE     = 0x0080,
};

enum NodeFlags
{
    NODEFLAGS_VISIBLE,
    NODEFLAGS_SELECTED,
    NODEFLAGS_CAN_BE_SELECTED,
    NODEFLAGS_RECORDABLE,
    NODEFLAGS_HIGHTLIGHT,
    NODEFLAGS_IS_IN_GAME,
    NODEFLAGS_ACTIVE,
    NB_NODEFLAGS
};

///////////////////////////////////////////////////////////////////////////
/// @class NoeudAbstrait
/// @brief Classe de base du patron composite utilisée pour créer l'arbre
///        de rendu.
///
///        Cette classe abstraite comprend l'interface de base que doivent
///        implanter tous les noeuds pouvant être présent dans l'arbre de
///        rendu.
///
/// @author DGI-2990, Michael Ferris, Samuel Ledoux, Mathieu Parent
/// @date 2007-01-24
///////////////////////////////////////////////////////////////////////////
class NoeudAbstrait
{
public:
	friend VisiteurNoeudTest;
	/// Constructeur.
	NoeudAbstrait(
		const std::string& type = std::string( "" )
		);
	/// Destructeur.
	virtual ~NoeudAbstrait();

	// Interface d'un noeud
    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait*);
	/// Calcule la profondeur de l'arbre sous le noeud courant.
	virtual unsigned int treeDepth() const;

	/// Vide le noeud de ses enfants.
	virtual void empty();
	/// Efface le noeud passé en paramètre.
	virtual void erase( const NoeudAbstrait* noeud );
    /// correctly delete a node and removes it form its parent.
    /// do not call in destructor because the parent might be initialising the
    // destruction.
    void deleteThis();

	/// Cherche un noeud par le type (sur un noeud constant).
	virtual const NoeudAbstrait* find( const std::string& typeNoeud ) const;
	/// Cherche un noeud par le type.
	virtual NoeudAbstrait* find( const std::string& typeNoeud );
	/// Cherche un noeud enfant selon l'indice (sur un noeud constant).
	virtual const NoeudAbstrait* find( unsigned int indice ) const;
	/// Cherche un noeud enfant selon l'indice.
	virtual NoeudAbstrait* find( unsigned int indice );

	/// Ajoute un noeud enfant.
	virtual bool add( NoeudAbstrait* enfant );
    /// Permet d'enlever l'enfant de la liste interne, sans libérer la mémoire de celui-ci
    virtual void unlinkChild( const NoeudAbstrait* noeud ){}
	/// Obtient le nombre d'enfants du noeud.
	virtual unsigned int childCount() const;

	/// Changer la sélection du noeud.
	virtual void toggleSelection();
	/// Sélectionne tous les enfants de même que le noeud.
	virtual void selectAll();
	/// Désélectionne tous les enfants de même que le noeud.
	virtual void deselectAll();
	/// Vérifier si le noeud ou un de ses enfants est sélectionné.
	virtual bool selectionPresent() const;

	/// Affiche le noeud.
	virtual void render() const;
	/// Affiche le noeud de manière concrète.
	virtual void renderReal() const;
    /// Fonction appeler dans render concret pour faire le
    /// rendu OpenGL, uniquement utilisé sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
	/// Anime le noeud.
	virtual void tick( const float& dt );
    /// node tick received when actually playing the game (simulation running)
    virtual void playTick(float temps);

	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);
	/// Met a jour la matrice de transformations
	virtual void updateMatrice();

	typedef std::pair<Vecteur3, Vecteur3> PaireVect3;
	/// Retourne la zone occupee par le noeud (coord virtuelles)
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Recalcule le rayon
	virtual void updateRadius();

	/// Permet d'assigner les attribut nécessaire à la collision
	virtual void setCollisionVisitorAttributes(VisiteurCollision* v);

#if MANUAL_PHYSICS_DETECTION
	/// Application de la physique des noeuds la ou applicable
	virtual void collisionDetection( const float& temps ){}
	/// Misae a Jour de la position de ce noeud
	virtual void positionUpdate( const float& temps) {}
	/// Repositionnement des modele pour enlever la penetration entre les noeuds
	virtual void fixOverlap(){}
	/// Ajustement de la vitesse des noeuds
	virtual void fixSpeed( const float& temps ) {}
#endif

	/// Creation du noeud XML du Noeud
	virtual XmlElement* createXmlNode();
    /// Initialisation du NoeudAbstrait à partir d'un element XML
	virtual bool initFromXml(const XmlElement* element);

    /// Utilities pour la lecture/ecriture de la position du noeud
    void XmlWriteNodePosition( XmlElement* elementNoeud );
    static bool XmlReadNodePosition( Vecteur3& pos, const XmlElement* element );
    

    /// accessor to the key to retrieve the list and/or 3D Model
    virtual const std::string& get3DModelKey() const {return mType;}

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody() {}
    /// determines if this node is active in the simulation
    void activate(bool activate);

    /// Allows to keep this node in sync with box2D
    static void synchroniseTransformFromB2CallBack(void* , const struct b2Transform&);
    virtual void synchroniseTransformFromB2(const struct b2Transform&);

    /// Recreates everything needed for the game
    virtual void forceFullUpdate();

	/// Libere la memoire de l'objet Box2D
    virtual void clearPhysicsBody();


protected:
    class b2Body* mPhysicBody;

    /// Type du noeud.
	std::string      mType;

	/// Mode d'affichage des polygones.
	GLenum           mModePolygones;

	/// Position relative du noeud.
	mutable Vecteur3  mPosition;

    /// flags of a node;
    Flags<char,NB_NODEFLAGS> mFlags;

	/// Pointeur vers le parent.
	NoeudComposite*   mParent;

	/// Matrice de transformation.
	GLfloat mTransformationMatrix[16];

	/// Echelle courante (afin de limiter la mise a l'echelle).
	Vecteur3 mScale;

	/// Angle de rotation autour de l'axe Z.
	float mAngle;

	/// Rayon de l'objet
	float mRadius;
    /// Rayon de l'objet sans la mise à l'échelle, 
    /// les noeuds concrets doivent assigner cette valeur
    float mDefaultRadius;

	/// id unique dans openGL
    GLuint mGlId;
    /// id unique dans openGL pour le type du noeud
	GLuint mGlTypeId;

    ModifiersContainer mModifiers;
private:
	/// Compteur pour les id unique dans openGL
	static GLuint mIdGlCounter;
	
	/// Pointeur sur le terrain que le noeud est inclu dedans, Null si le noeud n'est pas sur un terrain
	Terrain* mField;



	/// Accesseurs
public:
    virtual const class ArbreRendu* GetTreeRoot() const;

    /// Obtient le parent de ce noeud.
    inline NoeudComposite* getParent(){return mParent;}
    /// Obtient le parent de ce noeud (version constante).
    inline const NoeudComposite* getParent() const{return mParent;}
    /// Assigne le parent de ce noeud.
    virtual void setParent( NoeudComposite* parent );

    /// Obtient la position relative du noeud.
    inline virtual const Vecteur3& getPosition() const {return mPosition;}
    /// Assigne la position relative du noeud.
    virtual void setPosition( const Vecteur3& positionRelative );

	/// Accesseur des facteurs d'echelle
	virtual void getScale(Vecteur3& echelleCourante) const;
	/// Mutateur des facteurs d'echelle
	virtual void setScale(const Vecteur3& echelleCourante);

	/// Acces et modification de l'angle de rotation
    virtual void setAngle( float angle);
	virtual float getAngle() const;

    /// Obtient le type du noeud.
    inline const std::string& getType() const;
    inline const RazerKey getKey()const {return RazerGameUtilities::StringToKey(mType);}

    /// Écrit l'état de l'affichage du du noeud.
    inline void setVisible( bool affiche );
    /// Vérifie si le noeud se fait render.
    inline bool isVisible() const;

    /// Écrit l'état de la sélection du noeud.
    void setSelection( bool selectionne );
    /// Vérifie si le noeud est sélectionné.
    inline bool IsSelected() const;

    /// Écrit si le noeud peut être sélectionné ou non.
    inline void setCanBeSelected( bool selectionnable );
    /// Vérifie si le noeud est sélectionnable.
    inline bool canBeSelected() const;

    /// Écrit si le noeud peut être enregistré ou non.
    inline void setRecordable( bool enregistrable );
    /// Vérifie si le noeud est enregistrable.
    inline bool isRecordable() const;

	/// Modifie la valeur de la collision
	inline void setHighlight(bool estEnCollision) {mFlags.SetFlag(estEnCollision,NODEFLAGS_HIGHTLIGHT);}
	/// Retourne la valeur de collision_
	inline bool isHightlighted() const {return mFlags.IsFlagSet(NODEFLAGS_HIGHTLIGHT);}

	/// Retourne le rayon du noeud
	virtual float getRadius() const;

	/// Retourne le modele 3D representant le noeud ( !!! peut etre NULL )
	Modele3D* getModel() const;

	/// Accesseur de mField
	inline Terrain* getField() const { return mField; }
	/// Modificateur de mField
	virtual void setField(Terrain* val);

    /// Accessors of mWorld
    class b2World* getWorld();
    /// Accessors of mPhysicBody
    inline class b2Body* getPhysicBody() const { return mPhysicBody; }

	/// Accesseur de mGlId
	inline GLuint getGlId() const { return mGlId; }

    /// Accessors of mDefaultRadius
    inline float getDefaultRadius() const { return mDefaultRadius; }
    inline void setDefaultRadius(const float& pVal) { mDefaultRadius = pVal; updateRadius(); }

    inline bool IsInGame() const { return mFlags.IsFlagSet(NODEFLAGS_IS_IN_GAME);}
    inline void SetIsInGame(bool val) { mFlags.SetFlag(val,NODEFLAGS_IS_IN_GAME);}

    inline bool AddModifier(BonusModifierAbstract* pModifier)
    {
        auto res = mModifiers.insert(pModifier);
        return res.second;
    }
    inline const ModifiersContainer& GetModifiers()const
    {
        return mModifiers;
    }
	/// Change le mode d'affichage des polygones.
	virtual void cyclePolygonMode( bool estForce );
	/// Assigne le mode d'affichage des polygones.
	virtual void setPolygonMode( GLenum modePolygones );

    inline bool isActive()const{return mFlags.IsFlagSet(NODEFLAGS_ACTIVE);}
};

////////////////////////////////////////////////////////////////////////
///
/// @fn inline const std::string& NoeudAbstrait::getType(  )
///
/// Cette fonction retourne une chaîne représentante le type du noeud.
///
///
/// @return const std::string& : Le type du noeud.
///
////////////////////////////////////////////////////////////////////////
inline const std::string& NoeudAbstrait::getType() const
{
	return mType;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline void NoeudAbstrait::setVisible( bool affiche )
///
/// Cette fonction permet d'assigner si le noeud se fait render ou
/// non lorsque l'arbre de rendu se fait render.  Elle permet donc
/// de temporairement suspendre ou activer l'affichage d'un noeud.
///
/// @param[in] bool affiche : L'état affiché ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::setVisible( bool affiche )
{
    mFlags.SetFlag(affiche,NODEFLAGS_VISIBLE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::isVisible(  )
///
/// Cette fonction retourne l'état que le noeud se fait render ou non.
///
///
/// @return bool : L'état affiché ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::isVisible() const
{
    return mFlags.IsFlagSet(NODEFLAGS_VISIBLE);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::IsSelected(  )
///
/// Cette fonction retourne l'état d'être sélectionné ou non du noeud.
///
///
/// @return bool : L'état sélectionné ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::IsSelected() const
{
	// Un objet non sélectionnable n'est jamais sélectionné.
	return (mFlags.IsFlagSet(NODEFLAGS_SELECTED) && mFlags.IsFlagSet(NODEFLAGS_CAN_BE_SELECTED));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline void NoeudAbstrait::setCanBeSelected( bool selectionnable )
///
/// Cette fonction permet d'assigner l'état d'être sélectionnable ou non du noeud.
///
/// @param[in] bool selectionnable : L'état sélectionnable ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::setCanBeSelected( bool selectionnable )
{
    mFlags.SetFlag(selectionnable,NODEFLAGS_CAN_BE_SELECTED);
    setSelection(IsSelected() && selectionnable);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::canBeSelected(  )
///
/// Cette fonction retourne l'état d'être sélectionnable ou non du noeud.
///
///
/// @return bool : L'état sélectionnable ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::canBeSelected() const
{
    return mFlags.IsFlagSet(NODEFLAGS_CAN_BE_SELECTED);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setRecordable( bool enregistrable )
///
/// Cette fonction permet d'assigner l'état d'être entregistrable ou non du noeud.
///
/// @param[in] bool enregistrable : L'état enregistrable ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::setRecordable( bool enregistrable )
{
    mFlags.SetFlag(enregistrable,NODEFLAGS_RECORDABLE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::isRecordable(  )
///
/// Cette fonction retourne l'état d'être enregistrable en XML ou non du
/// noeud.
///
///
/// @return bool : L'état enregistrable ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::isRecordable() const
{
    return mFlags.IsFlagSet(NODEFLAGS_RECORDABLE);
}


#endif // __ARBRE_NOEUDS_NOEUDABSTRAIT_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
