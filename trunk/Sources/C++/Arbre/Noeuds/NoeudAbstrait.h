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
#include <set>

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
};

enum NodeFlags
{
    NODEFLAGS_AFFICHE,
    NODEFLAGS_SELECTIONNE,
    NODEFLAGS_SELECTIONNABLE,
    NODEFLAGS_ENREGISTRABLE,
    NODEFLAGS_SURLIGNE,
    NODEFLAGS_IS_IN_GAME,
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

    virtual const class ArbreRendu* GetTreeRoot() const;

	/// Obtient le parent de ce noeud.
	inline NoeudComposite* obtenirParent();

	/// Obtient le parent de ce noeud (version constante).
	inline const NoeudComposite* obtenirParent() const;

	/// Assigne le parent de ce noeud.
	virtual void assignerParent( NoeudComposite* parent );

	/// Obtient la position relative du noeud.
	inline const Vecteur3& getPosition() const {return mPosition;}

	/// Assigne la position relative du noeud.
    virtual void setPosition( const Vecteur3& positionRelative );

	/// Obtient le type du noeud.
	inline const std::string& obtenirType() const;

	/// Écrit l'état de l'affichage du du noeud.
	inline void assignerAffiche( bool affiche );
	/// Vérifie si le noeud se fait afficher.
	inline bool estAffiche() const;

	/// Écrit l'état de la sélection du noeud.
    void assignerSelection( bool selectionne );
	/// Vérifie si le noeud est sélectionné.
	inline bool estSelectionne() const;
	/// Écrit si le noeud peut être sélectionné ou non.
    inline void assignerEstSelectionnable( bool selectionnable );
	/// Vérifie si le noeud est sélectionnable.
	inline bool estSelectionnable() const;
	/// Écrit si le noeud peut être enregistré ou non.
    inline void assignerEstEnregistrable( bool enregistrable );
	/// Vérifie si le noeud est enregistrable.
	inline bool estEnregistrable() const;



	// Interface d'un noeud
    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait*);
	/// Calcule la profondeur de l'arbre sous le noeud courant.
	virtual unsigned int calculerProfondeur() const;

	/// Vide le noeud de ses enfants.
	virtual void vider();
	/// Efface le noeud passé en paramètre.
	virtual void effacer( const NoeudAbstrait* noeud );
    /// correctly delete a node and removes it form its parent.
    /// do not call in destructor because the parent might be initialising the
    // destruction.
    void deleteThis();

	/// Cherche un noeud par le type (sur un noeud constant).
	virtual const NoeudAbstrait* chercher( const std::string& typeNoeud ) const;
	/// Cherche un noeud par le type.
	virtual NoeudAbstrait* chercher( const std::string& typeNoeud );
	/// Cherche un noeud enfant selon l'indice (sur un noeud constant).
	virtual const NoeudAbstrait* chercher( unsigned int indice ) const;
	/// Cherche un noeud enfant selon l'indice.
	virtual NoeudAbstrait* chercher( unsigned int indice );

	/// Ajoute un noeud enfant.
	virtual bool ajouter( NoeudAbstrait* enfant );
    /// Permet d'enlever l'enfant de la liste interne, sans libérer la mémoire de celui-ci
    virtual void detacherEnfant( const NoeudAbstrait* noeud ){}
	/// Obtient le nombre d'enfants du noeud.
	virtual unsigned int obtenirNombreEnfants() const;

	/// Changer la sélection du noeud.
	virtual void inverserSelection();
	/// Sélectionne tous les enfants de même que le noeud.
	virtual void selectionnerTout();
	/// Désélectionne tous les enfants de même que le noeud.
	virtual void deselectionnerTout();
	/// Vérifier si le noeud ou un de ses enfants est sélectionné.
	virtual bool selectionExiste() const;

	/// Change le mode d'affichage des polygones.
	virtual void changerModePolygones( bool estForce );
	/// Assigne le mode d'affichage des polygones.
	virtual void assignerModePolygones( GLenum modePolygones );
	/// Affiche le noeud.
	virtual void afficher() const;
	/// Affiche le noeud de manière concrète.
	virtual void afficherConcret() const;
    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilisé sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
	/// Anime le noeud.
	virtual void animer( const float& dt );
    /// node tick received when actually playing the game (simulation running)
    virtual void PlayTick(float temps);

	/// Accueil un visiteur
	virtual void acceptVisitor( class VisiteurNoeud& v);   
	/// Accesseur des facteurs d'echelle
	virtual void obtenirEchelleCourante(Vecteur3& echelleCourante) const;
	/// Mutateur des facteurs d'echelle
	virtual void modifierEchelleCourante(const Vecteur3& echelleCourante);
	/// Acces et modification de l'angle de rotation
	virtual void assignerAngle(const float& angle);
	virtual float obtenirAngle() const;

	/// Met a jour la matrice de transformations
	virtual void updateMatrice();
	/// Modifie la valeur de la collision
	inline void modifierSurligner(bool estEnCollision) {mFlags.SetFlag(estEnCollision,NODEFLAGS_SURLIGNE);}
	/// Retourne la valeur de collision_
	inline bool estSurligne() const {return mFlags.IsFlagSet(NODEFLAGS_SURLIGNE);}
	typedef std::pair<Vecteur3, Vecteur3> PaireVect3;
	/// Retourne la zone occupee par le noeud (coord virtuelles)
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Retourne le rayon du noeud
	virtual float obtenirRayon() const;
	/// Recalcule le rayon
	virtual void updateRayon();

	/// Permet d'assigner les attribut nécessaire à la collision
	virtual void assignerAttributVisiteurCollision(VisiteurCollision* v);

	/// Application de la physique des noeuds la ou applicable
	virtual void gestionCollision( const float& temps ){}
	/// Misae a Jour de la position de ce noeud
	virtual void majPosition( const float& temps) {}
	/// Repositionnement des modele pour enlever la penetration entre les noeuds
	virtual void ajusterEnfoncement(){}
	/// Ajustement de la vitesse des noeuds
	virtual void ajusterVitesse( const float& temps ) {}

	/// Creation du noeud XML du Noeud
	virtual XmlElement* creerNoeudXML();
    /// Initialisation du NoeudAbstrait à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);

    /// Utilities pour la lecture/ecriture de la position du noeud
    void XmlWriteNodePosition( XmlElement* elementNoeud );
    static bool XmlReadNodePosition( Vecteur3& pos, const XmlElement* element );
    

	/// Retourne le modele 3D representant le noeud ( !!! peut etre NULL )
	Modele3D* obtenirModele() const;
    /// accessor to the key to retrieve the list and/or 3D Model
    virtual const std::string& get3DModelKey() const {return type_;}

	/// Accesseur de terrain_
	inline Terrain* GetTerrain() const { return terrain_; }
	/// Modificateur de terrain_
	virtual void modifierTerrain(Terrain* val);

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody() {}
    /// determines if this node is active in the simulation
    void Activate(bool activate);

    /// Allows to keep this node in sync with box2D
    static void SynchroniseTransformFromB2CallBack(void* , const struct b2Transform&);
    virtual void SynchroniseTransformFromB2(const struct b2Transform&);

    /// Recreates everything needed for the game
    virtual void forceFullUpdate();

	/// Libere la memoire de l'objet Box2D
    virtual void clearPhysicsBody();
#if BOX2D_INTEGRATED  
    /// Accessors of mWorld
    class b2World* getWorld();
#endif

protected:
#if BOX2D_INTEGRATED  
    class b2Body* mPhysicBody;
#endif

    /// Type du noeud.
	std::string      type_;

	/// Mode d'affichage des polygones.
	GLenum           modePolygones_;

	/// Position relative du noeud.
	mutable Vecteur3  mPosition;

    /// flags of a node;
    Flags<char,NB_NODEFLAGS> mFlags;

	/// Pointeur vers le parent.
	NoeudComposite*   parent_;

	/// Matrice de transformation.
	GLfloat matrice_[16];

	/// Echelle courante (afin de limiter la mise a l'echelle).
	Vecteur3 echelleCourante_;

	/// Angle de rotation autour de l'axe Z.
	float mAngle;

	/// Rayon de l'objet
	float rayon_;
    /// Rayon de l'objet sans la mise à l'échelle, 
    /// les noeuds concrets doivent assigner cette valeur
    float mDefaultRadius;

	/// id unique dans openGL
    GLuint glId_;
    /// id unique dans openGL pour le type du noeud
	GLuint glTypeId_;
private:
	/// Compteur pour les id unique dans openGL
	static GLuint compteurIdGl_;
	
	/// Pointeur sur le terrain que le noeud est inclu dedans, Null si le noeud n'est pas sur un terrain
	Terrain* terrain_;
    typedef std::set<BonusModifierAbstract*> ModifiersContainer;
    ModifiersContainer mModifiers;



	/// Accesseurs
public:
    /// Accessors of mPhysicBody
    inline class b2Body* getPhysicBody() const { return mPhysicBody; }
	/// Accesseur de glId_
	inline GLuint obtenirGlId() const { return glId_; }

    /// Accessors of mDefaultRadius
    inline float getDefaultRadius() const { return mDefaultRadius; }
    inline void setDefaultRadius(const float& pVal) { mDefaultRadius = pVal; updateRayon(); }
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
};

////////////////////////////////////////////////////////////////////////
///
/// @fn inline NoeudComposite* NoeudAbstrait::obtenirParent(  )
///
/// Cette fonction retourne le pointeur vers le parent de ce noeud.
///
///
/// @return NoeudComposite* : Le pointeur vers le parent.
///
////////////////////////////////////////////////////////////////////////
inline NoeudComposite* NoeudAbstrait::obtenirParent()
{
	return parent_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline const NoeudComposite* NoeudAbstrait::obtenirParent(  )
///
/// Cette fonction retourne le pointeur constant vers le parent de ce noeud.
///
///
/// @return const NoeudComposite* : Le pointeur constant vers le parent.
///
////////////////////////////////////////////////////////////////////////
inline const NoeudComposite* NoeudAbstrait::obtenirParent() const
{
	return parent_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline const std::string& NoeudAbstrait::obtenirType(  )
///
/// Cette fonction retourne une chaîne représentante le type du noeud.
///
///
/// @return const std::string& : Le type du noeud.
///
////////////////////////////////////////////////////////////////////////
inline const std::string& NoeudAbstrait::obtenirType() const
{
	return type_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline void NoeudAbstrait::assignerAffiche( bool affiche )
///
/// Cette fonction permet d'assigner si le noeud se fait afficher ou
/// non lorsque l'arbre de rendu se fait afficher.  Elle permet donc
/// de temporairement suspendre ou activer l'affichage d'un noeud.
///
/// @param[in] bool affiche : L'état affiché ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::assignerAffiche( bool affiche )
{
    mFlags.SetFlag(affiche,NODEFLAGS_AFFICHE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::estAffiche(  )
///
/// Cette fonction retourne l'état que le noeud se fait afficher ou non.
///
///
/// @return bool : L'état affiché ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estAffiche() const
{
    return mFlags.IsFlagSet(NODEFLAGS_AFFICHE);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::estSelectionne(  )
///
/// Cette fonction retourne l'état d'être sélectionné ou non du noeud.
///
///
/// @return bool : L'état sélectionné ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estSelectionne() const
{
	// Un objet non sélectionnable n'est jamais sélectionné.
	return (mFlags.IsFlagSet(NODEFLAGS_SELECTIONNE) && mFlags.IsFlagSet(NODEFLAGS_SELECTIONNABLE));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline void NoeudAbstrait::assignerEstSelectionnable( bool selectionnable )
///
/// Cette fonction permet d'assigner l'état d'être sélectionnable ou non du noeud.
///
/// @param[in] bool selectionnable : L'état sélectionnable ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::assignerEstSelectionnable( bool selectionnable )
{
    mFlags.SetFlag(selectionnable,NODEFLAGS_SELECTIONNABLE);
    assignerSelection(estSelectionne() && selectionnable);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::estSelectionnable(  )
///
/// Cette fonction retourne l'état d'être sélectionnable ou non du noeud.
///
///
/// @return bool : L'état sélectionnable ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estSelectionnable() const
{
    return mFlags.IsFlagSet(NODEFLAGS_SELECTIONNABLE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::assignerEstEnregistrable( bool enregistrable )
///
/// Cette fonction permet d'assigner l'état d'être entregistrable ou non du noeud.
///
/// @param[in] bool enregistrable : L'état enregistrable ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::assignerEstEnregistrable( bool enregistrable )
{
    mFlags.SetFlag(enregistrable,NODEFLAGS_ENREGISTRABLE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::estEnregistrable(  )
///
/// Cette fonction retourne l'état d'être enregistrable en XML ou non du
/// noeud.
///
///
/// @return bool : L'état enregistrable ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estEnregistrable() const
{
    return mFlags.IsFlagSet(NODEFLAGS_ENREGISTRABLE);
}


#endif // __ARBRE_NOEUDS_NOEUDABSTRAIT_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
