///////////////////////////////////////////////////////////////////////////////
/// @file Terrain.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////
///
/// @fn  Terrain::Terrain()
///
/// Constructeur par param�tre.
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
// // 	double posX = -1207;
// // 	double posY = 337;
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
/// Destructeur, lib�ration de la m�moire
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
/// Lib�re la m�moire du terrain et le retourne � son �tat de base
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
		// Lib�ration de l'arbre de rendu complet
		arbreRendu_->vider();
		delete arbreRendu_;
		arbreRendu_ = NULL;
	}
	if(arbreAjoutNoeud_ != NULL)
	{
		// Ses enfants ne seront pas lib�r�s, mais le terrain n'en n'est pas responsable
		arbreAjoutNoeud_->vider();
		delete arbreAjoutNoeud_;
		arbreAjoutNoeud_ = NULL;
	}
	// On ne lib�re pas la table, car elle est un enfant de l'arbre de rendu
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
		obtenirZoneEdition().afficher();
	if(arbreAffichage_)
		arbreAffichage_->afficher();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiser( std::string nom )
///
/// Permet d'initialiser le terrain avec ces �l�ments de bases pour le terrain
/// Permet de reintialiser en meme temps
///
/// @param[in] std::string nom : nom du terrain est en fait le chemin pour la sauvegarde en XML
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::initialiser( std::string nom )
{
	// On s'assure que les �l�ments du terrain sont lib�r� avant de les �craser
	libererMemoire();
	// Doit rester la premiere chose que l'on fait dans cette m�thode sinon on pourrait invalider des pointeurs
	///////////////////////////////////////////////////////////////////////////

	// Assignation du nom du terrain
	nom_ = nom;
	// Initialisation de la Zone d'edition
	obtenirZoneEdition().reinitialiser();

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

	// Permet de rediriger les bandes ext�rieur de la table vers le groupe  gMuret
	table_->reassignerParentBandeExt();

	arbreRendu_->modifierTerrain(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiserXml( TiXmlElement* element )
///
/// Permet d'initialiser le terrain avec ces �l�ments a partir d'un noeud XML
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
		ConfigScene::obtenirInstance()->lireDOM(*racine,arbreRendu_);
		arbreRendu_->modifierTerrain(this);
		table_ = arbreRendu_->obtenirTable();
		if(table_ == NULL)
			throw std::runtime_error("Il ny a pas de table sur le terrain");
		table_->reassignerParentBandeExt();
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

	if(!obtenirZoneEdition().initialisationXML(racine))
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
	obtenirZoneEdition().reinitialiser();
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
	// Cr�er le noeud 
	TiXmlElement* racine = XMLUtils::creerNoeud("Terrain");

	racine->SetAttribute("nom",obtenirNom().c_str() );

	if(getArbreRendu())
	{
		getArbreRendu()->deselectionnerTout();
		// Creation du domaine de l'arbre de rendu
		ConfigScene::obtenirInstance()->creerDOM(*racine,getArbreRendu());
	}

	racine->LinkEndChild(obtenirZoneEdition().creerNoeudXML());

	return racine;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::animerTerrain( const float& temps )
///
/// Permet d'animer les noeuds des arbres du terrain, la physique reste g�r� par FacadeModele
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
/// Transfert d'un noeud de l'arbre de noeud temporaire � l'arbre de rendu
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
/// V�rification pour voir si un noeud est dans les limites de la zone edition
///
/// @param[in] NoeudAbstrait * noeud : noeud � v�rifier
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
				Vecteur2(-obtenirZoneEdition().obtenirLimiteExtLongueur(),obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(obtenirZoneEdition().obtenirLimiteExtLongueur(),obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne de droite
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(obtenirZoneEdition().obtenirLimiteExtLongueur(),obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(obtenirZoneEdition().obtenirLimiteExtLongueur(),-obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne du bas
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(obtenirZoneEdition().obtenirLimiteExtLongueur(),-obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(-obtenirZoneEdition().obtenirLimiteExtLongueur(),-obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne de Gauche
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(-obtenirZoneEdition().obtenirLimiteExtLongueur(),-obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2(-obtenirZoneEdition().obtenirLimiteExtLongueur(),obtenirZoneEdition().obtenirLimiteExtLargeur()),
				Vecteur2()	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
		}
	}
	// Tests sur les positions avec leurs rayons beaucoup plus simple
	// sert aussi de double check pour les murets car leur rayon est nulle
	if(pos[VX]+noeud->obtenirRayon() > obtenirZoneEdition().obtenirLimiteExtLongueur() || pos[VX]-noeud->obtenirRayon() < -obtenirZoneEdition().obtenirLimiteExtLongueur())
		return false;
	if(pos[VY]+noeud->obtenirRayon() > obtenirZoneEdition().obtenirLimiteExtLargeur() || pos[VY]-noeud->obtenirRayon() < -obtenirZoneEdition().obtenirLimiteExtLargeur())
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::creerTerrainParDefaut()
///
/// Cr�ation d'un terrain par d�faut.
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
	double hauteurBut = table_->obtenirBut(1)->obtenirHauteurBut();

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
/// @fn NoeudRondelle* Terrain::obtenirRondelle()
///
/// Accesseur de la rondelle
///
///
/// @return NoeudRondelle* : pointeur sur la rondelle s'il la trouve, 0 sinon
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle* Terrain::obtenirRondelle() const
{
	if(obtenirTable())
	{
		NoeudComposite* g = (NoeudComposite*)obtenirTable()->obtenirGroupe(ArbreRenduINF2990::NOM_RONDELLE);
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
		arbreRendu_->majPosition(temps);
		arbreRendu_->gestionCollision(temps);
		arbreRendu_->ajusterVitesse(temps);
		arbreRendu_->ajusterEnfoncement();
	}
}



///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////