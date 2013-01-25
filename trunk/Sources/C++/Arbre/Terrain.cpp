///////////////////////////////////////////////////////////////////////////////
/// @file Terrain.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "UtilitaireINF2990.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif

#include "Terrain.h"
#include "ArbreRenduINF2990.h"
#include "ArbreNoeudLibre.h"
#include "ZoneEdition.h"
#include "NoeudTable.h"
#include "NoeudGroupe.h"
#include "ZoneEdition.h"
#include "ConfigScene.h"
#include "AideCollision.h"
#include "NoeudBut.h"
#include "utilitaire.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "NoeudPiece.h"
#include "XMLUtils.h"
#include "FacadeModele.h"
#include "NoeudAccelerateur.h"
#include "NoeudPortail.h"
#include "VisiteurFunction.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  Terrain::Terrain()
///
/// Constructeur par paramètre.
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Terrain::Terrain(): arbreRendu_(NULL), arbreAjoutNoeud_(NULL), table_(NULL),nom_(""),initialise_(false),arbreAffichage_(0)
{
	arbreAffichage_ = new ArbreRenduINF2990();
	NoeudAbstrait* piece = new NoeudPiece(ArbreRenduINF2990::NOM_PIECE);
	arbreAffichage_->ajouter(piece);
	
// 	std::vector<Vecteur3> vec;
// 	vec.push_back(Vecteur3(-1200,-538,-100.51) );
// 	vec.push_back(Vecteur3(-1155,-505,-100.51) );
// 	vec.push_back(Vecteur3(-1190,-462,-100.51) );
// 	vec.push_back(Vecteur3(-1157,-380,-100.51) );
// 
// 	vec.push_back(Vecteur3(-1200,-538,20.16) );
// 	vec.push_back(Vecteur3(-1155,-505,20.16) );
// 	vec.push_back(Vecteur3(-1190,-462,20.16) );
// 	vec.push_back(Vecteur3(-1157,-380,20.16) );
// 
// 	vec.push_back(Vecteur3(-1200,-538,140.83) );
// 	vec.push_back(Vecteur3(-1155,-505,140.83) );
// 	vec.push_back(Vecteur3(-1190,-462,140.83) );
// 	vec.push_back(Vecteur3(-1157,-380,140.83) );
// 
// 	for(unsigned int i = 0; i<vec.size(); ++i)
// 	{
// 		NoeudAbstrait* maillet = new NoeudMailletMeuble();
// 		maillet->assignerPositionRelative(vec[i]);
// 		maillet->modifierEchelleCourante(Vecteur3(3,3,2.5));
// 		piece->ajouter(maillet);
// 	}
// 	vec.clear();
// 
// 	for(int i=0; i<5; ++i)
// 	{
// 		vec.push_back(Vecteur3( -1167+(rand()%15-7),-520+(rand()%15-7),270.78+2*4*i+0.8*4) );
// 		vec.push_back(Vecteur3( -1190+(rand()%15-7),-462+(rand()%15-7),270.78+2*4*i+0.8*4) );
// 		vec.push_back(Vecteur3( -1157+(rand()%15-7),-380+(rand()%15-7),270.78+2*4*i+0.8*4) );
// 	}
// 	for(int i=0; i<5; ++i)
// 	{
// 		vec.push_back(Vecteur3( -1167+(rand()%15-7),-520+(rand()%15-7),405.37+2*4*i+0.8*4) );
// 		vec.push_back(Vecteur3( -1190+(rand()%15-7),-462+(rand()%15-7),405.37+2*4*i+0.8*4) );
// 		vec.push_back(Vecteur3( -1157+(rand()%15-7),-380+(rand()%15-7),405.37+2*4*i+0.8*4) );
// 	}
// 	for(int i=0; i<5; ++i)
// 	{
// 		vec.push_back(Vecteur3( -1167+(rand()%15-7),-520+(rand()%15-7),535.32+2*4*i+0.8*4) );
// 		vec.push_back(Vecteur3( -1190+(rand()%15-7),-462+(rand()%15-7),535.32+2*4*i+0.8*4) );
// 		vec.push_back(Vecteur3( -1157+(rand()%15-7),-380+(rand()%15-7),535.32+2*4*i+0.8*4) );
// 	}
// 	for(unsigned int i = 0; i<vec.size(); ++i)
// 	{
// 		NoeudAbstrait* rondelle = new NoeudRondelleMeuble();
// 		rondelle->assignerPositionRelative(vec[i]);
// 		rondelle->modifierEchelleCourante(Vecteur3(4,4,4));
// 		piece->ajouter(rondelle);
// 	}
// 
// 	vec.clear();
// 
// // 	float posX = -1207;
// // 	float posY = 337;
// // 
// // 	while(posY < 555)
// // 	{
// // 		vec.push_back(Vecteur3( -1207+(rand()%15-7),-520+(rand()%15-7),270.78+2*4*i+0.8*4) );
// // 		vec.push_back(Vecteur3( -1190+(rand()%15-7),-462+(rand()%15-7),270.78+2*4*i+0.8*4) );
// // 		vec.push_back(Vecteur3( -1157+(rand()%15-7),-380+(rand()%15-7),270.78+2*4*i+0.8*4) );
// // 	}

}


////////////////////////////////////////////////////////////////////////
///
/// @fn  Terrain::~Terrain(  )
///
/// Destructeur, libération de la mémoire
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Terrain::~Terrain()
{
	libererMemoire();
	if(arbreAffichage_)
	{
		arbreAffichage_->vider();
		delete arbreAffichage_;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::libererMemoire()
///
/// Libère la mémoire du terrain et le retourne à son état de base
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::libererMemoire()
{
	// S'assurer de remettre tous les pointeurs a NULL car la comparaison est utiliser 
	// partout dans le terrain pour savoir si le pointeur est valide
	if(arbreRendu_ != NULL)
	{
		// Libération de l'arbre de rendu complet
		arbreRendu_->vider();
		delete arbreRendu_;
		arbreRendu_ = NULL;
	}
	if(arbreAjoutNoeud_ != NULL)
	{
		// Ses enfants ne seront pas libérés, mais le terrain n'en n'est pas responsable
		arbreAjoutNoeud_->vider();
		delete arbreAjoutNoeud_;
		arbreAjoutNoeud_ = NULL;
	}
	// On ne libère pas la table, car elle est un enfant de l'arbre de rendu
	table_ = NULL;
	
	nom_ = "";
	initialise_ = false;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::afficherTerrain()
///
/// Permet d'effectuer le rendu des arbres du terrain
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::afficherTerrain( bool afficherZoneEdition /*= false*/ )
{
	if(arbreRendu_)
		arbreRendu_->afficher();
	if(arbreAjoutNoeud_)
		arbreAjoutNoeud_->afficher();
	if(afficherZoneEdition)
		getZoneEdition().afficher();
	if(arbreAffichage_)
		arbreAffichage_->afficher();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiser( std::string nom )
///
/// Permet d'initialiser le terrain avec ces éléments de bases pour le terrain
/// Permet de reintialiser en meme temps
///
/// @param[in] std::string nom : nom du terrain est en fait le chemin pour la sauvegarde en XML
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::initialiser( std::string nom )
{
	// On s'assure que les éléments du terrain sont libéré avant de les écraser
	libererMemoire();
	// Doit rester la premiere chose que l'on fait dans cette méthode sinon on pourrait invalider des pointeurs
	///////////////////////////////////////////////////////////////////////////

	// Assignation du nom du terrain
	nom_ = nom;
	// Initialisation de la Zone d'edition
	getZoneEdition().reinitialiser();

	// Initialisation des arbres de rendus
	arbreAjoutNoeud_ = new ArbreNoeudLibre();

	initialiserArbreRendu();
	
	initialise_ = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiserArbreRendu()
///
/// Methode pour initialiser l'arbre de rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::initialiserArbreRendu()
{
	if(arbreRendu_ == NULL)
		arbreRendu_ = new ArbreRenduINF2990();
	else
		arbreRendu_->vider();
    arbreRendu_->modifierTerrain(this);

	// Ajout d'une table de base au terrain
	table_ = new NoeudTable(ArbreRenduINF2990::NOM_TABLE);
	arbreRendu_->ajouter(table_);


	/// Groupe destine a contenir les noeud concret pour un meilleur parcours d'arbre
	NoeudGroupe* 	gMaillet =	new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_MAILLET),
		*gRondelle =	new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_RONDELLE),
		*gAccel =		new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_ACCELERATEUR),
		*gMuret =		new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_MURET),
		*gPortail =		new NoeudGroupe(ArbreRenduINF2990::NOM_GROUPE,ArbreRenduINF2990::NOM_PORTAIL);

	// La table contient ces groupes.
	table_->ajouter(gRondelle);
	table_->ajouter(gMaillet);
	table_->ajouter(gAccel);
	table_->ajouter(gMuret);
	table_->ajouter(gPortail);

	// Permet de rediriger les bandes extérieur de la table vers le groupe  gMuret
	table_->reassignerParentBandeExt();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiserXml( TiXmlElement* element )
///
/// Permet d'initialiser le terrain avec ces éléments a partir d'un noeud XML
///
/// @param[in] TiXmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::initialiserXml( TiXmlElement* element )
{
	libererMemoire();
	
	// Initialisation des arbres de rendus
	arbreAjoutNoeud_ = new ArbreNoeudLibre();
	arbreRendu_ = new ArbreRenduINF2990();

	TiXmlElement* racine = element->FirstChildElement("Terrain");
	if(!racine)
		return false;
	const char* nom = racine->Attribute("nom");
	if(nom == 0)
		return false;
	nom_ = nom;

	try
	{
        // l'arbre doit connaitre le terrain avant de s'initialiser pour avoir acces a celui durant l'initialisation
		arbreRendu_->modifierTerrain(this);

		ConfigScene::obtenirInstance()->lireDOM(*racine,arbreRendu_);
		table_ = arbreRendu_->obtenirTable();
		if(table_ == NULL)
			throw std::runtime_error("Il ny a pas de table sur le terrain");
		table_->reassignerParentBandeExt();
        fullRebuild();
	}
	catch(std::runtime_error& e)
	{
		// Erreur dans le fichier XML, on remet l'arbre de base
		initialiserArbreRendu();
        utilitaire::afficherErreur(e.what());
	}
	catch(...)
	{
		// Erreur dans le fichier XML, on remet l'arbre de base
		initialiserArbreRendu();
	}

	if(!getZoneEdition().initialisationXML(racine))
		return false;

	
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::reinitialiser()
///
/// Remet le terrain a son etat de base
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::reinitialiser()
{
	
	if(!arbreAjoutNoeud_)
		arbreAjoutNoeud_ = new ArbreNoeudLibre();
	else
		arbreAjoutNoeud_->vider();

	initialiserArbreRendu();
	getZoneEdition().reinitialiser();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* Terrain::creerNoeudXML()
///
/// Creation du noeud XML du Terrain
///
///
/// @return TiXmlElement*
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* Terrain::creerNoeudXML()
{
	// Créer le noeud 
	TiXmlElement* racine = XMLUtils::creerNoeud("Terrain");

	racine->SetAttribute("nom",getNom().c_str() );

	if(getArbreRendu())
	{
		getArbreRendu()->deselectionnerTout();
		// Creation du domaine de l'arbre de rendu
		ConfigScene::obtenirInstance()->creerDOM(*racine,getArbreRendu());
	}

	racine->LinkEndChild(getZoneEdition().creerNoeudXML());

	return racine;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::animerTerrain( const float& temps )
///
/// Permet d'animer les noeuds des arbres du terrain, la physique reste géré par FacadeModele
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::animerTerrain( const float& temps )
{
	if(arbreRendu_)
		arbreRendu_->animer(temps);
	if(arbreAjoutNoeud_)
		arbreAjoutNoeud_->animer(temps);
	if(arbreAffichage_)
		arbreAffichage_->animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::ajouterNoeudTemp( const NoeudAbstrait* noeud )
///
/// Ajout d'un noeud dans l'arbre de noeud Temporaire
///
/// @param[in] const NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::ajouterNoeudTemp( NoeudAbstrait* noeud )
{
	arbreAjoutNoeud_->ajouter(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::transfererNoeud( NoeudAbstrait* noeud )
///
/// Transfert d'un noeud de l'arbre de noeud temporaire à l'arbre de rendu
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::transfererNoeud( NoeudAbstrait* noeud )
{
	arbreAjoutNoeud_->detacherEnfant(noeud);
	table_->ajouter(noeud);
    noeud->forceFullUpdate();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::retirerNoeudTemp( NoeudAbstrait* noeud )
///
/// Permet de detacher un noeud temporaire de l'arbre de noeud temporaire
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::retirerNoeudTemp( NoeudAbstrait* noeud )
{
	arbreAjoutNoeud_->detacherEnfant(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::insideLimits( NoeudAbstrait* noeud )
///
/// Vérification pour voir si un noeud est dans les limites de la zone edition
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return bool Vrai si le noeud est dans les limites de la zone
///
////////////////////////////////////////////////////////////////////////
bool Terrain::insideLimits( NoeudAbstrait* noeud )
{
	Vecteur3 pos = noeud->obtenirPositionAbsolue();

	// Cas particulier pour des muret puisque ce sont des segment et non des cercles
	if(noeud->obtenirType() == ArbreRenduINF2990::NOM_MURET)
	{
		NoeudMuret *muret = dynamic_cast<NoeudMuret *>(noeud);
		if (muret)
		{
			// Ligne du haut
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(-getZoneEdition().obtenirLimiteExtLongueur(),getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(getZoneEdition().obtenirLimiteExtLongueur(),getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne de droite
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(getZoneEdition().obtenirLimiteExtLongueur(),getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(getZoneEdition().obtenirLimiteExtLongueur(),-getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne du bas
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(getZoneEdition().obtenirLimiteExtLongueur(),-getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(-getZoneEdition().obtenirLimiteExtLongueur(),-getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne de Gauche
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(-getZoneEdition().obtenirLimiteExtLongueur(),-getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(-getZoneEdition().obtenirLimiteExtLongueur(),getZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
		}
	}
	// Tests sur les positions avec leurs rayons beaucoup plus simple
	// sert aussi de float check pour les murets car leur rayon est nulle
	if(pos[VX]+noeud->obtenirRayon() > getZoneEdition().obtenirLimiteExtLongueur() || pos[VX]-noeud->obtenirRayon() < -getZoneEdition().obtenirLimiteExtLongueur())
		return false;
	if(pos[VY]+noeud->obtenirRayon() > getZoneEdition().obtenirLimiteExtLargeur() || pos[VY]-noeud->obtenirRayon() < -getZoneEdition().obtenirLimiteExtLargeur())
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::creerTerrainParDefaut()
///
/// Création d'un terrain par défaut.
/// Celui-ci a les dimemsions de base, 2 maillets et une rondelle
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::creerTerrainParDefaut(std::string nom)
{
	initialiser(nom);
	NoeudAbstrait* maillet1 = getArbreRendu()->creerNoeud(ArbreRenduINF2990::NOM_MAILLET);
	NoeudAbstrait* maillet2 = getArbreRendu()->creerNoeud(ArbreRenduINF2990::NOM_MAILLET);
	NoeudAbstrait* rondelle = getArbreRendu()->creerNoeud(ArbreRenduINF2990::NOM_RONDELLE);

	maillet1->assignerPositionRelative(table_->obtenirPoint(POSITION_MILIEU_GAUCHE)->obtenirPositionAbsolue()/2.0);
	maillet2->assignerPositionRelative(table_->obtenirPoint(POSITION_MILIEU_DROITE)->obtenirPositionAbsolue()/2.0);
	rondelle->assignerPositionRelative(Vecteur3(0.0,0.0,0.0));

	table_->ajouter(maillet1);
	table_->ajouter(maillet2);
	table_->ajouter(rondelle);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::verifierValidite()
///
/// Verifie si le terrain respecte les conditions de jeu soit 2 maillet, 1 de chaque cote
/// de le table et une rondelle. Les elements a l'exterieur de la table sont eliminer
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::verifierValidite( bool afficherErreur /*= true*/ )
{
	if(!table_)
	{
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\naucune table presente sur le terrain");
		return false;
	}
	bool mailletGaucheOk = false, mailletDroiteOk = false, rondelleOk = false;

	// on prend un des 2 buts
	if(!table_->obtenirBut(1))
	{
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\naucun buts presents sur le terrain");
		return false;
	}
	float hauteurBut = table_->obtenirBut(1)->obtenirHauteurBut();

	const unsigned int nbTypeModifiable = 5;
	std::string typeNoeudModifiable[] = {
		ArbreRenduINF2990::NOM_RONDELLE,
		ArbreRenduINF2990::NOM_MAILLET,
		ArbreRenduINF2990::NOM_ACCELERATEUR,
		ArbreRenduINF2990::NOM_PORTAIL,
		ArbreRenduINF2990::NOM_MURET,
	};
	
	// Suppression des noeuds qui ne sont pas sur la table
	for(unsigned int i=0; i<nbTypeModifiable; ++i)
	{
		NoeudComposite* g = (NoeudComposite*)table_->obtenirGroupe(typeNoeudModifiable[i]);
		if(g)
		{
			for(unsigned int j=0; j<g->obtenirNombreEnfants(); ++j)
			{
				NoeudAbstrait* n = g->chercher(j);
				// On verifie que le type est bon, surtout utile pour les bande exterieur qui ont un nom different et doivent etre ignorer par cette methode
				if(n->obtenirType() == typeNoeudModifiable[i])
				{
					if(!table_->estSurTable(n) )
					{
						// La suppression du noeud l'enlevera du groupe
						g->effacer(n);
						// Repositionnement de i pour pointer au bon endroit a la prochaine iteration
						j--; 
					}
				}
			}
		}
	}

	// Verification des maillets
	NoeudComposite* g = (NoeudComposite*)table_->obtenirGroupe(ArbreRenduINF2990::NOM_MAILLET);
	if(!g)
		return false;
	for(unsigned int j=0; j<g->obtenirNombreEnfants(); ++j)
	{
		NoeudMaillet *maillet = dynamic_cast<NoeudMaillet *>(g->chercher(j));
		if (maillet)
		{
			// verification des maillets
			if(maillet->obtenirPositionAbsolue()[VX] <= 0)
			{
				if(mailletGaucheOk)
				{
					if(afficherErreur)
						utilitaire::afficherErreur("Erreur: table invalide\n2 Maillets sur le cote gauche de la table");
					return false;
				}
				mailletGaucheOk = true;
				maillet->modifierPositionOriginale(maillet->obtenirPositionRelative());
			}
			else
			{
				if(mailletDroiteOk)
				{
					if(afficherErreur)
						utilitaire::afficherErreur("Erreur: table invalide\n2 Maillets sur le cote droite de la table");
					return false;
				}
				mailletDroiteOk = true;
				maillet->modifierPositionOriginale(maillet->obtenirPositionRelative());
			}
		}
	}
	g = (NoeudComposite*)table_->obtenirGroupe(ArbreRenduINF2990::NOM_RONDELLE);
	if(!g)
		return false;
	for(unsigned int j=0; j<g->obtenirNombreEnfants(); ++j)
	{
		NoeudRondelle *rondelle = dynamic_cast<NoeudRondelle *>(g->chercher(j));
		if (rondelle)
		{
			if(rondelleOk)
			{
				if(afficherErreur)
					utilitaire::afficherErreur("Erreur: table invalide\n2 rondelles presente sur la table");
				return false;
			}
			rondelleOk = true;
			rondelle->validerPropriteteTablePourJeu();
			// On multiplie le rayon par 3 pour avoir une distance supplementaire proportionnel a la taille de la rondelle
			if(hauteurBut <= rondelle->obtenirRayon()*3)
			{
				if(afficherErreur)
                {
                    std::ostringstream mess;
                    mess << "Erreur: table invalide\nbuts trop petits pour jouer.\nDistance manquante = ";
                    mess << (float)rondelle->obtenirRayon()*2+20-hauteurBut;
                    mess << " pixels";
					utilitaire::afficherErreur(mess.str());
                }
				return false;
			}
		}

	}
	if(!rondelleOk)
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\nil n'y a pas de rondelle presente sur la table");
	if(!mailletGaucheOk)
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\nil n'y a pas de maillet present sur le cote gauche de la table");
	if(!mailletDroiteOk)
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\nil n'y a pas de maillet present sur le cote droit de la table");

	return rondelleOk && mailletDroiteOk && mailletGaucheOk;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle* Terrain::getRondelle()
///
/// Accesseur de la rondelle
///
///
/// @return NoeudRondelle* : pointeur sur la rondelle s'il la trouve, 0 sinon
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle* Terrain::getRondelle() const
{
	if(getTable())
	{
		NoeudComposite* g = (NoeudComposite*)getTable()->obtenirGroupe(ArbreRenduINF2990::NOM_RONDELLE);
		if(g)
		{
			for(unsigned int i=0; i<g->obtenirNombreEnfants(); ++i)
			{
				NoeudRondelle* r = dynamic_cast<NoeudRondelle *>(g->chercher(i));
				if(r)
					return r;
			}
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::appliquerPhysique()
///
/// Applique la physique sur l'arbre de rendu contenant les noeuds de la table
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::appliquerPhysique( float temps )
{
	if(arbreRendu_)
	{
#if BOX2D_INTEGRATED
        /// TODO:: cache pointer to mallet for field to access directly
        NoeudMaillet* mailletGauche = FacadeModele::getInstance()->obtenirMailletJoueurDroit();
        NoeudMaillet* mailletDroit = FacadeModele::getInstance()->obtenirMailletJoueurGauche();

        if(mailletDroit)
        {
            mailletDroit->preSimulationActions();
        }
        if(mailletGauche)
        {
            mailletGauche->preSimulationActions();
        }

        FacadeModele::getInstance()->getWorld()->SetWarmStarting(true);
        FacadeModele::getInstance()->getWorld()->SetContinuousPhysics(true);
        FacadeModele::getInstance()->getWorld()->SetSubStepping(true);
        FacadeModele::getInstance()->getWorld()->Step(temps, 8, 3);
#else
		arbreRendu_->majPosition(temps);
		arbreRendu_->gestionCollision(temps);
		arbreRendu_->ajusterVitesse(temps);
		arbreRendu_->ajusterEnfoncement();
#endif
	}
}

#if BOX2D_INTEGRATED
////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::BeginContact( b2Contact* contact )
///
/// Callback before the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::BeginContact( b2Contact* contact )
{
    bool isEnabled = contact->IsEnabled();
    bool isTouching = contact->IsTouching();

    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    auto bodyA = fixtureA->GetBody();
    auto bodyB = fixtureB->GetBody();
    auto filterA = fixtureA->GetFilterData();
    auto filterB = fixtureB->GetFilterData();
    short category = filterA.categoryBits | filterB.categoryBits;
    bool isPuckPresent = !!(( category ) & CATEGORY_PUCK );

    if(isPuckPresent)
    {
        // on ne conserve que l'autre categorie
        category &= ~CATEGORY_PUCK;
        b2Body* bodies[2];
        if(filterA.categoryBits & CATEGORY_PUCK)
        {
            bodies[0] = bodyA;
            bodies[1] = bodyB;
        }
        else
        {
            bodies[0] = bodyB;
            bodies[1] = bodyA;
        }


        switch(category)
        {
        case CATEGORY_BOUNDARY:
            // TODO:: à tester
            if(bodies[0]->GetLinearVelocity().LengthSquared() > 200)
                SoundFMOD::obtenirInstance()->playEffect(COLLISION_MURET_EFFECT);
            break;
            //     case CATEGORY_PUCK    : // impossible case
            //         break;
        case CATEGORY_MALLET  :
            SoundFMOD::obtenirInstance()->playEffect(effect(COLLISION_MAILLET_EFFECT1+(rand()%5)));			
            break;
        case CATEGORY_PORTAL  :
            {
                NoeudPortail* portail = (NoeudPortail*)bodies[1]->GetUserData();
                checkf(portail, "Portal's body's User Data is not the Portal");
                
                if(portail->isAttractionFieldActive())
                {
                    // Collision avec un portail et téléportation
                    NoeudGroupe *groupe = dynamic_cast<NoeudGroupe *>(portail->obtenirParent());
                    if(groupe)
                    {
                        unsigned int nbEnfant = groupe->obtenirNombreEnfants();

                        if( nbEnfant > 1 )
                        {
                            // Choix aléatoire du portail de sortie
                            NoeudPortail* portailDeSortie = NULL;
                            while(!portailDeSortie || portailDeSortie == portail)
                            {
                                int noPortailDeSortie = rand()%nbEnfant;
                                portailDeSortie = dynamic_cast<NoeudPortail*>(groupe->chercher(noPortailDeSortie));
                            }

                            NoeudRondelle* rondelle = (NoeudRondelle*)bodies[0]->GetUserData();

                            // The new pos can only be assigned outside of the world's step, so we queue it
                            FacadeModele::getInstance()->RunOnUpdateThread(new Runnable([=](Runnable*)
                            {
                                portailDeSortie->setIsAttractionFieldActive(false);
                                auto newPos = portailDeSortie->obtenirPositionRelative();
                                rondelle->assignerPositionRelative(newPos);
                                SoundFMOD::obtenirInstance()->playEffect(effect(PORTAL_EFFECT));
                            }),true);
                            return;
                        }
                    }
                }
            }
            break;
        case CATEGORY_BOOST   :
            {
                NoeudAccelerateur* accelerateur = (NoeudAccelerateur*)bodies[1]->GetUserData();
                checkf(accelerateur, "Boost's body's User Data is not the boost");

                auto bonusAccel = accelerateur->obtenirBonusAccel();
                auto linearVelocity = bodies[0]->GetLinearVelocity();

                linearVelocity *= bonusAccel;
                bodies[0]->SetLinearVelocity(linearVelocity);

                SoundFMOD::obtenirInstance()->playEffect(effect(ACCELERATOR_EFFECT));
            }
            break;
        default:
            break;
        }
    }
    
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::EndContact( b2Contact* contact )
///
/// Callback after the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::EndContact( b2Contact* contact )
{
    bool isEnabled = contact->IsEnabled();
    bool isTouching = contact->IsTouching();

    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    auto bodyA = fixtureA->GetBody();
    auto bodyB = fixtureB->GetBody();
    auto filterA = fixtureA->GetFilterData();
    auto filterB = fixtureB->GetFilterData();
    short category = filterA.categoryBits | filterB.categoryBits;
    bool isPuckPresent = !!(( category ) & CATEGORY_PUCK );

    // la rondelle est sortie d'un portal
    if( category == (CATEGORY_PUCK|CATEGORY_PORTAL) )
    {
        b2Body* portalBody = NULL;
        if(filterA.categoryBits & CATEGORY_PORTAL)
        {
            portalBody = bodyA;
        }
        else
        {
            portalBody = bodyB;
        }

        NoeudPortail* portal = (NoeudPortail*)portalBody->GetUserData();
        checkf(portal, "Portal's body's User Data is not the Portal");

        portal->setIsAttractionFieldActive(true);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
///
/// Callback before the solving the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
/// @param[in] const b2Manifold * oldManifold
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
    B2_NOT_USED(contact);
    B2_NOT_USED(oldManifold);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
///
/// Callback after the solving the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
/// @param[in] const b2ContactImpulse * impulse
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
    B2_NOT_USED(contact);
    B2_NOT_USED(impulse);
}
#endif


////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::getGoals( NoeudBut** pOutGoals )
///
/// /*Description*/
///
/// @param[in] NoeudBut * * pOutGoals
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::getGoals( NoeudBut** pOutGoals )
{
    pOutGoals[0] = NULL;
    pOutGoals[1] = NULL;

    NoeudTable* table = getTable();
    if(table)
    {
        pOutGoals[0] = table->obtenirBut(1);
        pOutGoals[1] = table->obtenirBut(2);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::fullRebuild()
///
/// Updates the content of the game to be ready to play
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::fullRebuild()
{
    checkf(arbreRendu_,"Requete pour un full rebuild d'un terrain sans arbre");
    if(arbreRendu_)
    {
        VisiteurFunction v([](NoeudAbstrait* n)
        {
            n->forceFullUpdate();
        });
        arbreRendu_->acceptVisitor(v);
    }
}



///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////