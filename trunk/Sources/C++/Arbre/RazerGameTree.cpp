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
/// Ce constructeur cr�e toutes les usines qui seront utilis�es par le
/// projet et les enregistre aupr�s de la classe de base.
/// Il cr�e �galement la structure de base de l'arbre de rendu, c'est-�-dire
/// avec les noeuds structurants.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
RazerGameTree::RazerGameTree()
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
    ajouterUsine(RazerGameUtilities::NAME_POLYGONE, new UsineNodePolygone(RazerGameUtilities::NAME_POLYGONE));
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
/// @fn void RazerGameTree::initialiser()
///
/// Cette fonction cr�e la structure de base de l'arbre de rendu, c'est-�-dire
/// avec les noeuds structurants (pour les objets, les murs, les billes,
/// les parties statiques, etc.)
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void RazerGameTree::initialiser()
{
	// On vide l'arbre
	vider();
	NoeudTable* noeudTable = new NoeudTable(RazerGameUtilities::NOM_TABLE);
	ajouter(noeudTable);
	
	/// Groupe destine a contenir les noeud concret pour un meilleur parcours d'arbre
	NoeudGroupe* 	gMaillet =	new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_MAILLET),
		*gRondelle =	new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_RONDELLE),
		*gAccel =		new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_ACCELERATEUR),
		*gMuret =		new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_MURET),
		*gPortail =		new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_PORTAIL);
	noeudTable->ajouter(gRondelle);
	noeudTable->ajouter(gMaillet);/// Pas toucher a lordre
	noeudTable->ajouter(gAccel);
	noeudTable->ajouter(gMuret);
	noeudTable->ajouter(gPortail);

	noeudTable->reassignerParentBandeExt();	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudTable* RazerGameTree::obtenirTable()
///
/// Retourne le noeud de l'arbre de rendu repr�sentant la table.
///
/// @return Le noeud repr�sentant la table. NULL si non presente
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
