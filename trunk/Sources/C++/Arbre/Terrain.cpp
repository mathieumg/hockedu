///////////////////////////////////////////////////////////////////////////////
/// @file Terrain.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "RazerGameUtilities.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#include "FacadeModele.h"
#include "Partie.h"
#include "SoundFMOD.h"
#endif
#if BOX2D_DEBUG
#include "DebugRenderBox2D.h"
#include "Runnable.h"
#endif
#ifndef __APPLE__
#include "../Reseau/Paquets/PaquetMaillet.h"
#include "../Reseau/GestionnaireReseau.h"

#endif

#include "Terrain.h"
#include "RazerGameTree.h"
#include "ArbreNoeudLibre.h"
#include "ZoneEdition.h"
#include "NoeudTable.h"
#include "NoeudGroupe.h"
#include "ZoneEdition.h"
#include "AideCollision.h"
#include "NoeudBut.h"
#include "utilitaire.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "NoeudBut.h"
#include "NoeudPiece.h"
#include "XMLUtils.h"
#include "NoeudAccelerateur.h"
#include "NoeudPortail.h"
#include "VisiteurDupliquer.h"
#include "VisiteurCollision.h"
#include "VisiteurDeplacement.h"
#include "VisiteurEcrireXML.h"
#include "ExceptionJeu.h"
#include "NodeWallEdition.h"
#include "NodeControlPoint.h"
#include "VisiteurFunction.h"

const unsigned int MAX_PUCKS = 1;
const unsigned int MAX_MALLETS = 2;



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
Terrain::Terrain(Partie* pGame): mLogicTree(NULL), mNewNodeTree(NULL), mTable(NULL),mFieldName(""),mRenderTree(0),mGame(pGame)
{
    mEditionZone = NULL;
    if(!mGame)
    {
        mEditionZone = new ZoneEdition();
    }
    mZamboni = new NoeudAffichage(RazerGameUtilities::NAME_ZAMBONI);
#if BOX2D_INTEGRATED
    b2Vec2 gravity(0,0);
    mWorld = new b2World(gravity);
    mWorld->SetContactListener(this);
#if BOX2D_DEBUG
    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_shapeBit);
    mWorld->SetDebugDraw(DebugRenderBox2D::mInstance);
#endif

#endif
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
#ifndef __APPLE__
    RunnableBreaker::signalObservers();
#endif
	libererMemoire();

    if(mEditionZone)
    {
        delete mEditionZone;
    }

#if BOX2D_INTEGRATED
    if(mWorld)
    {
        delete mWorld;
        mWorld = NULL;
    }
#endif
    if(mZamboni)
    {
        delete mZamboni;
    }
    mZamboni = NULL;
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
	if(mLogicTree)
	{
		// Libération de l'arbre de rendu complet
		mLogicTree->vider();
		delete mLogicTree;
		mLogicTree = NULL;
	}
	if(mNewNodeTree)
	{
		// Ses enfants ne seront pas libérés, mais le terrain n'en n'est pas responsable
		mNewNodeTree->vider();
		delete mNewNodeTree;
		mNewNodeTree = NULL;
	}
    // Libère l'arbre de rendu servant à l'affichage seulement
    if(mRenderTree)
    {
        mRenderTree->vider();
        delete mRenderTree;
        mRenderTree = NULL;
    }
	// On ne libère pas la table, car elle est un enfant de l'arbre de rendu
	mTable = NULL;
	
	mFieldName = "";
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::renderField()
///
/// Permet d'effectuer le rendu des arbres du terrain
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::renderField()
{
	if(mLogicTree)
		mLogicTree->afficher();
	if(mNewNodeTree)
		mNewNodeTree->afficher();
	if(getZoneEdition())
		getZoneEdition()->afficher();
	if(mRenderTree)
		mRenderTree->afficher();
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
	mFieldName = nom;
	// Initialisation de la Zone d'edition
    if(getZoneEdition())
    {
        getZoneEdition()->reinitialiser();
    }

	initialiserArbreRendu();
	
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
    // Initialisation des arbres de rendus
    if(mLogicTree == NULL)
    {
		mLogicTree = new RazerGameTree(this,MAX_MALLETS,MAX_PUCKS);
    }
	else
    {
		mLogicTree->vider();
    }

    if(!mRenderTree)
    {
        mRenderTree = new ArbreRendu(this);
        NoeudAbstrait* piece = new NoeudPiece(RazerGameUtilities::NOM_HOUSE);
        mRenderTree->ajouter(piece);
        mRenderTree->ajouter(mZamboni);
    }
    if(!mGame)
    {
        if(!mNewNodeTree)
        {
            mNewNodeTree = new ArbreNoeudLibre();
        }
        else
        {
            mNewNodeTree->vider();
        }
        mNewNodeTree->modifierTerrain(this);
    }

	// Ajout d'une table de base au terrain
	mTable = new NoeudTable(RazerGameUtilities::NOM_TABLE);
	mLogicTree->ajouter(mTable);


	/// Groupe destine a contenir les noeud concret pour un meilleur parcours d'arbre
	NoeudGroupe* 	gMaillet =	new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_MAILLET),
		*gRondelle =	new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_RONDELLE),
		*gAccel =		new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_ACCELERATEUR),
        *gMuret =		new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_MURET),
        *gPortail =		new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,RazerGameUtilities::NOM_PORTAIL);

	// La table contient ces groupes.
	mTable->ajouter(gRondelle);
	mTable->ajouter(gMaillet);
	mTable->ajouter(gAccel);
	mTable->ajouter(gMuret);
    mTable->ajouter(gPortail);

	// Permet de rediriger les bandes extérieur de la table vers le groupe  gMuret
	//mTable->reassignerParentBandeExt();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiserXml( XmlElement* element )
///
/// Permet d'initialiser le terrain avec ces éléments a partir d'un noeud XML
///
/// @param[in] XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::initialiserXml( XmlElement* element )
{
	libererMemoire();
	
	// Initialisation des arbres de rendus
    if(!mRenderTree)
    {
        mRenderTree = new ArbreRendu(this);
        NoeudAbstrait* piece = new NoeudPiece(RazerGameUtilities::NOM_HOUSE);
        mRenderTree->ajouter(piece);
        mRenderTree->ajouter(mZamboni);
    }
    if(!mGame)
    {
        if(!mNewNodeTree)
        {
            mNewNodeTree = new ArbreNoeudLibre();
        }
        else
        {
            mNewNodeTree->vider();
        }
        mNewNodeTree->modifierTerrain(this);
    }
	mLogicTree = new RazerGameTree(this,MAX_MALLETS,MAX_PUCKS);

	XmlElement* racine = XMLUtils::FirstChildElement(element,"Terrain");
	if(!racine)
		return false;
    if(!XMLUtils::readAttribute(racine,"nom",mFieldName))
		return false;

	try
	{
        mLogicTree->initialiser(racine);
		mTable = mLogicTree->obtenirTable();
		if(mTable == NULL)
        {
			throw ExceptionJeu("Il ny a pas de table sur le terrain");
        }
        fullRebuild();
	}
	catch(ExceptionJeu& e)
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

	if(getZoneEdition() && !getZoneEdition()->initialisationXML(racine))
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
	initialiserArbreRendu();
	getZoneEdition()->reinitialiser();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* Terrain::creerNoeudXML()
///
/// Creation du noeud XML du Terrain
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* Terrain::creerNoeudXML()
{
	// Créer le noeud 
	XmlElement* racine = XMLUtils::createNode("Terrain");
    XMLUtils::writeAttribute(racine,"nom",getNom());

    VisiteurEcrireXML v;
    acceptVisitor(v);

    XmlElement* elem = v.obtenirRacine();
    XMLUtils::LinkEndChild(racine,elem);

    checkf(getZoneEdition(), "Tentative de sauvegarder la zone édition qui n'existe pas. S'assurer qu'on est pas en train d'essaye de save un terrain pour le mode jeu");
    if(getZoneEdition())
    {
        XMLUtils::LinkEndChild(racine,getZoneEdition()->creerNoeudXML());
    }

	return racine;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::animerTerrain( const float& temps )
///
/// Permet d'animer les noeuds des arbres du terrain, la physique reste géré par
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::animerTerrain( const float& temps )
{
	if(mLogicTree)
		mLogicTree->animer(temps);
	if(mNewNodeTree)
		mNewNodeTree->animer(temps);
	if(mRenderTree)
		mRenderTree->animer(temps);
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
	mNewNodeTree->ajouter(noeud);
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
	mNewNodeTree->detacherEnfant(noeud);
	mTable->ajouter(noeud);
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
	mNewNodeTree->detacherEnfant(noeud);
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
	const Vecteur3& pos = noeud->getPosition();
    checkf(getZoneEdition(),"call illégal à InsideLimits, doit seulement etre utiliser pour le mode édition");
	// Cas particulier pour des muret puisque ce sont des segment et non des cercles
	if(getZoneEdition() && noeud->obtenirType() == RazerGameUtilities::NOM_MURET)
	{
        Vecteur2 intersection;
		NodeWallAbstract *muret = (NodeWallAbstract *)noeud;
		if (muret)
		{
			// Ligne du haut
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(-getZoneEdition()->obtenirLimiteExtLongueur(),getZoneEdition()->obtenirLimiteExtLargeur()),
				Vecteur2(getZoneEdition()->obtenirLimiteExtLongueur(),getZoneEdition()->obtenirLimiteExtLargeur()),
				intersection	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne de droite
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(getZoneEdition()->obtenirLimiteExtLongueur(),getZoneEdition()->obtenirLimiteExtLargeur()),
				Vecteur2(getZoneEdition()->obtenirLimiteExtLongueur(),-getZoneEdition()->obtenirLimiteExtLargeur()),
				intersection	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne du bas
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(getZoneEdition()->obtenirLimiteExtLongueur(),-getZoneEdition()->obtenirLimiteExtLargeur()),
				Vecteur2(-getZoneEdition()->obtenirLimiteExtLongueur(),-getZoneEdition()->obtenirLimiteExtLargeur()),
				intersection	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
			// Ligne de Gauche
			if(aidecollision::calculerCollisionSegmentSegment(
				muret->obtenirCoin1().convertir<2>(),
				muret->obtenirCoin2().convertir<2>(),
				Vecteur2(-getZoneEdition()->obtenirLimiteExtLongueur(),-getZoneEdition()->obtenirLimiteExtLargeur()),
				Vecteur2(-getZoneEdition()->obtenirLimiteExtLongueur(),getZoneEdition()->obtenirLimiteExtLargeur()),
				intersection	// pas besoin du point dintersection
				).type != aidecollision::COLLISION_AUCUNE)
				return false;
		}
	}
	// Tests sur les positions avec leurs rayons beaucoup plus simple
	// sert aussi de float check pour les murets car leur rayon est nulle
	if(pos[VX]+noeud->obtenirRayon() > getZoneEdition()->obtenirLimiteExtLongueur() || pos[VX]-noeud->obtenirRayon() < -getZoneEdition()->obtenirLimiteExtLongueur())
		return false;
	if(pos[VY]+noeud->obtenirRayon() > getZoneEdition()->obtenirLimiteExtLargeur() || pos[VY]-noeud->obtenirRayon() < -getZoneEdition()->obtenirLimiteExtLargeur())
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
void Terrain::creerTerrainParDefaut(const std::string& nom)
{
	initialiser(nom);
	NoeudAbstrait* maillet1 = getLogicTree()->creerNoeud(RazerGameUtilities::NOM_MAILLET);
	NoeudAbstrait* maillet2 = getLogicTree()->creerNoeud(RazerGameUtilities::NOM_MAILLET);
	NoeudAbstrait* rondelle = getLogicTree()->creerNoeud(RazerGameUtilities::NOM_RONDELLE);

	maillet1->setPosition(mTable->obtenirPoint(POSITION_MILIEU_GAUCHE)->getPosition()/2.0);
	maillet2->setPosition(mTable->obtenirPoint(POSITION_MILIEU_DROITE)->getPosition()/2.0);
	rondelle->setPosition(Vecteur3(0.0,0.0,0.0));

	mTable->ajouter(maillet1);
	mTable->ajouter(maillet2);
	mTable->ajouter(rondelle);

    fullRebuild();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void createRandomField( const std::string& )
///
/// Creation d'un terrain 
///
/// @param[in] const std::string &
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::createRandomField(const std::string& nom)
{
    creerTerrainParDefaut(nom);

    VisiteurDeplacement d1(Vecteur2((float)(-rand()%50),(float)(rand()%50)),true);
    VisiteurDeplacement d2(Vecteur2((float)(-rand()%50),(float)(rand()%50)),true);
    VisiteurDeplacement d3(Vecteur2((float)(-rand()%50),(float)(-rand()%50)),true);

    // shuffle it a bit
    mTable->obtenirPoint(POSITION_MILIEU_DROITE)->acceptVisitor(d1);
    mTable->obtenirPoint(POSITION_HAUT_DROITE)->acceptVisitor(d2);
    mTable->obtenirPoint(POSITION_BAS_DROITE)->acceptVisitor(d3);

#define AddAccel(x,y,acc,e)                                                             \
    if(rand()&1)                                                                        \
    {                                                                                   \
    NoeudAccelerateur* a = new NoeudAccelerateur(RazerGameUtilities::NOM_ACCELERATEUR); \
    a->setPosition(Vecteur3((float)x,(float)y));                                        \
    a->modifierBonusAccel((float)acc);                                                  \
    a->modifierEchelleCourante(Vecteur3((float)e,(float)e,1));                          \
    mTable->ajouter(a);                                                                 \
    }                                                                                 
    AddAccel(-40,0,1.7,1);
    AddAccel(45,0,0.8,1);
    AddAccel(50,50,1.5,1.2);
    AddAccel(-50,50,1.6,1);
    AddAccel(-50,-50,1.5,1);
    AddAccel(50,-50,1.5,1);
    AddAccel(115,0,1.3,1);
    AddAccel(-115,0,1.5,0.6);

#define AddPortal(x,y,e)                                                              \
    if(rand()&1)                                                                      \
    {                                                                                 \
    NoeudPortail* a = new NoeudPortail(RazerGameUtilities::NOM_PORTAIL);              \
    a->setPosition(Vecteur3((float)x,(float)y));                                      \
    a->modifierEchelleCourante(Vecteur3((float)e,(float)e,1));                        \
    mTable->ajouter(a);                                                               \
    }  
    AddPortal(-90,30,1);
    AddPortal(-95,-30,1);
    AddPortal(0,-50,0.55);
    AddPortal(80,-40,1.55);
    AddPortal(80,30,1);
    AddPortal(0,55,0.65);

#define AddWall(x1,y1,x2,y2,e,rebond)                                                           \
    if(rand()&1)                                                                                \
    {                                                                                           \
        if(IsGameField())                                                                       \
        {                                                                                       \
            NoeudMuret* muret = new NoeudMuret(RazerGameUtilities::NOM_MURET);                  \
            muret->modifierEchelleCourante(Vecteur3(1,(float)e,1));                             \
            muret->assignerPositionCoin(1,Vecteur3((float)x1,(float)y1));                       \
            muret->assignerPositionCoin(2,Vecteur3((float)x2,(float)y2));                       \
            muret->setReboundRatio((float)rebond);                                              \
            mTable->ajouter(muret);                                                             \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            NodeWallEdition* wall = new NodeWallEdition(RazerGameUtilities::NOM_MURET);         \
            NodeControlPoint* p1 = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);\
            NodeControlPoint* p2 = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);\
            wall->ajouter(p1);                                                                  \
            wall->ajouter(p2);                                                                  \
            p1->setPosition(Vecteur3((float)x1,(float)y1));                                     \
            p2->setPosition(Vecteur3((float)x2,(float)y2));                                     \
            wall->setReboundRatio((float)rebond);                                               \
            wall->modifierEchelle(e);                                                           \
            mTable->ajouter(wall);                                                              \
        }                                                                                       \
    }                                                                                           

    AddWall(65,65,130,35,1,1);
    AddWall(130,-35,75,-66,1,0.8);
    AddWall(-80,-65,-130,-40,1,0.75);
    AddWall(-125,35,-75,60,2.5,1);
    AddWall(-35,30,30,30,0.5,1.2);
    AddWall(40,-30,-25,-30,2,0.4);

    fullRebuild();
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
	if(!mTable)
	{
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\naucune table presente sur le terrain");
		return false;
	}
	bool mailletGaucheOk = false, mailletDroiteOk = false, rondelleOk = false;

	// on prend un des 2 buts
	if(!mTable->obtenirBut(1))
	{
		if(afficherErreur)
			utilitaire::afficherErreur("Erreur: table invalide\naucun buts presents sur le terrain");
		return false;
	}
	float hauteurBut = mTable->obtenirBut(1)->obtenirHauteurBut();

	const unsigned int nbTypeModifiable = 5;
	std::string typeNoeudModifiable[] = {
		RazerGameUtilities::NOM_RONDELLE,
		RazerGameUtilities::NOM_MAILLET,
		RazerGameUtilities::NOM_ACCELERATEUR,
		RazerGameUtilities::NOM_PORTAIL,
		RazerGameUtilities::NOM_MURET,
	};
	
	// Suppression des noeuds qui ne sont pas sur la table
	for(unsigned int i=0; i<nbTypeModifiable; ++i)
	{
		NoeudComposite* g = (NoeudComposite*)mTable->obtenirGroupe(typeNoeudModifiable[i]);
		if(g)
		{
			for(unsigned int j=0; j<g->obtenirNombreEnfants(); ++j)
			{
				NoeudAbstrait* n = g->chercher(j);
				// On verifie que le type est bon, surtout utile pour les bande exterieur qui ont un nom different et doivent etre ignorer par cette methode
				if(n->obtenirType() == typeNoeudModifiable[i])
				{
					if(!mTable->estSurTable(n) )
					{
						// La suppression du pNode l'enlevera du groupe
						g->effacer(n);
						// Repositionnement de i pour pointer au bon endroit a la prochaine iteration
						j--; 
					}
				}
			}
		}
	}

	// Verification des maillets
	NoeudComposite* g = (NoeudComposite*)mTable->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
	if(!g)
		return false;
	for(unsigned int j=0; j<g->obtenirNombreEnfants(); ++j)
	{
		NoeudMaillet *maillet = dynamic_cast<NoeudMaillet *>(g->chercher(j));
		if (maillet)
		{
			// verification des maillets
			if(maillet->getPosition()[VX] <= 0)
			{
				if(mailletGaucheOk)
				{
					if(afficherErreur)
						utilitaire::afficherErreur("Erreur: table invalide\n2 Maillets sur le cote gauche de la table");
					return false;
				}
				mailletGaucheOk = true;
				maillet->modifierPositionOriginale(maillet->getPosition());
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
				maillet->modifierPositionOriginale(maillet->getPosition());
			}
		}
	}
	g = (NoeudComposite*)mTable->obtenirGroupe(RazerGameUtilities::NOM_RONDELLE);
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
/// @fn NoeudRondelle* Terrain::getPuck()
///
/// Accesseur de la rondelle
///
///
/// @return NoeudRondelle* : pointeur sur la rondelle s'il la trouve, 0 sinon
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle* Terrain::getPuck() const
{
	if(getTable())
	{
		NoeudComposite* g = (NoeudComposite*)getTable()->obtenirGroupe(RazerGameUtilities::NOM_RONDELLE);
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
	if(mLogicTree)
	{
        /// TODO:: cache pointer to mallet for field to access directly
        NoeudMaillet* mailletGauche = getLeftMallet();
        NoeudMaillet* mailletDroit = getRightMallet();
#if BOX2D_INTEGRATED
        

        if(mailletDroit)
        {
            mailletDroit->preSimulationActions();
        }
        if(mailletGauche)
        {
            mailletGauche->preSimulationActions();
        }

        mWorld->SetWarmStarting(true);
        mWorld->SetContinuousPhysics(true);
        mWorld->SetSubStepping(true);
        mWorld->Step(temps, 8, 8);
#else
		mLogicTree->majPosition(temps);
		mLogicTree->gestionCollision(temps);
		mLogicTree->ajusterVitesse(temps);
		mLogicTree->ajusterEnfoncement();
#endif
        
#ifndef __APPLE__
        if(mGame)
        {
            mGame->sendNetworkInfos();
        }
        /*if(mailletGauche && mGame)
        {
            PaquetMaillet* wPaquet = (PaquetMaillet*) GestionnaireReseau::obtenirInstance()->creerPaquet(MAILLET);
            wPaquet->setPosition(mailletGauche->getPosition());
            wPaquet->setGameId(mGame->getUniqueGameId());
            GestionnaireReseau::obtenirInstance()->envoyerPaquet("GameServer", wPaquet, TCP);
            
        }*/
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
            {
                NoeudBut *but = dynamic_cast<NoeudBut *>((NoeudAbstrait*)bodies[1]->GetUserData());
                if (but)
                {
                    b2Body* rondelleBody = bodies[0];
                    NoeudRondelle* rondelle = (NoeudRondelle*)(rondelleBody->GetUserData());
                    Runnable* r = new Runnable([=](Runnable*){
                        if(mGame)
                        {
                            auto position = rondelle->getPosition();
                            if(position[VX] < 0)
                            {
                                mGame->incrementerPointsJoueurDroit();
                            }
                            else
                            {
                                mGame->incrementerPointsJoueurGauche();
                            }
                            SoundFMOD::obtenirInstance()->playEffect(GOAL_EFFECT);
                        }
                        mGame->miseAuJeu();
                        rondelleBody->SetLinearVelocity(b2Vec2(0,0));
                    });
                    RunnableBreaker::attach(r);
                    RazerGameUtilities::RunOnUpdateThread(r,true);
                }
                else
                {
                    if(bodies[0]->GetLinearVelocity().LengthSquared() > 200)
                        SoundFMOD::obtenirInstance()->playEffect(COLLISION_MURET_EFFECT);
                }
            }
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
                            Runnable* r = new Runnable([=](Runnable*)
                            {
                                portailDeSortie->setIsAttractionFieldActive(false);
                                auto newPos = portailDeSortie->getPosition();
                                rondelle->setPosition(newPos);
                                SoundFMOD::obtenirInstance()->playEffect(effect(PORTAL_EFFECT));
                            });
                            RunnableBreaker::attach(r);
                            RazerGameUtilities::RunOnUpdateThread(r,true);
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
VISITEUR_FUNC_FUNC_DECLARATION(ForceFullRebuildFunc)
{
    pNoeud->forceFullUpdate();
}
void Terrain::fullRebuild()
{
    checkf(mLogicTree,"Requete pour un full rebuild d'un terrain sans arbre");
    if(mLogicTree)
    {
        VisiteurFunction v(ForceFullRebuildFunc);
        mLogicTree->acceptVisitor(v);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::setTableControlPointVisible( bool pVisible )
///
/// Show or hide the control points of the table ( they are hidden during play )
///
/// @param[in] bool pVisible
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::setTableControlPointVisible( bool pVisible )
{
    if(mTable)
    {
        for (int i = 0; i < NoeudTable::NB_CONTROL_POINTS ; ++i)
        {
            // the control points always exists with the table
            mTable->obtenirPoint(i)->assignerAffiche(pVisible);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::deselectTableItems()
///
/// removes user selection on all nodes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::setTableItemsSelection( bool pSelect )
{
    if(mLogicTree)
    {
        if(pSelect)
        {
            mLogicTree->selectionnerTout();
        }
        else
        {
            mLogicTree->deselectionnerTout();
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::IsNodeSelected()
///
/// Indicates if any node in the logic tree is selected
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::IsAnyNodeSelected() const
{
    return mLogicTree && mLogicTree->possedeSelection();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::acceptVisitor( class VisiteurNoeud* visiteur )
///
/// Launch a visitor on the field
///
/// @param[in] class VisiteurNoeud * visiteur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::acceptVisitor( class VisiteurNoeud& visitor )
{
    if(mLogicTree)
    {
        mLogicTree->acceptVisitor(visitor);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::duplicateSelection()
///
/// duplicate nodes selected that can be duplicated
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::duplicateSelection()
{
    if(mLogicTree)
    {
        VisiteurDupliquer v(mLogicTree);
        acceptVisitor(v);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::getSelectedNodes( ConteneurNoeuds& pSelectedNodes )
///
/// gets the list of node selected
///
/// @param[in] ConteneurNoeuds & pSelectedNodes
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::getSelectedNodes( ConteneurNoeuds& pSelectedNodes ) const
{
    if(mLogicTree)
    {
        mLogicTree->getSelectedNodes(pSelectedNodes);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet* Terrain::getLeftMallet()
///
/// accesseur du maillet de gauche
///
///
/// @return NoeudMaillet*
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet* Terrain::getLeftMallet() const
{
    NoeudMaillet* maillet = NULL;
    if(getTable())
    {
        NoeudGroupe* g = getTable()->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
        checkf(g, "Groupe pour les maillets manquant");
        if(g)
        {
            for(unsigned int i=0; i<g->obtenirNombreEnfants(); ++i)
            {
                NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->chercher(i));
                if( m->getPosition()[VX] <= 0 )
                    maillet = m;
            }
        }
    }
    return maillet;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet* Terrain::getRightMallet()
///
/// accesseur du maillet de droite
///
///
/// @return NoeudMaillet*
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet* Terrain::getRightMallet() const
{
    NoeudMaillet* maillet = NULL;
    if(getTable())
    {
        NoeudGroupe* g = getTable()->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
        checkf(g, "Groupe pour les maillets manquant");
        if(g)
        {
            for(unsigned int i=0; i<g->obtenirNombreEnfants(); ++i)
            {
                NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->chercher(i));
                if(m->getPosition()[VX]>0)
                    maillet = m;
            }
        }
    }
    return maillet;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::IsNodeAtValidEditionPosition( NoeudAbstrait* pNode, bool pDoHightlightNodeInCollision /*= false*/ )
///
/// indicate if the node can be released at that position safely
///
/// @param[in] NoeudAbstrait * pNode : node to validate
/// @param[in] bool pDoHightlightNodeInCollision : when true, node in collision with the node to validate will be highlighted
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::IsNodeAtValidEditionPosition( NoeudAbstrait* pNode, bool pDoHightlightNodeInCollision /*= false*/ )
{
    VisiteurCollision visiteur(pNode,pDoHightlightNodeInCollision);
    acceptVisitor(visiteur);
    return !(visiteur.collisionPresente()) && insideLimits(pNode);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::FixCollidingObjects()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::FixCollidingObjects()
{
    bool tableValide = false;

    static const int n = 5;
    static const std::string groups[n] = {
        RazerGameUtilities::NOM_ACCELERATEUR,
        RazerGameUtilities::NOM_RONDELLE,
        RazerGameUtilities::NOM_MURET,
        RazerGameUtilities::NOM_PORTAIL,
        RazerGameUtilities::NOM_MAILLET,
    };

    for(unsigned int iter = 0; iter < 10 && !tableValide ; ++iter)
    {
        tableValide = true;
        for(int g=0; g<n; ++g)
        {
            NoeudGroupe* groupe = mTable->obtenirGroupe(groups[g]);
            if(groupe)
            {
                for (auto it = groupe->obtenirEnfants().begin(); it != groupe->obtenirEnfants().end(); it++)
                {
                    tableValide &= FixCollindingNode(*it,2);
                }
            }
        }
    }
    return tableValide;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::FixCollindingNode( NoeudAbstrait* node )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * node
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::FixCollindingNode( NoeudAbstrait* pNode, unsigned int nbIterations )
{
    // Si l'un des pNode est null, on ne fait pas le travail
    if(!pNode)
        return true;

    // Avant les tests de collision, on regarde d'abord si le pNode est dans les limites
    if(!insideLimits(pNode))
    {
        // Repositionne au centre de la table
        pNode->setPosition(Vecteur3());
    }

    VisiteurCollision v(pNode,false);
    acceptVisitor(v);

    unsigned int tentative = 0;
    while( v.collisionPresente() && ++tentative <= nbIterations)
    {
        ConteneurNoeuds liste;
        ConteneurDetailsCollision details = v.obtenirConteneurDetailsCollision();
        v.obtenirListeCollision(liste);
        for (int j = 0; j < liste.size()  ; j++)
        {
            //Vecteur3 deplacement(elementSurTable_[i]->getPosition() - liste[j]->getPosition());
            Vecteur3 deplacement((details[j].direction*details[j].enfoncement)*-1);
            if(deplacement.norme() == 0)
                deplacement = Vecteur3(1.0,1.0);
            //deplacement.normaliser();
            VisiteurDeplacement vDeplacement(deplacement,true);
            pNode->acceptVisitor(vDeplacement);
        }
        if(!insideLimits(pNode))
        {
            // Repositionne au centre de la table
            pNode->setPosition(Vecteur3());
        }
        v.reinitialiser();
        acceptVisitor(v);
    }

    // Il faut la partie égale de la comparaison, car le ++tentative 
    // de la condition while ne ce fera pas s'il n'y a plus de collision
    return tentative <= nbIterations;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float Terrain::GetTableWidth()
///
/// /*Description*/
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float Terrain::GetTableWidth() const
{
    if(mTable)
    {
        return mTable->GetWidth();
    }
    if(mEditionZone)
    {
        return mEditionZone->obtenirLimiteExtLargeur()*2.f;
    }
    return ZoneEdition::DEFAUT_LIMITE_EXT_LARGEUR*2.f;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::NodeSelectionNotification( NoeudAbstrait* node, bool selectionne_ )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * node
/// @param[in] bool selectionne_
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::NodeSelectionNotification( NoeudAbstrait* node, bool selected )
{
    int nbSelectedOld = (int)mSelectedNodes.size();
    if(selected)
    {
        mSelectedNodes.insert(node);
    }
    else
    {
        auto it = mSelectedNodes.find(node);
        if(it != mSelectedNodes.end())
        {
            mSelectedNodes.erase(it);
        }
    }
    int nbSelectedNew = (int)mSelectedNodes.size();
    if(nbSelectedOld != nbSelectedNew)
    {
        if( nbSelectedNew == 0 )
        {
            // no more item selected
            FacadeModele::transmitEvent(THERE_ARE_NO_NODE_SELECTED);
        }
        else
        {
            // selection present
            FacadeModele::transmitEvent(THERE_ARE_NODES_SELECTED);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::CanSelectedNodeBeDeleted()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::CanSelectedNodeBeDeleted() const
{
    for(auto it=mSelectedNodes.begin(); it != mSelectedNodes.end(); ++it)
    {
        NoeudPoint *point = dynamic_cast<NoeudPoint *>(*it);
        if (!point)
        {
            // seulement les noeud point sont selectionnable et ne peuvent etre deleted so haha
            // dont ask dont tell....
            return true;
        }
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////