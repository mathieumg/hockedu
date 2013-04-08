//////////////////////////////////////////////////////////////////////////////
/// @file Partie.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#if BOX2D_DEBUG
#include <Box2D/Box2D.h>
#endif

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
#include "XMLUtils.h"
#include "Terrain.h"
#include "ExceptionJeu.h"
#include "NodeModelRender.h"
#include "RazerGameTree.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\Paquets\PaquetGameEvent.h"
#include "..\Reseau\RelayeurMessage.h"
#include "Solution_Defines.h"
#include "LaunchAchievementLite.h"
#include "..\Reseau\ControllerServeurJeu.h"


GLuint Partie::listePause_ = 0;
const int Partie::POINTAGE_GAGNANT = 3;


////////////////////////////////////////////////////////////////////////
///
/// @fn  Partie::Partie( SPJoueurAbstrait joueurGauche /*= nullShrPtr*/, SPJoueurAbstrait joueurDroit /*= nullShrPtr*/ )
///
/// Constructeur qui initialise la partie.
///
/// @param[in] SPJoueurAbstrait joueurGauche : le premier joueur.
/// @param[in] SPJoueurAbstrait joueurDroit  : le deuxième joueur.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Partie::Partie(GameType gameType, SPJoueurAbstrait joueurGauche /*= 0*/, SPJoueurAbstrait joueurDroit /*= 0*/, int uniqueGameId /*= 0*/, const std::vector<GameUpdateCallback>& updateCallback /*= 0*/ ):
pointsJoueurGauche_(0),pointsJoueurDroit_(0),joueurGauche_(joueurGauche),joueurDroit_(joueurDroit), faitPartieDunTournoi_(false), mPartieSyncer(uniqueGameId, 60, joueurGauche, joueurDroit),
mGameType(gameType),mMiseAuJeuDelai(4100)
{
    mClockLastTick = 0;
    chiffres_ = new NoeudAffichage("3");
    mField = new Terrain(this);
    mFieldSimulation = new Terrain(this);
    mFieldSimulation->setIsSimulation(true);

	mUniqueGameId = uniqueGameId;
	mUpdateCallbacks = updateCallback;
    mGameStatus = GAME_NOT_READY;
    mLastGameStatus = GAME_NOT_READY;
    mRequirePassword = false;
    mPassword = "";
    mPartieSyncer.setPlayers(joueurGauche, joueurDroit);

    GestionnaireAnimations::obtenirInstance()->attach(this);
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
    delete mFieldSimulation;
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
    mFieldSimulation->modifierNom(terrain);
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
void Partie::incrementerPointsJoueurGauche(bool pForceUpdate /*= false*/)
{
    pointsJoueurGauche_++;
    if(isNetworkServerGame())
    {
        // Dans le cas, il faut envoyer un paquet aux 2 clients
        PaquetGameEvent* wPaquet = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
        wPaquet->setEventOnPlayerLeft(true);
        wPaquet->setGameId(mUniqueGameId);
        wPaquet->setEvent(GAME_EVENT_PLAYER_SCORED);
        RelayeurMessage::obtenirInstance()->relayerPaquetGame(mUniqueGameId, wPaquet);
        if(partieTerminee())
        {
            setGameStatus(GAME_ENDED);
        }
    }
    else if(pForceUpdate || !isNetworkClientGame())
    {
        // Dans le cas du client en reseau, on ne fait rien puisque c'est le serveur qui va nous envoyer l'evenement du but
        if(partieTerminee())
        {
            setGameStatus(GAME_ENDED);
            SendAchievementEventToHumanPlayer(joueurGauche_,ACHIEVEMENT_EVENT_GAME_WON,ACHIEVEMENT_EVENT_GAME_LOOSE);
        }
        else
        {
            callGameUpdate(GAME_SCORE);
        }


        
    }
    
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
void Partie::incrementerPointsJoueurDroit(bool pForceUpdate /*= false*/)
{
    pointsJoueurDroit_++;
    if(isNetworkServerGame())
    {
        // Dans le cas, il faut envoyer un paquet aux 2 clients
        PaquetGameEvent* wPaquet = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
        wPaquet->setEventOnPlayerLeft(false);
        wPaquet->setGameId(mUniqueGameId);
        wPaquet->setEvent(GAME_EVENT_PLAYER_SCORED);
        RelayeurMessage::obtenirInstance()->relayerPaquetGame(mUniqueGameId, wPaquet);
        if(partieTerminee())
        {
            setGameStatus(GAME_ENDED);
        }
    }
    else if(pForceUpdate || !isNetworkClientGame()) // forceUpdate si on a recu le message du serveur et il faut incrementer
    {
        // Dans le cas du client en reseau, on ne fait rien puisque c'est le serveur qui va nous envoyer l'evenement du but
        if(partieTerminee())
        {
            setGameStatus(GAME_ENDED);
            SendAchievementEventToHumanPlayer(joueurDroit_,ACHIEVEMENT_EVENT_GAME_WON,ACHIEVEMENT_EVENT_GAME_LOOSE);
        }
        else
        {
            callGameUpdate(GAME_SCORE);
        }
        

    }
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
    callGameUpdate(mGameStatus);
    mPartieSyncer.setPlayers(joueurGauche_, joueurDroit_);
//  else
//      delete joueur;
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
    pointsJoueurGauche_ = 0;
    pointsJoueurDroit_ = 0;
    // Reinitialisation du noeudAffichage Chiffre
    chiffres_->setSkinKey(RAZER_KEY_MODEL_3);
    chiffres_->setVisible(false);
    tempsJeu_.unPause();
    tempsJeu_.reset_Time();
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
                case JOUEUR_VIRTUEL_RENFORCEMENT:
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
                wMaillets[i]->setPlayer(wJoueurs[i]);
                wMaillets[i]->buildMouseJoint();
            }
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



void Partie::reloadControleMallet()
{
    checkf(getGameStatus() == GAME_PAUSED, "La partie doit avoir ete initialise avant de pouvoir appeler un reload sur les maillets");
    Terrain* wField = getField();
    checkf(wField, "Terrain invalide lors du reloadMallet");
    assignerControlesMaillet(wField->getLeftMallet(), wField->getRightMallet(), wField->getPuck()); 


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
    setGameStatus(GAME_STARTED);
    // Obtention des éléments
    NoeudRondelle* rondelle = mField->getPuck();
    NoeudMaillet* maillet1 = mField->getLeftMallet();
    NoeudMaillet* maillet2 = mField->getRightMallet();

    mField->setPuckZone(PUCK_ZONE_UNKNOWN);

    // Positionnement
    rondelle->setPosition(rondelle->obtenirPositionOriginale());
    rondelle->modifierVelocite(0);
    rondelle->modifierVitesseRotation(0);
    rondelle->setAngle(0);

    maillet1->setPosition(maillet1->obtenirPositionOriginale());
    maillet2->setPosition(maillet2->obtenirPositionOriginale());
    maillet1->setTargetDestination(maillet1->obtenirPositionOriginale(), true);
    maillet2->setTargetDestination(maillet2->obtenirPositionOriginale(), true);



    if(debutDePartie)
    {
        tempsJeu_.reset_Time();
    }
    delais(mMiseAuJeuDelai);
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
    setGameStatus(GAME_WAITING);
    mClockDelaisDone = clock() + time;
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
    if(mGameStatus == GAME_WAITING)
    {
        // Si delais termine
        if(clock() > mClockDelaisDone-1000)
        {
            mClockDelaisDone = 0;
            chiffres_->setSkinKey(RAZER_KEY_MODEL_3);
            chiffres_->setVisible(false);
            setGameStatus(GAME_STARTED);
            Vecteur3 coordonneesSouris;
            
            FacadeModele::getInstance()->convertirClotureAVirtuelle(mMousePosScreen[VX], mMousePosScreen[VY], coordonneesSouris);
            if(joueurGauche_->obtenirType() == JOUEUR_HUMAIN)
            {
                NoeudMaillet* mailletGauche = getField()->getLeftMallet();
                if(mailletGauche && !mailletGauche->obtenirEstControleParOrdinateur())
                {
                    mailletGauche->setTargetDestination(coordonneesSouris, true);
                }
            }
            if(joueurDroit_->obtenirType() == JOUEUR_HUMAIN)
            {
                NoeudMaillet* mailletDroit = getField()->getRightMallet();
                if(mailletDroit && !mailletDroit->obtenirEstControleParOrdinateur())
                {
                    mailletDroit->setTargetDestination(coordonneesSouris, true);
                }
            }
            mClockLastTick = 0;
            tempsJeu_.unPause();
            return;
        }
        
        // Si on est a une seconde pile (beep et change de chiffre)
        clock_t wClockActuel = clock();
        int lequel = (mClockDelaisDone - wClockActuel)/1000;
        int lequelLastTick = (mClockDelaisDone - mClockLastTick)/1000;

        if(mClockLastTick && lequel != lequelLastTick)
        {
            if(chiffres_!=0)
            {
                //int lequel = (mClockDelaisDone/1000+1);
                if(lequel!=1 && lequel!=2 && lequel!=3)
                    return;
                chiffres_->setVisible(true);
                SoundFMOD::obtenirInstance()->playEffect(BEEP_EFFECT);
                chiffres_->setSkinKey(RazerKey(RAZER_KEY_MODEL_1+lequel-1));
                chiffres_->resetEchelle();

                if(lequel == 2)
                    FacadeModele::getInstance()->obtenirVue()->centrerCamera(mField->GetTableWidth());
            }
        }
        mClockLastTick = clock();
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
//  if(joueurDroit_)
//      delete joueurDroit_;
    joueurDroit_ = val;
    if(joueurDroit_)
    {
	    joueurDroit_->setPlayerSide(PLAYER_SIDE_RIGHT);
    }
    callGameUpdate(mGameStatus);
    mPartieSyncer.setPlayers(NULL, joueurDroit_);
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
//  if(joueurGauche_)
//      delete joueurGauche_;
    joueurGauche_ = val;
    if(joueurGauche_)
    {
        joueurGauche_->setPlayerSide(PLAYER_SIDE_LEFT);
    }
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
    
    

#if MAT_DEBUG_
    // Render trajet rondelle

    

#if WIN32
    // États de la lumière 
    GLboolean lighting_state;
    // Désactiver l'éclairage
    glGetBooleanv(GL_LIGHTING, &lighting_state);
    glDisable(GL_LIGHTING);
    FacadeModele::getInstance()->DeActivateShaders();
#endif

    // Dessin de la zone d'édition
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glColor4f(0,0,0,1);
    glLineWidth(5.0f);

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<NoeudMaillet::mListePointsDebug.size(); i++)
    {
        Vecteur3 wVect = NoeudMaillet::mListePointsDebug[i];
        glVertex3f(wVect[VX], wVect[VY], wVect[VZ]);
    }
    glEnd();




    glPopAttrib();
    glPopMatrix();
    

//     glTranslatef(0, 0, 5.0f);
// #if BOX2D_DEBUG
//     glPushMatrix();
//     glPushAttrib(GL_ALL_ATTRIB_BITS);
// 
//     auto world = mFieldSimulation->GetWorld();
//     if(world)
//     {
//         world->DrawDebugData();
//     }
//     // Restauration de la matrice.
//     glPopAttrib();
//     glPopMatrix();
// #endif

#if WIN32
    FacadeModele::getInstance()->ActivateShaders();

    // Réactiver l'éclairage et (s'il y a lieu)
    if (lighting_state == GL_TRUE) {
        glEnable(GL_LIGHTING);
    }
#endif


#endif

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
    mGameStatus = mLastGameStatus = GAME_NOT_READY;
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
    setGameStatus(GAME_ENDED);
	// On appelle le callback avant de remettre les informations a 0
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
bool Partie::getReadyToPlay( bool loadMapFile /*= true*/ )
{
    if(loadMapFile)
    {
        if(getFieldName().size() == 0)
        {
            mField->creerTerrainParDefaut(FacadeModele::FICHIER_TERRAIN_EN_COURS);
        }
        else
        {
            RazerGameUtilities::LoadFieldFromFile(getFieldName(),*mField);
        }
    }
    mField->fullRebuild();

    auto xml = mField->creerNoeudXML();
    mFieldSimulation->initialiserXml(xml,false);
    delete xml;

    if(!mField->verifierValidite())
    {
        return false;
    }
#if BOX2D_INTEGRATED
    auto body = mFieldSimulation->getPuck()->getPhysicBody();
    for(auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits &= ~CATEGORY_MALLET;
        fixture->SetFilterData(filter);
    }
#endif
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
    mFieldSimulation->setTableItemsSelection(false);
    mFieldSimulation->setTableControlPointVisible(false);

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
    chiffres_->tick(temps);
    mField->animerTerrain(temps);
    updateMinuterie((int)(temps*1000));
    if(getGameStatus() == GAME_STARTED)
    {
        // Gestion de la physique du jeu
        mField->appliquerPhysique(temps);
        mPartieSyncer.tick();
    }
#if !MAT_DEBUG_
    if(mGameStatus == GAME_WAITING)
    {
        auto zamboni = mField->getZamboni();
        Vecteur3 pos = zamboni->getPosition();
        auto angle = utilitaire::DEG_TO_RAD(zamboni->getAngle());
        Vecteur3 direction;
        direction[VX] = cos(angle);
        direction[VY] = sin(angle);
        zamboni->setPosition(pos+direction);
    }
#endif
    
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
    if(mGameStatus == GAME_STARTED)
    {
        // Gestion de la physique du jeu
        mField->appliquerPhysique(temps);
        mPartieSyncer.tick();
    }
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
    if(val && getGameStatus() != GAME_PAUSED)
    {
        SoundFMOD::obtenirInstance()->playEffect(effect(PAUSE_EFFECT));
        setGameStatus(GAME_PAUSED);
    }
    else if(getGameStatus() == GAME_PAUSED)
    {
        SoundFMOD::obtenirInstance()->playEffect(effect(PAUSE_EFFECT));
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
    if(pPassword.length() > 0)
    {
        mRequirePassword = true;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::updateObserver( const ReplaySubject* pSubject )
///
/// /*Description*/
///
/// @param[in] const ReplaySubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::updateObserver( const ReplaySubject* pSubject )
{
    if(pSubject->mReplaying)
    {
        tempsJeu_.pause();
        setGameStatus(GAME_REPLAYING);
    }
    else
    {
        tempsJeu_.unPause();
        //checkf(mLastGameStatus != GAME_REPLAYING);
        if(mLastGameStatus != GAME_REPLAYING)
        {
            setGameStatus(mLastGameStatus); // Utilise le dernier etat de partie pour unpause
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::setGameStatus( GameStatus pStatus )
///
/// /*Description*/
///
/// @param[in] GameStatus pStatus
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::setGameStatus( GameStatus pStatus )
{
    // Check pour ne pas redemarrer une game network qui est terminee
    if(mGameStatus == GAME_ENDED && isNetworkClientGame())
    {
        return;
    }

    mLastGameStatus = mGameStatus; 
    mGameStatus = pStatus;

    // Appeler les events de changement de state (ex: pause, unpause)
    switch(pStatus)
    {
    case GAME_PAUSED:
        {
            // Quand on pause, il faut mettre le timer en pause
            tempsJeu_.pause();
            break;
        }
    case GAME_READY:
        {
            // Quand on devient pret (apres getGameReady), on reset le timer
            tempsJeu_.reset_Time();
            break;
        }
    case GAME_REPLAYING:
        {
            // Quand on regarde un replay, il faut mettre le timer en pause
            tempsJeu_.pause();
            break;
        }
    case GAME_SCORE:
        {
            // Rien de special
            break;
        }
    case GAME_STARTED:
        {
            // Quand on met la partie en cours, on unpause le timer
            tempsJeu_.unPause();
            break;
        }
    case GAME_WAITING:
        {
            // Quand on attend, il faut mettre le timer en pause
            tempsJeu_.pause();
            break;
        }
    }
    callGameUpdate(pStatus);

#if !MAT_DEBUG_
    auto zamboni = mField->getZamboni();
    if(zamboni)
    {
        if(mGameStatus == GAME_WAITING)
        {
            zamboni->setVisible(true);
        }
        else
        {
            zamboni->setVisible(false);
            zamboni->setPosition(Vecteur3());
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Partie::SendAchievementEventToHumanPlayer( SPJoueurAbstrait player,AchievementEvent eventIfHuman, AchievementEvent eventIfNonHuman )
///
/// Permet d'envoyer un evenement conditionnel selon le type de joueur qui a declancher l'événement.
/// Si les 2 joueurs de la partie sont humains on n'envoit pas l'événement, car ca ne fait pas de sens.
///
/// @param[in] SPJoueurAbstrait player : joueur qui a déclancher l'événement
/// @param[in] AchievementEvent eventIfHuman : type d'événement désiré si le joueur déclanchant est humain
/// @param[in] AchievementEvent eventIfNonHuman : type d'événement désiré si le type de joueur est nonhumain (network ou AI)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Partie::SendAchievementEventToHumanPlayer( SPJoueurAbstrait player,AchievementEvent eventIfHuman, AchievementEvent eventIfNonHuman )
{
    checkf(player == joueurGauche_ || player == joueurDroit_);
    checkf(player);

    if(joueurGauche_ && joueurDroit_)
    {
        if(isNetworkServerGame())
        {
            // server doesnt care about achievements
            return;
        }
        if(isOfflineGame())
        {
            /// it is possible to have human vs human, so we check to make sure
            SPJoueurAbstrait oppositePlayer = player == joueurGauche_ ? joueurDroit_ : joueurGauche_;
            if(oppositePlayer->obtenirType() == player->obtenirType())
            {
                /// incoherence pour les achievements, on sort
                return;
            }
        }

        /// here we can assume we have only one human player
        if(player->obtenirType() == JOUEUR_HUMAIN)
        {
            Achievements::LaunchEvent(eventIfHuman);
        }
        else
        {
            Achievements::LaunchEvent(eventIfNonHuman);
        }

    }
}



PartieServeurs* Partie::buildPartieServeurs()
{
    PartieServeurs* wPartie = new PartieServeurs;
    wPartie->setUniqueGameId(mUniqueGameId);
    wPartie->setGameName(mName);
    wPartie->setMapName(mField->getNom());
    wPartie->setPlayerName1(obtenirNomJoueurGauche());
    wPartie->setPlayerName2(obtenirNomJoueurDroit());
    wPartie->setPlayer1Score(pointsJoueurGauche_);
    wPartie->setPlayer2Score(pointsJoueurDroit_);
    wPartie->setPassword(mPassword);
    wPartie->setGameStatus(mGameStatus);

    ControllerServeurJeu* wController = dynamic_cast<ControllerServeurJeu*>(GestionnaireReseau::obtenirInstance()->getController());
    if(wController)
    {
        wPartie->setServerId(wController->getServerId());
    }

    return wPartie;
}




bool Partie::updateTerrainSimulation()
{
    // On doit sauvegarder les infos suivantes:
    // - Position rondelle
    // - Velocite rondelle
    // - Vitesse angulaire rondelle
    // - Position maillets
    // - Velocite maillets

    NoeudMaillet* wMailletGauche = mField->getLeftMallet();
    NoeudMaillet* wMailletDroit  = mField->getRightMallet();
    NoeudRondelle* wRondelle     = mField->getPuck();

    if(wMailletGauche && wMailletDroit && wRondelle)
    {
        NoeudMaillet* wMailletGaucheSimulation = mFieldSimulation->getLeftMallet();
        NoeudMaillet* wMailletDroitSimulation  = mFieldSimulation->getRightMallet();
        NoeudRondelle* wRondelleSimulation     = mFieldSimulation->getPuck();
        // Il y a probablement eu un probleme de init si ca ne fonctionne pas
        checkf(wMailletDroitSimulation && wMailletGaucheSimulation && wRondelleSimulation);
        if(wMailletDroitSimulation && wMailletGaucheSimulation && wRondelleSimulation)
        {
            // Set position
            wRondelleSimulation->setPosition(wRondelle->getPosition());
            wMailletGaucheSimulation->setPosition(wMailletGauche->getPosition());
            wMailletDroitSimulation->setPosition(wMailletDroit->getPosition());


            // Set velocity
            wRondelleSimulation->modifierVelocite(wRondelle->obtenirVelocite());
            wMailletGaucheSimulation->modifierVelocite(wMailletGauche->obtenirVelocite());
            wMailletDroitSimulation->modifierVelocite(wMailletDroit->obtenirVelocite());

            // Set angular velocity
            wRondelleSimulation->modifierVitesseRotation(wRondelle->obtenirVitesseRotation());
            
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}



PuckProjection Partie::getPuckProjection( float pPosX, int pDelaisMaxMs /*= 10000*/ )
{
#if MAT_DEBUG_
    NoeudMaillet::mListePointsDebug.clear();
#endif

    PuckProjection wReturnVal;
    wReturnVal.time = -1;
    // On sync le terrain de simulation pour trouver la position
    updateTerrainSimulation();

    NoeudRondelle* wPuck = mFieldSimulation->getPuck();
    if(wPuck)
    {
        float wDiffXStart = pPosX - wPuck->getPosition()[VX];

        
        // On itere le world box2d jusqu'a ce qu'on trouve la posX voulue ou le delais max
        int wElapsedTime = 0;
        while(true)
        {
            // Tick le world
            mFieldSimulation->appliquerPhysique(16.0f/1000.0f);
            wElapsedTime += 16;

#if MAT_DEBUG_
            NoeudMaillet::mListePointsDebug.push_back(wPuck->getPosition());
#endif
            //std::cout << "Position:" << wPuck->getPosition() << std::endl;

            // Verifie la rondelle
            //std::cout << wDiffXStart << "\t" << (pPosX - wPuck->getPosition()[VX]) << std::endl;
            if((pPosX - wPuck->getPosition()[VX]) * wDiffXStart < 0) // Change de cote de la droite du milieu
            {
                wReturnVal.position = wPuck->getPosition().convertir<2>();
                wReturnVal.time = wElapsedTime;
                wReturnVal.velocite = wPuck->obtenirVelocite().convertir<2>();
                break;
            }
            
            // Verifie delais max
            if(wElapsedTime > pDelaisMaxMs)
            {
                wReturnVal.time = -1;
                break;
            }
        }
    }
    

    return wReturnVal;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

