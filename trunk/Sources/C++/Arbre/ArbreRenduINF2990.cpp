///////////////////////////////////////////////////////////////////////////
/// @file ArbreRenduINF2990.cpp
/// @author Martin Bisson
/// @date 2007-03-23
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#include "ArbreRenduINF2990.h"
#include "UsineNoeudMuret.h"
#include "UsineNoeudMaillet.h"
#include "UsineNoeudPortail.h"
#include "UsineNoeudRondelle.h"
#include "UsineNoeudTable.h"
#include "UsineNoeudAccelerateur.h"
#include "UsineNoeudAccelerateur.h"
#include "UsineNoeudGroupe.h"
#include "NoeudGroupe.h"


/// La chaîne représentant le dossier.
const std::string ArbreRenduINF2990::NOM_DOSSIER = "media/";

/// La chaîne représentant l'extension.
const std::string ArbreRenduINF2990::NOM_EXTENSION = ".obj";

/// La chaîne représentant le type de la piece.
const std::string ArbreRenduINF2990::NOM_PIECE = "piece";

/// La chaîne représentant le type des buts.
const std::string ArbreRenduINF2990::NOM_BUT_MILIEU = "but_milieu";

/// La chaîne représentant le type des buts.
const std::string ArbreRenduINF2990::NOM_BUT_COTE = "but_cote";

/// La chaîne représentant le type des murets.
const std::string ArbreRenduINF2990::NOM_MURET = "muret";

/// La chaîne représentant le type des murets.
const std::string ArbreRenduINF2990::NOM_MURET_RELATIF = "muret_relatif";

/// La chaîne représentant le type des murets de la zone d'edition.
const std::string ArbreRenduINF2990::NOM_MURET_ZONE_EDITION = "muret_zone_edition";

/// La chaîne représentant le type de la table de jeu.
const std::string ArbreRenduINF2990::NOM_TABLE = "table";

/// La chaîne représentant le type des portails.
const std::string ArbreRenduINF2990::NOM_PORTAIL = "portail";

/// La chaîne représentant le type de la rondelle.
const std::string ArbreRenduINF2990::NOM_RONDELLE = "rondelle";

/// La chaîne représentant le type des maillets.
const std::string ArbreRenduINF2990::NOM_MAILLET = "maillet";

/// La chaîne représentant le type des accelerateurs.
const std::string ArbreRenduINF2990::NOM_ACCELERATEUR = "accelerateur";

/// La chaîne représentant le type des points pour le redimensionnement de la table.
const std::string ArbreRenduINF2990::NOM_POINT = "point";

/// La chaîne représentant le type des groupes
const std::string ArbreRenduINF2990::NOM_GROUPE = "Groupe";


////////////////////////////////////////////////////////////////////////
///
/// @fn ArbreRenduINF2990::ArbreRenduINF2990()
///
/// Ce constructeur crée toutes les usines qui seront utilisées par le
/// projet de INF2990et les enregistre auprès de la classe de base.
/// Il crée également la structure de base de l'arbre de rendu, c'est-à-dire
/// avec les noeuds structurants.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
ArbreRenduINF2990::ArbreRenduINF2990()
{
	// Construction des usines
	ajouterUsine(NOM_MURET , new UsineNoeudMuret(NOM_MURET));
	ajouterUsine(NOM_TABLE , new UsineNoeudTable(NOM_TABLE));
	ajouterUsine(NOM_PORTAIL , new UsineNoeudPortail(NOM_PORTAIL));
	ajouterUsine(NOM_RONDELLE , new UsineNoeudRondelle(NOM_RONDELLE));
	ajouterUsine(NOM_MAILLET , new UsineNoeudMaillet(NOM_MAILLET));
	ajouterUsine(NOM_ACCELERATEUR, new UsineNoeudAccelerateur(NOM_ACCELERATEUR));
	ajouterUsine(NOM_GROUPE, new UsineNoeudGroupe(NOM_GROUPE));
	//ajouterUsine(NOM_POINT, new UsineNoeudPoint(NOM_POINT));
}


////////////////////////////////////////////////////////////////////////
///
/// @fn ArbreRenduINF2990::~ArbreRenduINF2990()
///
/// Ce destructeur ne fait rien pour le moment.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
ArbreRenduINF2990::~ArbreRenduINF2990()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ArbreRenduINF2990::initialiser()
///
/// Cette fonction crée la structure de base de l'arbre de rendu, c'est-à-dire
/// avec les noeuds structurants (pour les objets, les murs, les billes,
/// les parties statiques, etc.)
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ArbreRenduINF2990::initialiser()
{
	// On vide l'arbre
	vider();
	NoeudTable* noeudTable = new NoeudTable(NOM_TABLE);
	ajouter(noeudTable);
	

	/// Groupe destine a contenir les noeud concret pour un meilleur parcours d'arbre
	NoeudGroupe* 	gMaillet =	new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_MAILLET),
		*gRondelle =	new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_RONDELLE),
		*gAccel =		new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_ACCELERATEUR),
		*gMuret =		new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_MURET),
		*gPortail =		new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_PORTAIL);
	// Ajout des groupes a la facon d'un noeudcomposite normal,
	// car la methode ajouter de table est redefinie pour mettre 
	// les noeuds dans ces groupes plutot que dans la table
	noeudTable->ajouter(gRondelle);
	noeudTable->ajouter(gMaillet);/// Pas toucher a lordre
	noeudTable->ajouter(gAccel);
	noeudTable->ajouter(gMuret);
	noeudTable->ajouter(gPortail);

	noeudTable->reassignerParentBandeExt();	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudTable* ArbreRenduINF2990::obtenirTable()
///
/// Retourne le noeud de l'arbre de rendu représentant la table.
///
/// @return Le noeud représentant la table. NULL si non presente
///
////////////////////////////////////////////////////////////////////////
NoeudTable* ArbreRenduINF2990::obtenirTable()
{
	int nbrEnfants = obtenirNombreEnfants();
	const NoeudAbstrait* n;

	for (int i = 0; i < nbrEnfants ; ++i)
	{
		n = chercher(i);
		if(n->obtenirType() == NOM_TABLE)
			return (NoeudTable*)n;
		
	}
	throw std::runtime_error("Table non presente dans l'arbre de rendu");
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
