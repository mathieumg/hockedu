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
/// @brief Implantation d'un noeud du patron composite qui peut posséder
///        des enfants.
///
///        Cette classe implante les différentes fonctions relatives aux
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
	virtual unsigned int calculerProfondeur() const;

	/// Vide le noeud de ses enfants.
	virtual void vider();
	/// Permet d'enlever l'enfant de la liste interne, sans libérer la mémoire de celui-ci
	void detacherEnfant( const NoeudAbstrait* noeud );
	/// Efface le noeud passé en paramètre.
	virtual void effacer( const NoeudAbstrait* noeud );

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
	/// Obtient le nombre d'enfants du noeud.
	virtual unsigned int obtenirNombreEnfants() const;

	/// Sélectionne tous les enfants de même que le noeud.
	virtual void selectionnerTout();
	/// Désélectionne tous les enfants de même que le noeud.
	virtual void deselectionnerTout();
	/// Vérifier si le noeud ou un de ses enfants est sélectionné.
	virtual bool selectionExiste() const;
	/// Inverse la selection du noeud et de ses enfants
	virtual void inverserSelectionTout();


	/// Change le mode d'affichage des polygones.
	virtual void changerModePolygones( bool estForce );
	/// Assigne le mode d'affichage des polygones.
	virtual void assignerModePolygones( GLenum modePolygones );
	/// Affiche le noeud de manière concrète.
	virtual void afficherConcret() const;

    void DrawChild() const;

    /// Anime le noeud.
	virtual void animer( const float& dt );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);

	/// Retourne le conteneur d'enfant du noeud courant
	ConteneurNoeuds& obtenirEnfants() { return enfants_; }

	/// Application de la physique des noeuds la ou applicable
	virtual void gestionCollision( const float& temps );
	/// Mise a Jour de la position de ce noeud
	virtual void majPosition( const float& temps );
	/// Repositionnement des modele pour enlever la penetration entre les noeuds
	virtual void ajusterEnfoncement();
	/// Ajustement de la vitesse des noeuds

	virtual void ajusterVitesse( const float& temps );
	
	/// Retourne les racines des sous-arbres selectionnees
    void getSelectedNodes(ConteneurNoeuds& pSelectedNodes) const;
	/// Retourne true si un enfant possede un noeid selectionne
	virtual bool possedeSelection();
	/// Modificateur de terrain_
	virtual void modifierTerrain(Terrain* val);

    /// Allows to create and initialize nodes from an xml element, default behavior
    void CreateAndInitNodesFromXml( const XmlElement* child );

protected:

	/// La liste des enfants.
	ConteneurNoeuds enfants_;

private:
	/// Constructeur copie déclaré privé mais non défini pour éviter le
	/// constructeur copie généré par le compilateur.
	NoeudComposite(const NoeudComposite&);
};


#endif // __ARBRE_NOEUDS_NOEUDCOMPOSITE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
