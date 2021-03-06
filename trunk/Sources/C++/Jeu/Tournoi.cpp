///////////////////////////////////////////////////////////////////////////////
/// @file Tournoi.cpp
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "Tournoi.h"
#include "FacadeModele.h"
#include "PlayerComputer.h"
#include "DecodeString.h"
#include "Utilitaire.h"
#include <algorithm>
#include "PlayerHuman.h"
#include "XMLUtils.h"

const std::string Tournoi::tounoiNonJoue = "Aucun Gagnant de ce tournoi!";



////////////////////////////////////////////////////////////////////////
///
/// @fn  Tournoi::Tournoi(  )
///
/// Constructeur par d�faut
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Tournoi::Tournoi(): estEnCours_(false)
{
	mFieldName = "";

    for(int i=0; i<nbrParties_; ++i)
    {
        parties_[i] = GameManager::obtenirInstance()->addNewGame(GAME_TYPE_OFFLINE, Partie::POINTAGE_GAGNANT,NULL, NULL, true);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  Tournoi::~Tournoi( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Tournoi::~Tournoi(void)
{
	libererMemoire();

    for(int i = 0; i < nbrParties_; ++i)
    {
        Partie* wGame = GameManager::obtenirInstance()->getGame(parties_[i]);
        if(wGame)
        {
            GameManager::obtenirInstance()->removeGame(parties_[i]);
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn std::string Tournoi::obtenirNom() const
///
/// Accesseur du nom du tournoi.
///
/// @return Le nom du terrain.
///
////////////////////////////////////////////////////////////////////////
std::string Tournoi::obtenirNom() const
{
	return nom_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Tournoi::modifierNom( const std::string nom )
///
/// Modificateur du nom du tournoi.
///
/// @return Le nom du terrain.
///
////////////////////////////////////////////////////////////////////////
void Tournoi::modifierNom( const std::string nom )
{
	nom_ = nom;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Tournoi::initialisation( JoueursParticipant joueurs )
///
/// Initialisation d'un tournoi. Le style sera un arbre binaire complet avec le match final � l'index 0
///
/// @param[in] JoueursParticipant joueurs : conteneur des joueurs participant aux tournoi
///
/// @return bool vrai si le tournoi a bien �t� cr�er, false si le nombre de joueurIter est insuffisant
///
////////////////////////////////////////////////////////////////////////
bool Tournoi::initialisation( const JoueursParticipant& joueurs, const std::string& pFieldName)
{
	libererMemoire();

	//Verification du nombre de joueurIter dans le conteneur de joueurIter
	if(joueurs.size() != nbrJoueurs_)
	{
		utilitaire::afficherErreur("Nombre de joueurs pour le tournoi mauvais");
		return false;
	}




	mFieldName = pFieldName;

	unsigned int index = 14;
	indexPartieCourante_ = 14;
    Partie* wGames[nbrParties_];
    for (unsigned int j = 0; j < nbrParties_ ; j++)
    {
        wGames[j] = GameManager::obtenirInstance()->getGame(parties_[j]);
    }
	// Assignation des joueurs aux parties
	for (unsigned int j = 0; j < nbrJoueurs_ ; j++)
	{
		// Assignation d'un joueurIter � la partie, la partie peut avoir 2 joueurIter assign�s
		wGames[index]->assignerJoueur(joueurs[j]);
		// Passage � la partie suivante si on a assigner 2 joueurs � la partie courante
		index -= j&1; /// �quivalent � j%2
	}
	// On assigne le terrain au parties
	for (unsigned int i = 0; i < nbrParties_ ; i++)
	{
		wGames[i]->setFieldName(mFieldName);
		wGames[i]->modifierFaitPartieDunTournoi(true);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Tournoi::initialisationXML( XmlElement* element )
///
/// Initialisation d'un tournoi avec un noeud XML
///
/// @param[in] XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Tournoi::initialisationXML( const XmlElement* element, ConteneurJoueur* profilsVirtuelsExistant /*= 0 */ )
{
	libererMemoire();

	// Lecture du nom du tournoi
	const XmlElement* etiquetteNom = element->FirstChildElement("Nom");
	if(etiquetteNom == 0)
		return false;
	const char* nom = etiquetteNom->Attribute("nom");
	if(nom == 0)
		return false;
	nom_ = nom;

	
	
	// Lecture de la liste des gagnants
	const XmlElement* gagnants = element->FirstChildElement("Gagnants");
	if(!gagnants)
		return false;
	const XmlElement* gagnant = gagnants->FirstChildElement("Gagnant");
	if(!gagnant)
		return false;
	for(; gagnant; gagnant = gagnant->NextSiblingElement())
	{
		const XmlNode* child = gagnant->FirstChild();
		if(!child || !child->ToText())
			return false;
		if(child->ToText()->Value() != tounoiNonJoue)
			listeGagnants_.push_back(child->ToText()->Value());
	}

	// Lecture du chemin pour le terrain a jouer
	const XmlElement* map = element->FirstChildElement("Terrain");
	if(map == 0)
		return false;
	const char* mapPath = map->Attribute("chemin");
	if(mapPath == 0)
		return false;
	mFieldName = mapPath;

	// Lecture a savoir si la derniere instance du tournoi etati termine
	int termine;
	if(element->Attribute("termine",&termine) == 0)
		return false;

	if(element->Attribute("indexPartie", (int*)&indexPartieCourante_) == 0)
		return false;

	// Lecture des parties du tournoi
	const XmlElement* partie = element->FirstChildElement("Partie");
    GameManager* wGameManager = GameManager::obtenirInstance();
	for(int i=0; i<nbrParties_; i++)
	{
		if(partie == 0)
			return false;
        Partie* wGame = wGameManager->getGame(parties_[i]);
		if(!wGame->initialiserXML(partie, profilsVirtuelsExistant))
			return false;
		partie = partie->NextSiblingElement();
		wGame->modifierFaitPartieDunTournoi(true);
	}

	if(termine)
		reinitialiserTournoi();

	return true;
}





////////////////////////////////////////////////////////////////////////
///
/// @fn void Tournoi::libererMemoire()
///
/// M�thode pour lib�rer la m�moire
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Tournoi::libererMemoire()
{
	for(int i = 0; i < nbrParties_; ++i)
    {
        Partie* wGame = GameManager::obtenirInstance()->getGame(parties_[i]);
        if(wGame)
        {
            wGame->vider();
        }
    }

	listeGagnants_.clear();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* Tournoi::creerTournoiXML()
///
/// Enregistrement du tournoi dans le fichier XML
///
///
/// @return XmlElement* noeud contenant le tournoi
///
////////////////////////////////////////////////////////////////////////
XmlElement* Tournoi::creerTournoiXML() const
{
	// Noeud Principal
	XmlElement* elementNoeud = XMLUtils::createNode("Tournoi");

	// Indicateur de la partie courante a jouer
	elementNoeud->SetAttribute("indexPartie", indexPartieCourante_);
	// Regarde si la derniere partie est termine pour savoir si le tournoi est termin�
	elementNoeud->SetAttribute("termine", estTermine());

	// Enregistrement des Gagnants  ////////////////////////////////////////////////////////////////
	XmlElement* gagnants = XMLUtils::createNode("Gagnants");
	
	if(!listeGagnants_.empty())
	{
		ListeGagnants::const_iterator iter = listeGagnants_.begin();
		for (; iter != listeGagnants_.end()  ; iter++)
		{
			XmlElement* gagnant = XMLUtils::createNode("Gagnant");
			XmlText* text = new XmlText(iter->c_str());
			gagnant->LinkEndChild(text);
			gagnants->LinkEndChild(gagnant);
		}
	}
	else
	{
		XmlElement* gagnant = XMLUtils::createNode("Gagnant");
		XmlText* text = new XmlText(tounoiNonJoue.c_str());
		gagnant->LinkEndChild(text);	
		gagnants->LinkEndChild(gagnant);
	}	
	elementNoeud->LinkEndChild(gagnants);
	////////////////////////////////////////////////////////////////////////////////////////////////


	// Enregistrement du nom du tournoi  ////////////////////////////////////////////////////////////////
	XmlElement* nom = XMLUtils::createNode("Nom");
	nom->SetAttribute("nom", nom_.c_str());
	elementNoeud->LinkEndChild(nom);
	////////////////////////////////////////////////////////////////////////////////////////////////

	XmlElement* map = XMLUtils::createNode("Terrain");
	map->SetAttribute("chemin", GetFieldName().c_str());
	elementNoeud->LinkEndChild(map);

	// Creation du tournoi de facon lin�aire
    GameManager* wManager = GameManager::obtenirInstance();
	for (unsigned int i = 0; i < nbrParties_ ; i++)
	{
		elementNoeud->LinkEndChild(wManager->getGame(parties_[i])->creerNoeudXML());		
	}
	
	return elementNoeud;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn Partie* Tournoi::obtenirPartieRondeSuivante( unsigned int index )
///
/// Accesseur de la partie de la ronde suivante de la partie � l'index indiqu�
///
/// @param[in] unsigned int index : position de la partie concern�e
///
/// @return Partie* pointeur sur la partie recherch�, 0 si inexistant
///
////////////////////////////////////////////////////////////////////////
unsigned int Tournoi::obtenirPartieRondeSuivante( unsigned int index )
{
	return (index-1)>>1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Partie* Tournoi::obtenirPartieRondePrecedente( unsigned int index )
///
/// Accesseur de la partie de gauche de la ronde pr�cedente de la partie � l'index indiqu�
///
/// @param[in] unsigned int index : position de la partie concern�e
/// @param[in] bool gauche : True : retourne la partie de gauche, False: retourne la partie de droite
///
/// @return Partie* pointeur sur la partie recherch�, 0 si inexistant
///
////////////////////////////////////////////////////////////////////////
unsigned int Tournoi::obtenirPartieRondePrecedente( unsigned int index, bool gauche )
{
	return (index<<1)+(gauche ? 2:1);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* Tournoi::creerArbreXML(unsigned int index)
///
/// cr�e un arbre de noeuds XML pour l'enregistrement
///
/// @param[in]  unsigned int index : position de la partie courante
///
/// @return XmlElement* : Premier appel :noeuds XML d'un sous-arbres
///							Appel recursif: noeuds de la parties courante ou 0 si inexistante
///
////////////////////////////////////////////////////////////////////////
XmlElement* Tournoi::creerArbreXML( unsigned int index  )
{
	if(index >= nbrParties_)
		return 0;

	XmlElement* elementNoeud  = GameManager::obtenirInstance()->getGame(parties_[index])->creerNoeudXML();
	XmlElement* gauche = creerArbreXML(obtenirPartieRondePrecedente(index,true) );
	XmlElement* droite = creerArbreXML(obtenirPartieRondePrecedente(index,false) );

	if(gauche != 0)
		elementNoeud->LinkEndChild(gauche);
	if(droite != 0)
		elementNoeud->LinkEndChild(droite);

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Tournoi::reinitialiserTournoi(  )
///
/// Fonction pour remettre un tournoi a son etat original
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Tournoi::reinitialiserTournoi()
{
	indexPartieCourante_ = 14;
    Partie* wGames[nbrParties_];
    for(int i = 0; i < nbrParties_; ++i)
    {
        wGames[i] = GameManager::obtenirInstance()->getGame(parties_[i]);
    }

	for(int i = 0; i < (nbrParties_>>1); ++i)
	{
		wGames[i]->vider();
		wGames[i]->modifierFaitPartieDunTournoi(true);
	}
	for(int i = nbrParties_>>1; i < nbrParties_; ++i)
	{
		wGames[i]->reinitialiserPartie();
		wGames[i]->modifierFaitPartieDunTournoi(true);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Tournoi::miseAJour()
///
/// Permet de verifier les conditions du tournoi et de mettre a jour son etat
///
///
/// @return bool : vrai s'il y a eu un modification dans son etat
///
////////////////////////////////////////////////////////////////////////
bool Tournoi::miseAJour( bool save /*= true */ )
{
	if(obtenirPartieCourante()->partieTerminee())
	{
        obtenirPartieCourante()->setGameStatus(GAME_ENDED);
		SPPlayerAbstract joueur = obtenirPartieCourante()->obtenirGagnant();
		if(joueur)
		{
			if(indexPartieCourante_ > 0)
				GameManager::obtenirInstance()->getGame(parties_[obtenirPartieRondeSuivante(indexPartieCourante_)])->assignerJoueur(joueur);
			else
				/// Sauvegarde le gagnant
				listeGagnants_.push_back(joueur->obtenirNom());
			
			passerProchainePartie();
			if(save)
				FacadeModele::getInstance()->enregistrerTournoi(this);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueursParticipant Tournoi::obtenirListeJoueursInitiaux()
///
/// Obtient la liste des joueurs du tournoi
///
///
/// @return JoueursParticipant
///
////////////////////////////////////////////////////////////////////////
JoueursParticipant Tournoi::obtenirListeJoueursInitiaux()
{
	JoueursParticipant ret;
    GameManager* wManager = GameManager::obtenirInstance();
	for(int i = nbrParties_>>1; i < nbrParties_; ++i)
	{
        Partie* wGame = wManager->getGame(parties_[i]);
		ret.push_back(wGame->obtenirJoueurGauche());
		ret.push_back(wGame->obtenirJoueurDroit());
	}
	return ret;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Tournoi::majCheminVainqueur(  )
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Tournoi::majCheminVainqueur()
{
	cheminDernierVainqueur_.clear();
	PositionJoueur pos;
	unsigned int index = indexPartieCourante_;
	
	pos = GameManager::obtenirInstance()->getGame(parties_[index])->obtenirPositionGagant();
	cheminDernierVainqueur_.push_back(Vainqueur(index,pos));
	index = obtenirPartieRondePrecedente(index, pos == GAGNANT_GAUCHE);
	while(index < nbrParties_)
	{
		pos = GameManager::obtenirInstance()->getGame(parties_[index])->obtenirPositionGagant();
		cheminDernierVainqueur_.push_back(Vainqueur(index,pos));
		index = obtenirPartieRondePrecedente(index, pos == GAGNANT_GAUCHE);
	}
	if(indexPartieCourante_ != 0)
	{
		index = obtenirPartieRondeSuivante(indexPartieCourante_);
		pos = indexPartieCourante_&1 ? GAGNANT_DROITE : GAGNANT_GAUCHE;
		cheminDernierVainqueur_.push_back(Vainqueur(index,pos));
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////