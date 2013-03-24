///////////////////////////////////////////////////////////////////////////////
/// @file NoeudComposite.h
/// @author DGI-INF2990
/// @date 2007-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDCOMPOSITE_H__
#define __ARBRE_NOEUDS_NOEUDCOMPOSITE_H__


#include "NoeudAbstrait.h"
#include "RazerGameTypeDef.h"

class NoeudCompositeTest;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudComposite
/// @brief Implantation d'un noeud du patron composite qui peut poss�der
///        des enfants.
///
///        Cette classe implante les diff�rentes fonctions relatives aux
///        enfants, comme l'ajout, le retrait, la recherche, etc.
///
/// @author DGI-2990
/// @date 2007-01-24
///////////////////////////////////////////////////////////////////////////
class NoeudComposite : public NoeudAbstrait
{
public:
    typedef NoeudAbstrait Super;
	/// Pour les tests unitaires
	friend NoeudCompositeTest;

	/// Constructeur.
	NoeudComposite(
		const std::string& type = std::string( "" )
		);
	/// Destructeur.
	virtual ~NoeudComposite();


	// Interface d'un noeud
    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait*);
	/// Calcule la profondeur de l'arbre sous le noeud courant.
	virtual unsigned int treeDepth() const;

	/// Vide le noeud de ses enfants.
	virtual void empty();
	/// Permet d'enlever l'enfant de la liste interne, sans lib�rer la m�moire de celui-ci
	void unlinkChild( const NoeudAbstrait* noeud );
	/// Efface le noeud pass� en param�tre.
	virtual void erase( const NoeudAbstrait* noeud );

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
	/// Obtient le nombre d'enfants du noeud.
	virtual unsigned int childCount() const;

	/// S�lectionne tous les enfants de m�me que le noeud.
	virtual void selectAll();
	/// D�s�lectionne tous les enfants de m�me que le noeud.
	virtual void deselectAll();
	/// V�rifier si le noeud ou un de ses enfants est s�lectionn�.
	virtual bool selectionPresent() const;
	/// Inverse la selection du noeud et de ses enfants
	virtual void inverserSelectionTout();


	/// Change le mode d'affichage des polygones.
	virtual void cyclePolygonMode( bool estForce );
	/// Assigne le mode d'affichage des polygones.
	virtual void setPolygonMode( GLenum modePolygones );
	/// Affiche le noeud de mani�re concr�te.
	virtual void renderReal() const;

    void DrawChild() const;

    /// Anime le noeud.
	virtual void tick( const float& dt );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);

	/// Retourne le conteneur d'enfant du noeud courant
	ConteneurNoeuds& obtenirEnfants() { return enfants_; }

	/// Application de la physique des noeuds la ou applicable
	virtual void collisionDetection( const float& temps );
	/// Mise a Jour de la position de ce noeud
	virtual void positionUpdate( const float& temps );
	/// Repositionnement des modele pour enlever la penetration entre les noeuds
	virtual void fixOverlap();
	/// Ajustement de la vitesse des noeuds

	virtual void fixSpeed( const float& temps );
	
	/// Retourne les racines des sous-arbres selectionnees
    void getSelectedNodes(ConteneurNoeuds& pSelectedNodes) const;
	/// Modificateur de terrain_
	virtual void setField(Terrain* val);

    /// Allows to create and initialize nodes from an xml element, default behavior
    void CreateAndInitNodesFromXml( const XmlElement* child );

protected:

	/// La liste des enfants.
	ConteneurNoeuds enfants_;

private:
	/// Constructeur copie d�clar� priv� mais non d�fini pour �viter le
	/// constructeur copie g�n�r� par le compilateur.
	NoeudComposite(const NoeudComposite&);
};


#endif // __ARBRE_NOEUDS_NOEUDCOMPOSITE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
