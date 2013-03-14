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
#if WIN32
#include <GL/glu.h>
#endif
#include "Utilitaire.h"
#include "ExceptionJeu.h"
#include "XMLUtils.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn ArbreRendu::ArbreRendu()
///
/// Ne fait qu'assigner que ce noeud n'est pas sélectionnable.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
ArbreRendu::ArbreRendu(Terrain* pField)
	: NoeudComposite("racine")
{
	// On ne veut pas que ce noeud soit sélectionnable.
	setCanBeSelected(false);
    setField(pField);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn ArbreRendu::~ArbreRendu()
///
/// Détruit les usines des noeuds de l'arbre.
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
/// Cette fonction permet de créer un nouveau noeud, sans l'add
/// directement à l'arbre de rendu.
///
/// @param[in] typeNouveauNoeud : Le type du nouveau noeud.
///
/// @return Le noeud nouvellement créé ou 0 si le noeud n'a pu être crée.
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
	catch (ExceptionJeu& e)
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
/// Cette fonction permet d'add un nouveau noeud dans l'arbre de
/// rendu.
///
/// @param[in] typeParent       : Le type du parent du nouveau noeud.
/// @param[in] typeNouveauNoeud : Le type du nouveau noeud.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* ArbreRendu::ajouterNouveauNoeud(
	const std::string& typeParent,
	const std::string& typeNouveauNoeud
	)
{
	NoeudAbstrait* parent = find(typeParent);
	if (parent == 0) {
		// Incapable de trouver le parent
		return 0;
	}

	NoeudAbstrait* nouveauNoeud = creerNoeud(typeNouveauNoeud);
	if (nouveauNoeud)
		parent->add(nouveauNoeud);
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
/// les transformations, ainsi qu'un glPushName() pour add un nom
/// sur la pile des noms pour la sélection, la profondeur maximale de
/// l'arbre est limitée par la taille de la pile des matrices ainsi que
/// par celle de la pile des noms pour la sélection.
///
/// Cette fonction vérifie donc ces deux valeurs et retourne la plus
/// petite, c'est-à-dire celle qui limite la profondeur de l'arbre.
///
/// @return La profondeur maximale possible de l'arbre de rendu.
///
////////////////////////////////////////////////////////////////////////
unsigned int ArbreRendu::calculerProfondeurMaximale()
{
/*	GLint profondeurPileMatrice, profondeurPileNoms;

	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH , &profondeurPileMatrice);
	glGetIntegerv(GL_MAX_NAME_STACK_DEPTH      , &profondeurPileNoms);

	return (profondeurPileMatrice < profondeurPileNoms) ?
profondeurPileMatrice : profondeurPileNoms;*/
    return 10;
}


const char ETIQUETTE_ARBRE[] = "Arbre";

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* RazerGameTree::createXmlNode()
///
/// Creation du noeud XML du Noeud
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* ArbreRendu::createXmlNode()
{
    XmlElement* racine = XMLUtils::createNode(ETIQUETTE_ARBRE);

    return racine;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ArbreRendu::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudAbstrait à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool ArbreRendu::initFromXml( const XmlElement* element )
{
    // Tenter d'obtenir le noeud 'Arbre'
    const XmlElement* racine = XMLUtils::FirstChildElement(element, ETIQUETTE_ARBRE);
    if (racine)
    {
        for( auto child = XMLUtils::FirstChildElement(racine); child; child = XMLUtils::NextSibling(child) )
        {
            CreateAndInitNodesFromXml(child);
        }
    }
    else
    {
        throw ExceptionJeu("Etiquette de l'arbre manquant");
    }

    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ArbreRendu::CreateAndInitNodesFromXml( const XmlElement* racine )
///
/// Allows to create and initialize nodes from an xml element, default behavior
///
/// @param[in] const XmlElement * racine
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::CreateAndInitNodesFromXml( const XmlElement* child )
{
    const ArbreRendu* treeRoot = GetTreeRoot();
    if(treeRoot)
    {
        auto name = XMLUtils::GetNodeTag(child);
        auto node = treeRoot->creerNoeud(name);
        checkf(node,"Error creating node : %s",name );
        if(node)
        {
            add(node);
            try
            {
                if(!node->initFromXml(child))
                {
                    throw ExceptionJeu("Initialisation error in node");
                }
            }
            catch(ExceptionJeu&)
            {
                erase(node);
            }
        }
    }
}


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////
