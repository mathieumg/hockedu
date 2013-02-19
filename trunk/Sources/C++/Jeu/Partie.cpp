//////////////////////////////////////////////////////////////////////////////
/// @file Partie.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "Partie.h"
#include "JoueurHumain.h"
#include "JoueurVirtuel.h"
#include "FacadeModele.h"
#include "Utilitaire.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "SoundFMOD.h"
#include "GestionnaireModeles.h"
#include "Vue.h"
#include "Camera.h"
#include "GestionnaireAnimations.h"
#include "XMLUtils.h"


GLuint Partie::listePause_ = 0;
const int Partie::POINTAGE_GAGNANT = 7;


////////////////////////////////////////////////////////////////////////
///
/// @fn  Partie::Partie( SPJoueurAbstrait joueurGauche /*= nullShrPtr*/, SPJoueurAbstrait joueurDroit /*= nullShrPtr*/ )
///
/// Constructeur qui initialise la partie.
///
/// @param[in] SPJoueurAbstrait joueurGauche : le premier joueur.
/// @param[in] SPJoueurAbstrait joueurDroit	 : le deuxième joueur.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Partie::Partie(SPJoueurAbstrait joueurGauche /*= 0*/, SPJoueurAbstrait joueurDroit /*= 0*/ ):
pointsJoueurGauche_(0),pointsJoueurDroit_(0),joueurGauche_(joueurGauche),joueurDroit_(joueurDroit), enPause_(false), estPret_(false), faitPartieDunTournoi_(false)
{
	chiffres_ = new NoeudAffichage("3");
	terrain_ = "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Partie::~Partie( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
Partie::~Partie( void )
{
    SignalGameOver();
	joueurGauche_.reset();
	joueurDroit_.reset();
	delete chiffres_;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn int Partie::obtenirPointsJoueurGauche() const
///
/// Accesseur du nombre de points du joueur 1.
///
/// @return pointsJoueurGauche_ : le nombre de points du premier joueur.
///
////////////////////////////////////////////////////////////////////////
int Partie::obtenirPointsJoueurGauche() const
{
	return pointsJoueurGauche_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int Partie::obtenirPointsJoueurDroit() const
///
/// Accesseur du nombre de points du joueur 2.
///
/// @return pointsJoueurDroit_ : le nombre de points du deuxième joueur.
///
////////////////////////////////////////////////////////////////////////
int Partie::obtenirPointsJoueurDroit() const
{
	return pointsJoueurDroit_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string Partie::obtenirNomJoueurGauche() const
///
/// Accesseur du nom du joueur 1.
///
/// @return Le nom du premier joueur.
///
////////////////////////////////////////////////////////////////////////
std::string Partie::obtenirNomJoueurGauche() const
{
	if(joueurGauche_)
		return joueurGauche_->obtenirNom();
	return "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string Partie::obtenirNomJoueurDroit() const
///
/// Accesseur du nom du joueur 2.
///
/// @return Le nom du deuxième joueur.
///
////////////////////////////////////////////////////////////////////////
std::string Partie::obtenirNomJoueurDroit() const
{
	if(joueurDroit_)
		return joueurDroit_->obtenirNom();
	return "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string Partie::obtenirCheminTerrain() const
///
/// Accesseur du nom du terrain.
///
/// @return Le nom du terrain.
///
////////////////////////////////////////////////////////////////////////
std::string Partie::obtenirCheminTerrain() const
{
	return terrain_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::setFieldName( const std::string terrain )
///
/// Modificateur du nom du terrain.
///
/// @return Le nom du terrain.
///
////////////////////////////////////////////////////////////////////////
void Partie::setFieldName( const std::string terrain )
{
	terrain_ = terrain;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::incrementerPointsJoueurGauche()
///
/// Donne un point au premier joueur.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Partie::incrementerPointsJoueurGauche()
{
	pointsJoueurGauche_++;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::incrementerPointsJoueurDroit()
///
/// Donne un point au deuxième joueur.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Partie::incrementerPointsJoueurDroit()
{
	pointsJoueurDroit_++;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SPJoueurAbstrait Partie::obtenirGagnant() const
///
/// Retourne le gagnant de la partie
///
/// @return gagnant ou 0 si la partie n'est pas terminée
///
////////////////////////////////////////////////////////////////////////
SPJoueurAbstrait Partie::obtenirGagnant() const
{
	if(!partieTerminee())
		return 0;
	return pointsJoueurGauche_ > pointsJoueurDroit_ ? joueurGauche_ : joueurDroit_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::assignerJoueur( SPJoueurAbstrait joueur )
///
/// Assignation d'un joueur à la partie
///
/// @param[in] SPJoueurAbstrait joueur : joueur à ajouter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::assignerJoueur( SPJoueurAbstrait joueur )
{
	if(joueurGauche_ == 0)
		modifierJoueurGauche(joueur);
	else if(joueurDroit_ == 0)
		modifierJoueurDroit(joueur);
// 	else
// 		delete joueur;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* Partie::creerNoeudXML()
///
/// Création de l'élément XML contenant la partie
///
///
/// @return XmlElement* noeud contenant la partie
///
////////////////////////////////////////////////////////////////////////
XmlElement* Partie::creerNoeudXML() const
{
	XmlElement* elementNoeud = XMLUtils::createNode("Partie");

	elementNoeud->SetAttribute("ptsG", pointsJoueurGauche_);
	elementNoeud->SetAttribute("ptsD", pointsJoueurDroit_);
	elementNoeud->SetAttribute("Terrain", terrain_.c_str());

	if(joueurGauche_ != 0)
		elementNoeud->LinkEndChild(joueurGauche_->creerNoeudXML());
	if(joueurDroit_ != 0)
		elementNoeud->LinkEndChild(joueurDroit_->creerNoeudXML());

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::initialiserXML( XmlElement* elem )
///
/// Initialisation d'une partie a partir d'un noeud XML
///
/// @param[in] XmlElement * elem
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool Partie::initialiserXML( XmlElement* elem, ConteneurJoueur* profilsVirtuelsExistants /*= 0*/ )
{
	if(elem->QueryIntAttribute("ptsG", &pointsJoueurGauche_) != TIXML_SUCCESS)
		pointsJoueurGauche_ = 0;		//Probleme pour le chargement des points, on assigne un pointage nul
	if(elem->QueryIntAttribute("ptsD", &pointsJoueurDroit_) != TIXML_SUCCESS)
		pointsJoueurDroit_ = 0;		//Probleme pour le chargement des points, on assigne un pointage nul

	// On recherche le terrain
	const char* map = elem->Attribute("Terrain");
	if(map == 0)
		terrain_ = "";
	else
		terrain_ = map;

	const XmlElement* joueurXml = elem->FirstChildElement();
	int nbJoueurs=0;
	for( ; joueurXml/*Vérifie si child est non-null*/; joueurXml = joueurXml->NextSiblingElement() )
	{
		// On s'assure de ne pas ajouter plus que deux joueurs
		if(nbJoueurs>= 2)
			break;
		SPJoueurAbstrait joueur( JoueurAbstrait::usineJoueurXML(joueurXml, profilsVirtuelsExistants) );
		if(joueur)
		{
			assignerJoueur(joueur);
			nbJoueurs++;
		}
		else
		{
			utilitaire::afficherErreur("Erreur: Problème pour l'initialisation d'un joueur dans une partie, joueur humain aléatoire generé");
			joueur.reset(new JoueurHumain());
			joueur->genererAleatoirement();
			assignerJoueur(joueur);
			nbJoueurs++;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::reinitialiserPartie()
///
/// Permet de reinitialiser la partie courante
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::reinitialiserPartie()
{
    SignalGameOver();
	pointsJoueurGauche_ = 0;
	pointsJoueurDroit_ = 0;
	// Reinitialisation du noeudAffichage Chiffre
	chiffres_->modifierListes("3");
	chiffres_->assignerAffiche(false);
	modifierEstPret(true);
	tempsJeu_.reset_Time();
	// Mise au jeu
	//miseAuJeu();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::assignerControlesMaillet( NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit )
///
/// Methode pour indiquer au maillet par qui ils sont controlles
///
/// @param[in] NoeudMaillet * mailletGauche
/// @param[in] NoeudMaillet * mailletDroit
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::assignerControlesMaillet( NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit, NoeudRondelle* rondelle ) throw(std::logic_error)
{
	if(joueurDroit_ && joueurGauche_)
	{
		if(mailletGauche && mailletDroit && rondelle)
		{
            joueurGauche_->setControlingMallet(mailletGauche);
            joueurDroit_->setControlingMallet(mailletDroit);
			mailletGauche->setIsLeft(true);
			mailletDroit->setIsLeft(false);
			mailletGauche->setKeyboardControlled(false);
			if(joueurGauche_->obtenirType() == JOUEUR_HUMAIN)
			{
				mailletGauche->setIsAI(false);
			}
			else
			{
				mailletGauche->setIsAI(true);
				
				mailletGauche->setAIPlayer((JoueurVirtuel*)joueurGauche_.get());
			}
			if(joueurDroit_->obtenirType() == JOUEUR_HUMAIN)
			{
				mailletDroit->setIsAI(false);
				// Si le maillet gauche est controller par lordinateur alors le maillet droit est controle par la souris
				mailletDroit->setKeyboardControlled(!mailletGauche->obtenirEstControleParOrdinateur());
			}
			else
			{
				mailletDroit->setIsAI(true);
				mailletDroit->setAIPlayer((JoueurVirtuel*)joueurDroit_.get());
			}
            mailletGauche->buildMouseJoint();
            mailletDroit->buildMouseJoint();
		}
		else
			throw std::logic_error("Tente d'assigner les controles a des maillets et/ou rondelle non valides");
	}
	else
	{
		throw std::logic_error("Tente d'assigner les controles a des maillets lorsqu'il manque encore des joueurs dans la partie");
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::miseAuJeu()
///
/// Initialisations pour le début d'une partie.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::miseAuJeu( bool debutDePartie /*= false */ )
{
	// Obtention des éléments
	NoeudRondelle* rondelle = FacadeModele::getInstance()->obtenirRondelle();
	NoeudMaillet* maillet1 = FacadeModele::getInstance()->obtenirMailletJoueurGauche();
	NoeudMaillet* maillet2 = FacadeModele::getInstance()->obtenirMailletJoueurDroit();

	// Positionnement
	rondelle->setPosition(rondelle->obtenirPositionOriginale());
	maillet1->setPosition(maillet1->obtenirPositionOriginale());
	maillet2->setPosition(maillet2->obtenirPositionOriginale());
	rondelle->modifierVitesseRotation(0);

	int dureeAnimationIntro = 0;
	

	if(debutDePartie)
	{
		dureeAnimationIntro = 3500;


		AnimationFrame* frame[7];
		frame[0] = new AnimationFrame(0, Vecteur3(-300, -150, 200), Vecteur3(0, 0, 0), Vecteur3(0, 0, 1));
		frame[1] = new AnimationFrame(500, Vecteur3(300, -150, 200), Vecteur3(0, 0, 0), Vecteur3(0, 0, 1));
		frame[2] = new AnimationFrame(1000, Vecteur3(300, 150, 200), Vecteur3(0, 0, 0), Vecteur3(0, 0, 1));
		frame[3] = new AnimationFrame(1700, Vecteur3(-300, 150, 200), Vecteur3(0, 0, 0), Vecteur3(0, 0, 1));
		frame[4] = new AnimationFrame(2000, Vecteur3(-300, -150, 200), Vecteur3(0, 0, 0), Vecteur3(0, 0, 1));
		frame[5] = new AnimationFrame(2500, Vecteur3(300, 0, 0), Vecteur3(150, 0, 0), Vecteur3(0, 0, 1));
		frame[6] = new AnimationFrame((float)dureeAnimationIntro, Vecteur3(0, -0.0001f, 300), Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));


		Animation* animation = new Animation(BEZIER, true, true, true);
		for(int i=0; i<7; i++)
			animation->ajouterFrame(frame[i]);

		vue::Camera* cameraCourante = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
		animation->ajouterObjet((ObjetAnimable*)cameraCourante);
		GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
	
		
		tempsJeu_.reset_Time();
	}
	delais(4100+dureeAnimationIntro);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::afficherScore() const
///
/// Affichae le pointage dans un message d'erreur (temporaire, bien sûr).
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::afficherScore() const
{
	return; // Non utilisée pour l'instant
	FacadeModele::getInstance()->togglePause();
    std::ostringstream mess;
    mess << "Pointage: ";
    mess << obtenirPointsJoueurGauche();
    mess << "\t";
    mess << obtenirPointsJoueurDroit();
	utilitaire::afficherErreur(mess.str());
	FacadeModele::getInstance()->togglePause();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::delais( int time )
///
/// Gestion du délai de mise au jeu.
///
/// @param[in] int time : Le temps écoulé dans le jeu.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::delais( int time )
{
	modifierEstPret(false);
	minuterie_ = time;
	if(chiffres_==0)
		return;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::updateMinuterie( int time )
///
/// Mise à jour de la minuterie.
///
/// @param[in] int time : Le temps écoulé dans le jeu.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::updateMinuterie( int time )
{
	if(estPret_)
		return;

	if(estEnPause())
	{
		delais(3100);
		return;
	}

	int minuterieAvant = minuterie_;
	minuterie_ -= (time*2);
	if(minuterie_ < 0)
	{
		//FacadeModele::obtenirInstance()->obtenirVue()->centrerCamera(FacadeModele::obtenirInstance()->obtenirLargeurZoneEdition());

		/////

		minuterie_ = 0;
		chiffres_->modifierListes("3");
		chiffres_->assignerAffiche(false);
		modifierEstPret(true);
		return;
	}

	
	if((minuterieAvant/1000-minuterie_/1000)!=0)
	{
		if(chiffres_!=0)
		{
			int lequel = (minuterie_/1000+1);
			if(lequel!=1 && lequel!=2 && lequel!=3)
				return;
			chiffres_->assignerAffiche(true);
			SoundFMOD::obtenirInstance()->playEffect(BEEP_EFFECT);

            // Creation d'une chaine avec le charactère 0
            std::string numeroString = "0";
            // le charactère 0 + une valeur entre 0 et 9 donne le charactère désiré
            numeroString[0] += lequel;
            chiffres_->modifierListes(numeroString);

			chiffres_->resetEchelle();
			if(lequel == 2)
				FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->obtenirLargeurZoneEdition());
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::modifierJoueurDroit( JoueurAbstrait* val )
///
/// Assignation du joueur de droite à la partie
///
/// @param[in] SPJoueurAbstrait val : Pointeur sur le joueur à assigner à cette partie
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::modifierJoueurDroit( SPJoueurAbstrait val )
{
// 	if(joueurDroit_)
// 		delete joueurDroit_;
	joueurDroit_ = val;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::modifierJoueurGauche( SPJoueurAbstrait val )
///
/// Assignation du joueur de gauche à la partie
///
/// @param[in] SPJoueurAbstrait val : Pointeur sur le joueur à assigner à cette partie
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::modifierJoueurGauche( SPJoueurAbstrait val )
{
// 	if(joueurGauche_)
// 		delete joueurGauche_;
	joueurGauche_ = val;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::afficher()
///
/// Effectue l'affichage des elements d'une partie
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::afficher()
{
	
	

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::vider()
///
/// Supprime les joueurs et reinitialise les variables
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::vider()
{
	pointsJoueurGauche_ = 0;
	pointsJoueurDroit_ = 0;
	joueurGauche_.reset();
	joueurDroit_.reset();
// 	modifierJoueurGauche(0);
// 	modifierJoueurDroit(0);
	enPause_ = false;
	estPret_ = false;
	faitPartieDunTournoi_ = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::terminerSi2AI()
///
/// Termine la partie si les 2 joueur sont des AI
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool Partie::terminerSi2AI()
{
	if(partieVirtuelle())
	{
		JoueurVirtuel* jvg = dynamic_cast<JoueurVirtuel*>(obtenirJoueurGauche().get());
		JoueurVirtuel* jvd = dynamic_cast<JoueurVirtuel*>(obtenirJoueurDroit().get());
		if(jvg && jvd)
		{
			// Score joueur gauche
			unsigned int s1 = (110-jvg->obtenirProbabiliteEchec())*jvg->obtenirVitesse();
			// Score joueur droit
			unsigned int s2 = (110-jvd->obtenirProbabiliteEchec())*jvd->obtenirVitesse();
			// Probabilite du joueur gauche de gagner
			unsigned int p1 = (int)(( s1/(float(s1+s2)) )*100);

			// On roule de pour savoir qui a gagner
			if(rand()%100 <= (int)p1)
			{
				pointsJoueurGauche_ = POINTAGE_GAGNANT;
				pointsJoueurDroit_ = rand()%POINTAGE_GAGNANT;
			}
			else
			{
				pointsJoueurDroit_ = POINTAGE_GAGNANT;
				pointsJoueurGauche_ = rand()%POINTAGE_GAGNANT;
			}
			return true;
		}
		else
		{
			utilitaire::afficherErreur("Les joueurs de la partie sont indiqués comme joueur virtuel mais ne le sont pas");
		}
	}
	return false;
}

bool Partie::partieVirtuelle()
{
	if(obtenirJoueurGauche() && obtenirJoueurDroit())
	{
		return obtenirJoueurGauche()->obtenirType() == JOUEUR_VIRTUEL && obtenirJoueurDroit()->obtenirType() == JOUEUR_VIRTUEL;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PositionJoueur Partie::obtenirPositionGagant()
///
/// Permet de savoir la position du gagnant
///
///
/// @return PositionJoueur
///
////////////////////////////////////////////////////////////////////////
PositionJoueur Partie::obtenirPositionGagant()
{
	if(!partieTerminee())
		return GAGNANT_AUCUN;
	return pointsJoueurGauche_ > pointsJoueurDroit_ ? GAGNANT_GAUCHE : GAGNANT_DROITE;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::SignalGameOver()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::SignalGameOver()
{
    if(joueurGauche_)
    {
        NoeudMaillet* mailletGauche = joueurGauche_->getControlingMallet();

        if(mailletGauche)
        {
            mailletGauche->destroyMouseJoint();
        }

        joueurGauche_->setControlingMallet(NULL);
    }
    if(joueurDroit_)
    {
        NoeudMaillet* mailletDroit = joueurDroit_->getControlingMallet();

        if(mailletDroit)
        {
            mailletDroit->destroyMouseJoint();
        }

        joueurDroit_->setControlingMallet(NULL);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

