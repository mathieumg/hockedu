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
        ajouterUsine(RAZER_KEY_WALL , new UsineNoeudMuret());
    }
    else
    {
        ajouterUsine( RAZER_KEY_WALL, new UsineNodeWallEdition() );
    }

    ajouterUsine(RAZER_KEY_TABLE, new UsineNoeudTable());
    ajouterUsine(RAZER_KEY_PORTAL , new UsineNoeudPortail());
    ajouterUsine(RAZER_KEY_PUCK, new UsineNoeudRondelle(limitPuck));
    ajouterUsine(RAZER_KEY_MALLET, new UsineNoeudMaillet(limitMallet));
    ajouterUsine(RAZER_KEY_BOOST, new UsineNoeudAccelerateur());
    ajouterUsine(RAZER_KEY_GROUP, new UsineNoeudGroupe());
    ajouterUsine(RAZER_KEY_BONUS, new UsineNodeBonus());
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
        n = getChild(i);
        if(n->getKey() == RAZER_KEY_TABLE)
            return (NoeudTable*)n;
        
    }
    return NULL;
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
