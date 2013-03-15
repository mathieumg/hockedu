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
#include "Terrain.h"

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
RazerGameTree::RazerGameTree(class Terrain* pField,unsigned int limitMallet, unsigned int limitPuck):
    Super(pField)
{
	// Construction des usines
    if(pField && pField->IsGameField())
    {
        ajouterUsine(RazerGameUtilities::NOM_MURET , new UsineNoeudMuret(RazerGameUtilities::NOM_MURET));
    }
    else
    {
        ajouterUsine(RazerGameUtilities::NOM_MURET , new UsineNodeWallEdition(RazerGameUtilities::NOM_MURET));
    }


	ajouterUsine(RazerGameUtilities::NOM_TABLE , new UsineNoeudTable(RazerGameUtilities::NOM_TABLE));
	ajouterUsine(RazerGameUtilities::NOM_PORTAIL , new UsineNoeudPortail(RazerGameUtilities::NOM_PORTAIL));
	ajouterUsine(RazerGameUtilities::NOM_RONDELLE , new UsineNoeudRondelle(RazerGameUtilities::NOM_RONDELLE,limitPuck));
	ajouterUsine(RazerGameUtilities::NOM_MAILLET , new UsineNoeudMaillet(RazerGameUtilities::NOM_MAILLET,limitMallet));
	ajouterUsine(RazerGameUtilities::NOM_ACCELERATEUR, new UsineNoeudAccelerateur(RazerGameUtilities::NOM_ACCELERATEUR));
    ajouterUsine(RazerGameUtilities::NOM_GROUPE, new UsineNoeudGroupe(RazerGameUtilities::NOM_GROUPE));
    ajouterUsine(RazerGameUtilities::NAME_EMPTY_BONUS, new UsineNodeBonus(RazerGameUtilities::NAME_EMPTY_BONUS));
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
	int nbrEnfants = childCount();
	const NoeudAbstrait* n;

	for (int i = 0; i < nbrEnfants ; ++i)
	{
		n = find(i);
		if(n->getType() == RazerGameUtilities::NOM_TABLE)
			return (NoeudTable*)n;
		
	}
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
