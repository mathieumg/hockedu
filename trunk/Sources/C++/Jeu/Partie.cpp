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
#include "Terrain.h"
#include "ExceptionJeu.h"
#include "NodeModelRender.h"
#include "RazerGameTree.h"


GLuint Partie::listePause_ = 0;
const int Partie::POINTAGE_GAGNANT = 7;


////////////////////////////////////////////////////////////////////////
///
/// @fn  Partie::Partie( SPJoueurAbstrait joueurGauche /*= nullShrPtr*/, SPJoueurAbstrait joueurDroit /*= nullShrPtr*/ )
///
/// Constructeur qui initialise la partie.
///
/// @param[in] SPJoueurAbstrait joueurGauche : le premier joueur.
/// @param[in] SPJoueurAbstrait joueurDroit  : le deuxi�me joueur.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Partie::Partie(SPJoueurAbstrait joueurGauche /*= 0*/, SPJoueurAbstrait joueurDroit /*= 0*/, int uniqueGameId /*= 0*/, const std::vector<GameUpdateCallback>& updateCallback /*= 0*/ ):
pointsJoueurGauche_(0),pointsJoueurDroit_(0),joueurGauche_(joueurGauche),joueurDroit_(joueurDroit), estPret_(false), faitPartieDunTournoi_(false), mPartieSyncer(uniqueGameId, 60, joueurGauche, joueurDroit)
{
    chiffres_ = new NoeudAffichage("3");
    mField = new Terrain(this);
	mUniqueGameId = uniqueGameId;
	mUpdateCallbacks = updateCallback;
    mGameStatus = GAME_NOT_READY;
    mLastGameStatus = GAME_NOT_READY;
    mRequirePassword = false;
    mPassword = "";
    mPartieSyncer.setPlayers(joueurGauche, joueurDroit);
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
    mGameStatus = GAME_ENDED;
    SignalGameOver();
    delete mField;
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
/// @return pointsJoueurDroit_ : le nombre de points du deuxi�me joueur.
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
/// @return Le nom du deuxi�me joueur.
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
/// @fn std::string Partie::getFieldName() const
///
/// Accesseur du nom du terrain.
///
/// @return Le nom du terrain.
///
////////////////////////////////////////////////////////////////////////
const std::string& Partie::getFieldName() const
{
    return mField->getNom();
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
void Partie::setFieldName( const std::string& terrain )
{
    mField->modifierNom(terrain);
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
	callGameUpdate(GAME_SCORE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::incrementerPointsJoueurDroit()
///
/// Donne un point au deuxi�me joueur.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Partie::incrementerPointsJoueurDroit()
{
    pointsJoueurDroit_++;
	callGameUpdate(GAME_SCORE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SPJoueurAbstrait Partie::obtenirGagnant() const
///
/// Retourne le gagnant de la partie
///
/// @return gagnant ou 0 si la partie n'est pas termin�e
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
/// Assignation d'un joueur � la partie
///
/// @param[in] SPJoueurAbstrait joueur : joueur � ajouter
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
    callGameUpdate(mGameStatus);
    mPartieSyncer.setPlayers(joueurGauche_, joueurDroit_);
//  else
//      delete joueur;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* Partie::creerNoeudXML()
///
/// Cr�ation de l'�l�ment XML contenant la partie
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
    elementNoeud->SetAttribute("Terrain", getFieldName().c_str());

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
bool Partie::initialiserXML( const XmlElement* elem, ConteneurJoueur* profilsVirtuelsExistants /*= 0*/ )
{
    if(elem->QueryIntAttribute("ptsG", &pointsJoueurGauche_) != TIXML_SUCCESS)
        pointsJoueurGauche_ = 0;        //Probleme pour le chargement des points, on assigne un pointage nul
    if(elem->QueryIntAttribute("ptsD", &pointsJoueurDroit_) != TIXML_SUCCESS)
        pointsJoueurDroit_ = 0;     //Probleme pour le chargement des points, on assigne un pointage nul

    // On recherche le terrain
    setFieldName("");
    const char* map = elem->Attribute("Terrain");
    if(map)
    {
        setFieldName(map);
    }

    const XmlElement* joueurXml = elem->FirstChildElement();
    int nbJoueurs=0;
    for( ; joueurXml/*V�rifie si child est non-null*/; joueurXml = joueurXml->NextSiblingElement() )
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
            utilitaire::afficherErreur("Erreur: Probl�me pour l'initialisation d'un joueur dans une partie, joueur humain al�atoire gener�");
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
    //SignalGameOver();
    pointsJoueurGauche_ = 0;
    pointsJoueurDroit_ = 0;
    // Reinitialisation du noeudAffichage Chiffre
    chiffres_->modifierListes("3");
    chiffres_->setVisible(false);
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
void Partie::assignerControlesMaillet( NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit, NoeudRondelle* rondelle ) 
{
    if(joueurDroit_ && joueurGauche_)
    {
        // Si joueur 2 humain aussi

        if(mailletGauche && mailletDroit && rondelle)
        {

            joueurGauche_->setControlingMallet(mailletGauche);
            joueurDroit_->setControlingMallet(mailletDroit);
            mailletGauche->setIsLeft(true);
            mailletDroit->setIsLeft(false);

            SPJoueurAbstrait wJoueurs[2] = {joueurGauche_, joueurDroit_};
            NoeudMaillet* wMaillets[2] = {mailletGauche, mailletDroit};

            for(int i=0; i<2; ++i)
            {
                switch(wJoueurs[i]->obtenirType())
                {
                case JOUEUR_HUMAIN:
                    {
                        wMaillets[i]->setIsAI(false);
                        // Si le maillet gauche est controller par lordinateur alors le maillet droit est controle par la souris
                        wMaillets[i]->setKeyboardControlled(i==1 && wJoueurs[0]->obtenirType() == JOUEUR_HUMAIN);
                        wMaillets[i]->setIsNetworkPlayer(false);
                        break;
                    }
                case JOUEUR_NETWORK:
                case JOUEUR_NETWORK_SERVEUR:
                    {
                        wMaillets[i]->setIsAI(false);
                        wMaillets[i]->setIsNetworkPlayer(true);
                        break;
                    }
                case JOUEUR_VIRTUEL:
                    {
                        wMaillets[i]->setIsNetworkPlayer(false);
                        wMaillets[i]->setIsAI(true);
                        //wMaillets[i]->setPlayer((JoueurVirtuel*)wJoueurs[i].get());
                        break;
                    }
                default:
                    throw ExceptionJeu("Combinaison de joueurs invalides");
                    break;
                }
                wMaillets[i]->setPlayer(wJoueurs[i].get());
                wMaillets[i]->buildMouseJoint();
            }
// 
//             // Ancien code
//             if(joueurGauche_->obtenirType() == JOUEUR_NETWORK)
//             {
//                 mailletGauche->setIsAI(false);
//                 mailletGauche->setIsNetworkPlayer(true);
//                 mailletGauche->buildMouseJoint(true);
//             }
//             else
//             {
//                 mailletGauche->setKeyboardControlled(false);
//                 mailletGauche->setIsNetworkPlayer(false);
//                 if(joueurGauche_->obtenirType() == JOUEUR_HUMAIN)
//                 {
//                     mailletGauche->setIsAI(false);
//                 }
//                 else
//                 {
//                     mailletGauche->setIsAI(true);
//                     mailletGauche->setAIPlayer((JoueurVirtuel*)joueurGauche_.get());
//                 }
//                 mailletGauche->buildMouseJoint();
//             }
// 
//             if(joueurDroit_->obtenirType() == JOUEUR_NETWORK)
//             {
//                 mailletDroit->setIsAI(false);
//                 mailletDroit->setIsNetworkPlayer(true);
//                 mailletDroit->buildMouseJoint(true);
//             }
//             else
//             {
//                 mailletDroit->setIsNetworkPlayer(false);
//                 if(joueurDroit_->obtenirType() == JOUEUR_HUMAIN)
//                 {
//                     mailletDroit->setIsAI(false);
//                     // Si le maillet gauche est controller par lordinateur alors le maillet droit est controle par la souris
//                     mailletDroit->setKeyboardControlled(!mailletGauche->obtenirEstControleParOrdinateur());
//                 }
//                 else
//                 {
//                     mailletDroit->setIsAI(true);
//                     mailletDroit->setAIPlayer((JoueurVirtuel*)joueurDroit_.get());
//                 }
//                 mailletDroit->buildMouseJoint();
//             }

        }
        else
        {
            throw ExceptionJeu("Tente d'assigner les controles a des maillets et/ou rondelle non valides");
        }
    }
    else
    {
        throw ExceptionJeu("Tente d'assigner les controles a des maillets lorsqu'il manque encore des joueurs dans la partie");
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::miseAuJeu()
///
/// Initialisations pour le d�but d'une partie.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::miseAuJeu( bool debutDePartie /*= false */ )
{
    mGameStatus = GAME_STARTED;
    // Obtention des �l�ments
    NoeudRondelle* rondelle = mField->getPuck();
    NoeudMaillet* maillet1 = mField->getLeftMallet();
    NoeudMaillet* maillet2 = mField->getRightMallet();

    // Positionnement
    rondelle->setPosition(rondelle->obtenirPositionOriginale());
    rondelle->modifierVitesseRotation(0);

    maillet1->setPosition(maillet1->obtenirPositionOriginale());
    maillet2->setPosition(maillet2->obtenirPositionOriginale());

    int dureeAnimationIntro = 0;
    

    if(debutDePartie)
    {
        tempsJeu_.reset_Time();
    }
    delais(4100+dureeAnimationIntro);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::delais( int time )
///
/// Gestion du d�lai de mise au jeu.
///
/// @param[in] int time : Le temps �coul� dans le jeu.
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
/// Mise � jour de la minuterie.
///
/// @param[in] int time : Le temps �coul� dans le jeu.
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
        minuterie_ = 0;
        chiffres_->modifierListes("3");
        chiffres_->setVisible(false);
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
            chiffres_->setVisible(true);
            SoundFMOD::obtenirInstance()->playEffect(BEEP_EFFECT);

            // Creation d'une chaine avec le charact�re 0
            std::string numeroString = "0";
            // le charact�re 0 + une valeur entre 0 et 9 donne le charact�re d�sir�
            numeroString[0] += lequel;
            chiffres_->modifierListes(numeroString);

            chiffres_->resetEchelle();
            if(lequel == 2)
                FacadeModele::getInstance()->obtenirVue()->centrerCamera(mField->GetTableWidth());
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::modifierJoueurDroit( JoueurAbstrait* val )
///
/// Assignation du joueur de droite � la partie
///
/// @param[in] SPJoueurAbstrait val : Pointeur sur le joueur � assigner � cette partie
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::modifierJoueurDroit( SPJoueurAbstrait val )
{
//  if(joueurDroit_)
//      delete joueurDroit_;
    joueurDroit_ = val;
    callGameUpdate(mGameStatus);
    mPartieSyncer.setPlayers(NULL, joueurDroit_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::modifierJoueurGauche( SPJoueurAbstrait val )
///
/// Assignation du joueur de gauche � la partie
///
/// @param[in] SPJoueurAbstrait val : Pointeur sur le joueur � assigner � cette partie
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::modifierJoueurGauche( SPJoueurAbstrait val )
{
//  if(joueurGauche_)
//      delete joueurGauche_;
    joueurGauche_ = val;
    callGameUpdate(mGameStatus);
    mPartieSyncer.setPlayers(joueurGauche_, NULL);
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
    mField->renderField();
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
//  modifierJoueurGauche(0);
//  modifierJoueurDroit(0);
    //enPause_ = false;
    mGameStatus = mLastGameStatus = GAME_NOT_READY;
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
            utilitaire::afficherErreur("Les joueurs de la partie sont indiqu�s comme joueur virtuel mais ne le sont pas");
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
    mGameStatus = GAME_ENDED;
	// On appelle le callback avant de remettre les informations a 0
	callGameUpdate(GAME_ENDED);
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

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Partie::getReadyToPlay()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Partie::getReadyToPlay()
{
    if(getFieldName().size() == 0)
    {
        mField->creerTerrainParDefaut(FacadeModele::FICHIER_TERRAIN_EN_COURS);
    }
    else
    {
        RazerGameUtilities::LoadFieldFromFile(getFieldName(),*mField);
    }

    mField->fullRebuild();

    if(!mField->verifierValidite())
    {
        return false;
    }
    
    try
    {
        assignerControlesMaillet(mField->getLeftMallet(),mField->getRightMallet(),mField->getPuck());
    }
    catch(ExceptionJeu& e)
    {
        utilitaire::afficherErreur(e.what());
        return false;
    }

    mField->setTableItemsSelection(false);
    mField->setTableControlPointVisible(false);

	// On appelle le callback pour dire que la partie va commencer
	callGameUpdate(GAME_STARTED);
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::animer( const float& temps )
///
/// /*Description*/
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::animer( const float& temps )
{
    if(getGameStatus() >= GAME_STARTED)
    {
        updateMinuterie((int)(temps*1000));
        chiffres_->tick(temps);
        mField->animerTerrain(temps);
        if(!GestionnaireAnimations::obtenirInstance()->estJouerReplay())
        {
            auto zamboni = mField->getZamboni();
            if(estPret() && !estEnPause() && !partieTerminee())
            {
                if(zamboni)
                {
                    zamboni->setVisible(false);
                    zamboni->setPosition(Vecteur3());
                }
                // Gestion de la physique du jeu
                mField->appliquerPhysique(temps);
            }
            else
            {
                if(zamboni)
                {
                    zamboni->setVisible(true);
                }
                Vecteur3 pos = zamboni->getPosition();
                auto angle = utilitaire::DEG_TO_RAD(zamboni->getAngle());
                Vecteur3 direction;
                direction[VX] = cos(angle);
                direction[VY] = sin(angle);
                zamboni->setPosition(pos+direction);
            }
        }
        mPartieSyncer.tick();
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::animerBase( const float& temps )
///
/// Animation de base sans replay ni rien de ca (utile pour le serveur de jeu)
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::animerBase( const float& temps )
{
    updateMinuterie((int)(temps*1000));
    // On va plus deep pour ne pas render d'items d'affichage
    if(mField)
    {
        auto wLogicTree = mField->getLogicTree();
        if(wLogicTree)
        {
            wLogicTree->tick(temps);
        }
    }
    if(estPret() && !estEnPause() && !partieTerminee())
    {
        // Gestion de la physique du jeu
        mField->appliquerPhysique(temps);
    }
    mPartieSyncer.tick();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::callGameUpdate()
///
/// Methode qui appelle le callback de la partie s'il est valide
///
/// @param[in] :	Status a envoyer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::callGameUpdate(GameStatus pGameStatus) const
{
    for(auto it = mUpdateCallbacks.begin(); it!=mUpdateCallbacks.end(); ++it)
    {
		(*it)(mUniqueGameId, pGameStatus);
	}
}



void Partie::modifierEnPause( bool val )
{
    checkf(mGameStatus == GAME_PAUSED, "On ne doit pas mettre en pause une partie qui l'est deja");
    if(val)
    {
        tempsJeu_.pause();
        setGameStatus(GAME_PAUSED);
    }
    else
    {
        tempsJeu_.unPause();
        setGameStatus(mLastGameStatus); // Utilise le dernier etat de partie pour unpause
    }
}



// Le mot de passe ne doit pas sortir de l'objet
bool Partie::validatePassword( const std::string& pPasswordToValidate ) const
{
    return mPassword == pPasswordToValidate;
}



void Partie::setPassword( const std::string& pPassword )
{
    mPassword = pPassword;
    if(pPassword.length() == 0)
    {
        mRequirePassword = false;
    }
}



bool Partie::isNetworkClientGame() const
{
    // Est une partie network point de vue du client si un joueur network y est connecte
    SPJoueurAbstrait wJoueurGauche = obtenirJoueurGauche();
    SPJoueurAbstrait wJoueurDroit = obtenirJoueurDroit();

    if(wJoueurGauche && wJoueurDroit)
    {
        return wJoueurGauche->obtenirType() == JOUEUR_NETWORK || wJoueurDroit->obtenirType() == JOUEUR_NETWORK;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

