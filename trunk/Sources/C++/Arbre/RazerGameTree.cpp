///////////////////////////////////////////////////////////////////////////
/// @file RazerGameTree.cpp
/// @author Martin Bisson
/// @date 2007-03-23
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#include "RazerGameTree.h"
#include "UsineNoeud.h"
#include "NoeudGroupe.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn RazerGameTree::RazerGameTree()
///
/// Ce constructeur crée toutes les usines qui seront utilisées par le
/// projet et les enregistre auprès de la classe de base.
/// Il crée également la structure de base de l'arbre de rendu, c'est-à-dire
/// avec les noeuds structurants.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
RazerGameTree::RazerGameTree(class Terrain* pField):
    Super(pField)
{
	// Construction des usines
    ajouterUsine(RazerGameUtilities::NOM_MURET , new UsineNoeudMuret(RazerGameUtilities::NOM_MURET));
    ajouterUsine(RazerGameUtilities::NAME_RELATIVE_WALL , new UsineNodeWallEdition(RazerGameUtilities::NAME_RELATIVE_WALL));
	ajouterUsine(RazerGameUtilities::NOM_TABLE , new UsineNoeudTable(RazerGameUtilities::NOM_TABLE));
	ajouterUsine(RazerGameUtilities::NOM_PORTAIL , new UsineNoeudPortail(RazerGameUtilities::NOM_PORTAIL));
	ajouterUsine(RazerGameUtilities::NOM_RONDELLE , new UsineNoeudRondelle(RazerGameUtilities::NOM_RONDELLE));
	ajouterUsine(RazerGameUtilities::NOM_MAILLET , new UsineNoeudMaillet(RazerGameUtilities::NOM_MAILLET));
	ajouterUsine(RazerGameUtilities::NOM_ACCELERATEUR, new UsineNoeudAccelerateur(RazerGameUtilities::NOM_ACCELERATEUR));
    ajouterUsine(RazerGameUtilities::NOM_GROUPE, new UsineNoeudGroupe(RazerGameUtilities::NOM_GROUPE));
    ajouterUsine(RazerGameUtilities::NAME_CONTROL_POINT, new UsineNoeudGroupe(RazerGameUtilities::NAME_CONTROL_POINT));
    //ajouterUsine(RazerGameUtilities::NAME_POLYGONE, new UsineNodePolygone(RazerGameUtilities::NAME_POLYGONE));
}


////////////////////////////////////////////////////////////////////////
///
/// @fn RazerGameTree::~RazerGameTree()
///
/// Ce destructeur ne fait rien pour le moment.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
RazerGameTree::~RazerGameTree()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudTable* RazerGameTree::obtenirTable()
///
/// Retourne le noeud de l'arbre de rendu représentant la table.
///
/// @return Le noeud représentant la table. NULL si non presente
///
////////////////////////////////////////////////////////////////////////
NoeudTable* RazerGameTree::obtenirTable()
{
	int nbrEnfants = obtenirNombreEnfants();
	const NoeudAbstrait* n;

	for (int i = 0; i < nbrEnfants ; ++i)
	{
		n = chercher(i);
		if(n->obtenirType() == RazerGameUtilities::NOM_TABLE)
			return (NoeudTable*)n;
		
	}
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
