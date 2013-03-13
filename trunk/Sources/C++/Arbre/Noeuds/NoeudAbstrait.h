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

//Foward Declaration
class NoeudComposite;
class VisiteurNoeud;
class VisiteurCollision;
class VisiteurNoeudTest;
class Modele3D;
class Terrain;

// max of 16 categories because box2D flag have only 16 bits
enum PhysicsCategory
{
    CATEGORY_NONE     = 0x0000,
    CATEGORY_BOUNDARY = 0x0001,
    CATEGORY_PUCK     = 0x0002,
    CATEGORY_MALLET   = 0x0004,
    CATEGORY_PORTAL   = 0x0008,
    CATEGORY_BOOST    = 0x0010,
};

///////////////////////////////////////////////////////////////////////////
/// @class NoeudAbstrait
/// @brief Classe de base du patron composite utilis�e pour cr�er l'arbre
///        de rendu.
///
///        Cette classe abstraite comprend l'interface de base que doivent
///        implanter tous les noeuds pouvant �tre pr�sent dans l'arbre de
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

	/// �crit l'�tat de l'affichage du du noeud.
	inline void assignerAffiche( bool affiche );
	/// V�rifie si le noeud se fait afficher.
	inline bool estAffiche() const;

	/// �crit l'�tat de la s�lection du noeud.
	void assignerSelection( const bool& selectionne );
	/// V�rifie si le noeud est s�lectionn�.
	inline bool estSelectionne() const;
	/// �crit si le noeud peut �tre s�lectionn� ou non.
	inline void assignerEstSelectionnable( const bool& selectionnable );
	/// V�rifie si le noeud est s�lectionnable.
	inline bool estSelectionnable() const;
	/// �crit si le noeud peut �tre enregistr� ou non.
	inline void assignerEstEnregistrable( const bool& enregistrable );
	/// V�rifie si le noeud est enregistrable.
	inline bool estEnregistrable() const;



	// Interface d'un noeud
    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait*);
	/// Calcule la profondeur de l'arbre sous le noeud courant.
	virtual unsigned int calculerProfondeur() const;

	/// Vide le noeud de ses enfants.
	virtual void vider();
	/// Efface le noeud pass� en param�tre.
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
    /// Permet d'enlever l'enfant de la liste interne, sans lib�rer la m�moire de celui-ci
    virtual void detacherEnfant( const NoeudAbstrait* noeud ){}
	/// Obtient le nombre d'enfants du noeud.
	virtual unsigned int obtenirNombreEnfants() const;

	/// Changer la s�lection du noeud.
	virtual void inverserSelection();
	/// S�lectionne tous les enfants de m�me que le noeud.
	virtual void selectionnerTout();
	/// D�s�lectionne tous les enfants de m�me que le noeud.
	virtual void deselectionnerTout();
	/// V�rifier si le noeud ou un de ses enfants est s�lectionn�.
	virtual bool selectionExiste() const;

	/// Change le mode d'affichage des polygones.
	virtual void changerModePolygones( bool estForce );
	/// Assigne le mode d'affichage des polygones.
	virtual void assignerModePolygones( GLenum modePolygones );
	/// Affiche le noeud.
	virtual void afficher() const;
	/// Affiche le noeud de mani�re concr�te.
	virtual void afficherConcret() const;
    /// Fonction appeler dans afficher concret pour faire le
    /// rendu OpenGL, uniquement utilis� sous APPLE.
    /// utiliser les liste d'affichage pour windows
    virtual void renderOpenGLES() const;
	/// Anime le noeud.
	virtual void animer( const float& dt );

	/// Accueil un visiteur
	virtual void acceptVisitor( class VisiteurNoeud& v);   
	/// Accesseur de la matrice de transformation
	virtual void obtenirMatrice(GLdouble* matriceRetour) const;
	/// Accesseur des facteurs d'echelle
	virtual void obtenirEchelleCourante(Vecteur3& echelleCourante) const;
	/// Mutateur des facteurs d'echelle
	virtual void modifierEchelleCourante(const Vecteur3& echelleCourante);
	/// Mutateur de la matrice de transformation
	virtual void modifierMatrice(GLdouble *matrice);
	/// Acces et modification de l'angle de rotation
	virtual void assignerAngle(const float& angle);
	virtual float obtenirAngle() const;

	/// Met a jour la matrice de transformations
	virtual void updateMatrice();
	/// Modifie la valeur de la collision
	virtual void modifierSurligner(const bool& estEnCollision);
	/// Retourne la valeur de collision_
	virtual bool estSurligne();
	/// Retourne true si le noeud est selectionne
	virtual bool possedeSelection();

	typedef std::pair<Vecteur3, Vecteur3> PaireVect3;
	/// Retourne la zone occupee par le noeud (coord virtuelles)
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Retourne le rayon du noeud
	virtual float obtenirRayon() const;
	/// Recalcule le rayon
	virtual void updateRayon();

	/// Permet d'assigner les attribut n�cessaire � la collision
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
    /// Initialisation du NoeudAbstrait � partir d'un element XML
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

    /// Allows to keep this node in sync with box2D
    static void SynchroniseTransformFromB2CallBack(void* , const struct b2Transform&);
    virtual void SynchroniseTransformFromB2(const struct b2Transform&);

    /// Recreates everything needed for the game
    virtual void forceFullUpdate();

	/// Libere la memoire de l'objet Box2D
    virtual void clearPhysicsBody();
    /// Accessors of mWorld
    class b2World* getWorld();
protected:
    class b2Body* mPhysicBody;

    /// Type du noeud.
	std::string      type_;

	/// Mode d'affichage des polygones.
	GLenum           modePolygones_;

	/// Position relative du noeud.
	mutable Vecteur3  mPosition;

	/// Vrai si on doit afficher le noeud.
	bool             affiche_;

	/// S�lection du noeud.
	bool             selectionne_;

	/// Vrai si le noeud est s�lectionnable.
	bool             selectionnable_;

	/// D�termine si l'objet peut �tre sauvegard� en XML.
	bool             enregistrable_;

	/// Pointeur vers le parent.
	NoeudComposite*   parent_;

	/// Matrice de transformation.
	GLdouble matrice_[16];

	/// Echelle courante (afin de limiter la mise a l'echelle).
	Vecteur3 echelleCourante_;

	/// Angle de rotation autour de l'axe Z.
	float mAngle;

	/// Rayon de l'objet
	float rayon_;
    /// Rayon de l'objet sans la mise � l'�chelle, 
    /// les noeuds concrets doivent assigner cette valeur
    float mDefaultRadius;

    

	/// Est en collision
	bool surligne_;
	
	/// id unique dans openGL
    GLuint glId_;
    /// id unique dans openGL pour le type du noeud
	GLuint glTypeId_;
private:
	/// Compteur pour les id unique dans openGL
	static GLuint compteurIdGl_;
	
	
	/// Pointeur sur le terrain que le noeud est inclu dedans, Null si le noeud n'est pas sur un terrain
	Terrain* terrain_;


	/// Accesseurs
public:
    /// Accessors of mPhysicBody
    inline class b2Body* getPhysicBody() const { return mPhysicBody; }
	/// Accesseur de glId_
	inline GLuint obtenirGlId() const { return glId_; }

    /// Accessors of mDefaultRadius
    inline float getDefaultRadius() const { return mDefaultRadius; }
    inline void setDefaultRadius(const float& pVal) { mDefaultRadius = pVal; updateRayon(); }
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
/// @fn inline void NoeudAbstrait::setPosition( const Vecteur3& positionRelative )
///
/// Cette fonction permet d'assigner la position relative du noeud par
/// rapport � son parent.
///
/// @param[in] const Vecteur3 & positionRelative : La position relative.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///
/// @fn inline const std::string& NoeudAbstrait::obtenirType(  )
///
/// Cette fonction retourne une cha�ne repr�sentante le type du noeud.
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
/// @param[in] bool affiche : L'�tat affich� ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::assignerAffiche( bool affiche )
{
	affiche_ = affiche;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::estAffiche(  )
///
/// Cette fonction retourne l'�tat que le noeud se fait afficher ou non.
///
///
/// @return bool : L'�tat affich� ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estAffiche() const
{
	return affiche_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::estSelectionne(  )
///
/// Cette fonction retourne l'�tat d'�tre s�lectionn� ou non du noeud.
///
///
/// @return bool : L'�tat s�lectionn� ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estSelectionne() const
{
	// Un objet non s�lectionnable n'est jamais s�lectionn�.
	return (selectionne_ && selectionnable_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline void NoeudAbstrait::assignerEstSelectionnable( bool selectionnable )
///
/// Cette fonction permet d'assigner l'�tat d'�tre s�lectionnable ou non du noeud.
///
/// @param[in] bool selectionnable : L'�tat s�lectionnable ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::assignerEstSelectionnable( const bool& selectionnable )
{
	selectionnable_ = selectionnable;
	selectionne_    = selectionne_ && selectionnable_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline bool NoeudAbstrait::estSelectionnable(  )
///
/// Cette fonction retourne l'�tat d'�tre s�lectionnable ou non du noeud.
///
///
/// @return bool : L'�tat s�lectionnable ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estSelectionnable() const
{
	return selectionnable_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::assignerEstEnregistrable( bool enregistrable )
///
/// Cette fonction permet d'assigner l'�tat d'�tre entregistrable ou non du noeud.
///
/// @param[in] bool enregistrable : L'�tat enregistrable ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
inline void NoeudAbstrait::assignerEstEnregistrable( const bool& enregistrable )
{
	enregistrable_ = enregistrable;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::estEnregistrable(  )
///
/// Cette fonction retourne l'�tat d'�tre enregistrable en XML ou non du
/// noeud.
///
///
/// @return bool : L'�tat enregistrable ou non.
///
////////////////////////////////////////////////////////////////////////
inline bool NoeudAbstrait::estEnregistrable() const
{
	return enregistrable_;
}


#endif // __ARBRE_NOEUDS_NOEUDABSTRAIT_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
