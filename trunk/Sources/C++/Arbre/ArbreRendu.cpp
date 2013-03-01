////////////////////////////////////////////////
/// @file   ArbreRendu.cpp
/// @author Martin Bisson
/// @date   2007-01-28
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////

#include "ArbreRendu.h"
#include "UsineNoeud.h"
#include "NoeudAbstrait.h"

#include <GL/glu.h>
#include <stdexcept>
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn ArbreRendu::ArbreRendu()
///
/// Ne fait qu'assigner que ce noeud n'est pas s�lectionnable.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
ArbreRendu::ArbreRendu(Terrain* pField)
	: NoeudComposite("racine")
{
	// On ne veut pas que ce noeud soit s�lectionnable.
	assignerEstSelectionnable(false);
    modifierTerrain(pField);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn ArbreRendu::~ArbreRendu()
///
/// D�truit les usines des noeuds de l'arbre.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
ArbreRendu::~ArbreRendu()
{
    for(auto it = usines_.begin(); it != usines_.end(); ++it)
    {
        delete (it->second);
    }
    usines_.clear();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* ArbreRendu::creerNoeud(const std::string& typeNouveauNoeud) const
///
/// Cette fonction permet de cr�er un nouveau noeud, sans l'ajouter
/// directement � l'arbre de rendu.
///
/// @param[in] typeNouveauNoeud : Le type du nouveau noeud.
///
/// @return Le noeud nouvellement cr�� ou 0 si le noeud n'a pu �tre cr�e.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* ArbreRendu::creerNoeud(
	const std::string& typeNouveauNoeud
	) const
{
	RegistreUsines::const_iterator usine = usines_.find(typeNouveauNoeud);
	if (usine == usines_.end()) {
		// Incapable de trouver l'usine
		return 0;
	}

	// Pour une raison que je ne comprends pas, la ligne suivante ne
	// compile pas:
	//
	// const UsineNoeud* usine = usines_[typeNouveauNoeud];
	//
	// On utilisera donc:
	//const UsineNoeud* usine = (*(usines_.find(typeNouveauNoeud))).second;
	NoeudAbstrait* noeud;
	try
	{
		noeud = usine->second->creerNoeud();
	}
	catch (std::logic_error& e)
	{
        utilitaire::afficherErreur(e.what());
		return 0;
	}

	return noeud;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* ArbreRendu::ajouterNouveauNoeud(const std::string& typeParent, const std::string& typeNouveauNoeud)
///
/// Cette fonction permet d'ajouter un nouveau noeud dans l'arbre de
/// rendu.
///
/// @param[in] typeParent       : Le type du parent du nouveau noeud.
/// @param[in] typeNouveauNoeud : Le type du nouveau noeud.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* ArbreRendu::ajouterNouveauNoeud(
	const std::string& typeParent,
	const std::string& typeNouveauNoeud
	)
{
	NoeudAbstrait* parent = chercher(typeParent);
	if (parent == 0) {
		// Incapable de trouver le parent
		return 0;
	}

	NoeudAbstrait* nouveauNoeud = creerNoeud(typeNouveauNoeud);
	if (nouveauNoeud)
		parent->ajouter(nouveauNoeud);
	/*if(nouveauNoeud!=0)
		nouveauNoeud->selectionnerTout();*/

	return nouveauNoeud;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int ArbreRendu::calculerProfondeurMaximale()
///
/// Cette fonction retourne la profondeur maximale possible de l'arbre.
/// Comme lors du rendu, on effectue un glPushMatrix() pour sauvegarder
/// les transformations, ainsi qu'un glPushName() pour ajouter un nom
/// sur la pile des noms pour la s�lection, la profondeur maximale de
/// l'arbre est limit�e par la taille de la pile des matrices ainsi que
/// par celle de la pile des noms pour la s�lection.
///
/// Cette fonction v�rifie donc ces deux valeurs et retourne la plus
/// petite, c'est-�-dire celle qui limite la profondeur de l'arbre.
///
/// @return La profondeur maximale possible de l'arbre de rendu.
///
////////////////////////////////////////////////////////////////////////
unsigned int ArbreRendu::calculerProfondeurMaximale()
{
	GLint profondeurPileMatrice, profondeurPileNoms;

	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH , &profondeurPileMatrice);
	glGetIntegerv(GL_MAX_NAME_STACK_DEPTH      , &profondeurPileNoms);

	return (profondeurPileMatrice < profondeurPileNoms) ?
profondeurPileMatrice : profondeurPileNoms;
}


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////
