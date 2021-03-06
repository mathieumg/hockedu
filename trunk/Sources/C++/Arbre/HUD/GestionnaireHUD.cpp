//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireHUD.cpp
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireHUD.h"

#include "gl/GLU.h"
#include "Utilitaire.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "Tournoi.h"
#include "GestionnaireModeles.h"
#include "EventManager.h"
#include "SourisEtatAbstrait.h"
#include "GameControllerAbstract.h"
#include "PlayerAbstract.h"
#include "Phont.h"

#include "HUDBonus.h"
#include "HUDForeverAlone.h"
#include "HUDDynamicText.h"
#include "HUDForeverAlone.h"
#include "HUDSurfaceGL.h"
#include "HUDTexte.h"
#include "HUDMultiligne.h"
#include "HUDElementComparatif.h"



// D�clarations de base pour le singleton
SINGLETON_DECLARATION_CPP(GestionnaireHUD);

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireHUD::GestionnaireHUD()
///
/// Constructeur par d�faut.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GestionnaireHUD::GestionnaireHUD()
{
    mSmallTextRenderer = new Phont();
    mBigTextRenderer = new Phont();
    if(!mSmallTextRenderer->load("", (RazerGameUtilities::NOM_DOSSIER_MEDIA + "game_over").c_str()))
    {
        utilitaire::afficherErreur("Erreur: incapable d'ouvrir le font game_over");
    }
    if(!mBigTextRenderer->load("", (RazerGameUtilities::NOM_DOSSIER_MEDIA + "game_over_big_text").c_str()))
    {
        utilitaire::afficherErreur("Erreur: incapable d'ouvrir le font game_over_big_text");
    }

    HUDBonus::initSurfaces();
    creerHUDJeu();
    creerHUDTournoi();
    //creerHUDEdition();


    GestionnaireModeles::obtenirInstance()->obtenirListe(RAZER_KEY_PAUSE,listePause_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireHUD::~GestionnaireHUD( void )
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GestionnaireHUD::~GestionnaireHUD()
{
    HUDBonus::clearSurfaces();

    delete mSmallTextRenderer;
    delete mBigTextRenderer;

    racineJeu_->vider();
    delete racineJeu_;
    //racineEdition_->vider();
    //delete racineEdition_;
    racineTournoi_->vider();
    delete racineTournoi_;
}

struct InfoHud
{
    InfoHud(NomEtatSouris etat,std::string outil,std::string info):
        mEtat(etat),mOutil(outil),mInfo(info)
    {

    }
    NomEtatSouris mEtat;
    std::string mOutil;
    std::string mInfo;
};

static const InfoHud hudEditionInfos[] =
{
    //,ETAT_SOURIS_DEPLACER_FENETRE
    InfoHud(ETAT_SOURIS_DEPLACER_FENETRE,
    "D�placer la cam�ra",
    "Cliquez-glissez la souris pour d�placer la vue."),

    //,ETAT_SOURIS_ZOOM_ELASTIQUE       
    InfoHud(ETAT_SOURIS_ZOOM_ELASTIQUE,
    "Zoom �lastique",
    "Fonction qui n'existe plus. D�sol�."),

    //,ETAT_SOURIS_ZOOM_PROPORTIONNEL   
    InfoHud(ETAT_SOURIS_ZOOM_PROPORTIONNEL,
    "Zoom proportionnel",
    "Cliquez-glissez la souris pour ajuster le zoom de la vue."),

    //,ETAT_SOURIS_TRANSFORMATION_ROTATION  
    InfoHud(ETAT_SOURIS_TRANSFORMATION_ROTATION,
    "Rotation d'un objet",
    "Cet outil effectue une rotation sur tous les objets s�lectionn�s.\nCliquez-glissez de haut en bas afin d'effectuer la transformation."),

    //,ETAT_SOURIS_TRANSFORMATION_ECHELLE   
    InfoHud(ETAT_SOURIS_TRANSFORMATION_ECHELLE,
    "Mise � l'�chelle d'un objet",
    "Cet outil effectue une mise � l'�chelle sur tous les objets s�lectionn�s.\nCliquez-glissez de haut en bas afin d'effectuer la transformation."),

    //,ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT   
    InfoHud(ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT,
    "D�placer un objet",
    "Cet outil d�place tous les objets s�lectionn�s en direction de la souris.\nCliquez-glissez d'effectuer le d�placement."),

    //,ETAT_SOURIS_SELECTION            
    InfoHud(ETAT_SOURIS_SELECTION,
    "S�lection d'objets",
    "S�lectionne des objets afin de pouvoir les transformer par la suite.\n"\
    "Un clic sur un objet s�lectionne ce dernier.\n"\
    "Clic-d�placer cr�e une zone de s�lection, dans laquelle tout sera s�lectionn�.\n"\
    "Ctrl-clic ajoute des �l�ments au groupe."),

    //,ETAT_SOURIS_AJOUTER_PORTAIL  
    InfoHud(ETAT_SOURIS_AJOUTER_PORTAIL,
    "Ajout de portails",
    "Cliquez pour ajouter un portail.\nL'�l�ment sera plac� uniquement si l'endroit est valide."),

    //,ETAT_SOURIS_AJOUTER_MURET    
    InfoHud(ETAT_SOURIS_AJOUTER_MURET,
    "Ajout de murets",
    "Ajoute des murets.\nLe premier clic place le d�but du muret, le second clic place la fin du muret.\nLe muret sera trac� entre les deux clics.\nL'�l�ment sera plac� uniquement si l'endroit est valide."),

    //,ETAT_SOURIS_AJOUTER_MAILLET          
    InfoHud(ETAT_SOURIS_AJOUTER_MAILLET,
    "Ajout de maillets",
    "Cliquez pour ajouter des maillets.\nVous ne pouvez placer que deux maillets sur la surface de jeu.\nL'�l�ment sera plac� uniquement si l'endroit est valide."),

    //,ETAT_SOURIS_AJOUTER_RONDELLE         
    InfoHud(ETAT_SOURIS_AJOUTER_RONDELLE,
    "Ajout de la rondelle",
    "Cliquez pour ajouter une rondelle.\nVous ne pouvez placer qu'une seule rondelle.\nL'�l�ment sera plac� uniquement si l'endroit est valide."),

    //,ETAT_SOURIS_AJOUTER_ACCELERATEUR
    InfoHud(ETAT_SOURIS_AJOUTER_ACCELERATEUR,
    "Ajout d'un acc�l�rateur",
    "Cliquez pour ajouter un acc�l�rateur.\nL'�l�ment sera plac� uniquement si l'endroit est valide."),

    //,ETAT_SOURIS_ORBIT
    InfoHud(ETAT_SOURIS_ORBIT,
    "Changer la rotation de la cam�ra",
    "Cliquez-glissez la roulette de la souris pour d�placer la vue autour de la surface de jeu.\nLa roulette de la souris effectue un zoom.\nCliquer-glisser d�place autour du centre de rotation.(Camera Orbite)\n Ou autour de la camera en mode libre"),

};

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::creerHUDEdition()
///
/// Initialisation du HUD.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::creerHUDEdition()
{
    racineEdition_ = new HUDElement();
    //la liste de tous les outils
    
    int size = ARRAY_COUNT(hudEditionInfos);

    for(int i=0;i<size;++i)
    {
        HUDElementComparatif* panneauDeFond = new HUDElementComparatif([=]() -> bool {
            NomEtatSouris indexOutilSelectionne;
            if(!EventManager::etatCourant_ || !EventManager::etatCourant_->GetMouseStateName(indexOutilSelectionne))
                return false;
            return indexOutilSelectionne==hudEditionInfos[i].mEtat;
        });
        panneauDeFond->modifierTaille(1.0f,1.0f);
        panneauDeFond->modifierPosition(0.0f,0.0f);

        HUDTexte* nom = new HUDTexte(hudEditionInfos[i].mOutil, Vecteur4f(1.0f, 1.0f, 0.0f, 1.0f));
        nom->modifierPosition(0.01f,0.05f);
        panneauDeFond->add(nom);

        HUDMultiligne* description = new HUDMultiligne(hudEditionInfos[i].mInfo, Vecteur4f(1.0f, 1.0f, 0.0f, 1.0f));
        description->modifierPosition(0.0f,0.0f);
        description->modifierTaille(1.0f,1.0f);
        panneauDeFond->add(description);

        racineEdition_->add(panneauDeFond);
    }

    HUDTexte* messageReset= new HUDTexte("Appuyez sur '0' pour replacer la cam�ra", Vecteur4f(1.0f,1.0f,0.0f,1.0f));
    messageReset->modifierPosition(0.65f,0.95f);
    racineEdition_->add(messageReset);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::dessinerHUDEdition()
///
/// Tra�age du HUD.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::dessinerHUDEdition()
{
    // �tats de la lumi�re et de la profondeur
    GLboolean lighting_state, depth_test_state;
    // D�sactiver le test de profondeur et l'�clairage
    glGetBooleanv(GL_LIGHTING, &lighting_state);
    glGetBooleanv(GL_DEPTH_TEST, &depth_test_state);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,1,1,0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    racineEdition_->repeindre();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // PROJECTION
    glPopAttrib(); // ATTRIBUTS
    glMatrixMode(GL_MODELVIEW);

    // R�activer l'�clairage et le test de profondeur (s'il y a lieu)
    if (lighting_state == GL_TRUE) {
        glEnable(GL_LIGHTING);
    }
    if (depth_test_state == GL_TRUE) {
        glEnable(GL_DEPTH_TEST);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::creerHUDJeu()
///
/// Initialisation du HUD.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::creerHUDJeu()
{
    racineJeu_ = new HUDElement();

    // ATTENTION Chaque surface doit avoir son propre conteneur de vertexes.

    // Surface pour les scores
    HUDElementComparatif* panneauScores = new HUDElementComparatif([](){return true/*!GestionnaireAnimations::obtenirInstance()->estJouerReplay()*/;});
    panneauScores->modifierPosition(0.05f, 0.01f);
    panneauScores->modifierTaille(0.9f,0.1f);

    //Surface pour le joueur de gauche
    ConteneurVertex2D* listeDesVertexes01=new ConteneurVertex2D;
    listeDesVertexes01->push_back(Vecteur2f(1.0f,0.5f));
    listeDesVertexes01->push_back(Vecteur2f(1.0f,0.0f));
    for(int angle = 90; angle <= 270; angle+=18)
    {
        listeDesVertexes01->push_back(Vecteur2(0.1f+(0.1f*cos(utilitaire::DEG_TO_RAD((float)angle))), 0.5f-(0.5f*sin(utilitaire::DEG_TO_RAD((float)angle)))));
    }
    listeDesVertexes01->push_back(Vecteur2f(1.0f,1.0f));
    HUDSurfaceGL *surfaceJoueurGauche = new HUDSurfaceGL(GL_TRIANGLE_FAN, listeDesVertexes01, Vecteur4f(0.0f,0.0f,1.0f,0.6f));
    surfaceJoueurGauche->modifierPosition(0.0f, 0.0f);
    surfaceJoueurGauche->modifierTaille(0.5f, 1.0f);

    HUDDynamicText* nomJoueurGauche = new HUDDynamicText( []( std::stringstream& text ) { 
        const Partie* game = FacadeModele::getInstance()->obtenirPartieCourante();
        if(game)text << game->obtenirNomJoueurGauche( );
        return Vecteur4f( 1, 1, 1, 1 ); 
    }, false );

    nomJoueurGauche->modifierPosition(0.3f,0.4f);
    surfaceJoueurGauche->add(nomJoueurGauche);

    HUDDynamicText* scoreJoueurGauche = new HUDDynamicText( []( std::stringstream& text ) {
        const Partie* game = FacadeModele::getInstance()->obtenirPartieCourante();
        if( game )text << game->obtenirPointsJoueurGauche( );
        return Vecteur4f( 1, 1, 1, 1 );
    }, false );
    scoreJoueurGauche->modifierPosition(0.4f,0.9f);
    surfaceJoueurGauche->add(scoreJoueurGauche);

    panneauScores->add(surfaceJoueurGauche);

    /// Bonus HUD left player
    mLeftPlayerBonuses = new HUDBonus();
    mLeftPlayerBonuses->modifierPosition(0,1);
    mLeftPlayerBonuses->modifierTaille(0.5,1);
    panneauScores->add(mLeftPlayerBonuses);

    // Surface pour le joueur de droite
    listeDesVertexes01=new ConteneurVertex2D;
    listeDesVertexes01->push_back(Vecteur2f(0.0f,1.0f));
    for(int angle = 270; angle >= 90; angle-=18)
    {
        listeDesVertexes01->push_back(Vecteur2(1.0f-0.1f-(0.1f*cos(utilitaire::DEG_TO_RAD((float)angle))), 0.5f-(0.5f*sin(utilitaire::DEG_TO_RAD((float)angle)))));
    }
    listeDesVertexes01->push_back(Vecteur2f(0.0f,0.0f));
    listeDesVertexes01->push_back(Vecteur2(0.0f,0.5f));

    HUDSurfaceGL *surfaceJoueurDroit = new HUDSurfaceGL(GL_TRIANGLE_FAN, listeDesVertexes01, Vecteur4f(1.0f,0.0f,0.0f,0.6f));
    surfaceJoueurDroit->modifierPosition(0.5f, 0.0f);
    surfaceJoueurDroit->modifierTaille(0.5f, 1.0f);

    HUDDynamicText* nomJoueurDroit = new HUDDynamicText( []( std::stringstream& text ) {
        const Partie* game = FacadeModele::getInstance()->obtenirPartieCourante();
        if( game )text << game->obtenirNomJoueurDroit( );
        return Vecteur4f( 1, 1, 1, 1 );
    }, false );
    nomJoueurDroit->modifierPosition(0.5f,0.4f);
    surfaceJoueurDroit->add(nomJoueurDroit);

    HUDDynamicText* scoreJoueurDroit = new HUDDynamicText( []( std::stringstream& text ) {
        const Partie* game = FacadeModele::getInstance()->obtenirPartieCourante();
        if( game )text << game->obtenirPointsJoueurDroit();
        return Vecteur4f( 1, 1, 1, 1 );
    }, false );
    scoreJoueurDroit->modifierPosition(0.6f,0.9f);
    surfaceJoueurDroit->add(scoreJoueurDroit);

    panneauScores->add(surfaceJoueurDroit);

    /// Bonus HUD Puck 
    mPuckBonuses = new HUDBonus();
    mPuckBonuses->modifierPosition(0.4f,1);
    mPuckBonuses->modifierTaille(0.2f,0.1f);
    panneauScores->add(mPuckBonuses);

    /// Bonus HUD right player
    mRightPlayerBonuses = new HUDBonus();
    mRightPlayerBonuses->modifierPosition(0.7f,1);
    mRightPlayerBonuses->modifierTaille(0.3f,0.1f);
    panneauScores->add(mRightPlayerBonuses);

    // Symbole au milieu.
    listeDesVertexes01 = new ConteneurVertex2D;
    listeDesVertexes01->push_back(Vecteur2f(0.5f, 0.5f));
    for(int i=360;i>=0;i-=36)
        listeDesVertexes01->push_back(Vecteur2f((float)(0.5f+0.5f*cos(utilitaire::DEG_TO_RAD((float)i))),(float)(0.5f+0.5f*sin(utilitaire::DEG_TO_RAD((float)i)))));
    HUDSurfaceGL *cercleDuCentre = new HUDSurfaceGL(GL_TRIANGLE_FAN, listeDesVertexes01,Vecteur4f(1.0f,1.0f,1.0f,1.0f));
    cercleDuCentre->modifierPosition(0.47f,0.0f);
    cercleDuCentre->modifierTaille(0.06f,0.7f);
    cercleDuCentre->assignerTexture(RazerGameUtilities::NOM_DOSSIER_MEDIA +"razer-logo_opaque.png");
    panneauScores->add(cercleDuCentre);

    // Chronom�tre sous le symbole.
    listeDesVertexes01 = new ConteneurVertex2D;
    listeDesVertexes01->push_back(Vecteur2f(0.0f, 0.0f));
    listeDesVertexes01->push_back(Vecteur2f(0.0f, 1.0f));
    listeDesVertexes01->push_back(Vecteur2f(1.0f, 1.0f));
    listeDesVertexes01->push_back(Vecteur2f(1.0f, 0.0f));

    HUDSurfaceGL *panneauTempsJeu = new HUDSurfaceGL(GL_QUADS, listeDesVertexes01,Vecteur4f(0.0f,0.0f,0.0f,1.0f));
    panneauTempsJeu->modifierPosition(0.4f,0.7f);
    panneauTempsJeu->modifierTaille(0.2f,0.3f);
    panneauScores->add(panneauTempsJeu);

    HUDDynamicText* texteTempsJeu = new HUDDynamicText( []( std::stringstream& text ) {
        float tempsJeu = FacadeModele::getInstance()->obtenirPartieCourante()->obtenirTempsJeu();
        int nbHeures = (int)tempsJeu / 3600;
        tempsJeu -= nbHeures * 3600;
        if( nbHeures > 0 )
            text << nbHeures << ":";
        int nbMinutes = (int)tempsJeu / 60;
        if( nbMinutes > 0 )
            text << ( ( nbHeures > 0 && nbMinutes < 10 ) ? "0" : "" ) << nbMinutes << ":";
        tempsJeu -= nbMinutes * 60;
        tempsJeu = (float)( (int)( tempsJeu * 100 ) );
        tempsJeu /= 100.0;
        text << ( ( tempsJeu<10 && nbMinutes > 0 ) ? "0" : "" ) << tempsJeu;
        return Vecteur4f( 1, 1, 1, 1 );
    } );
    texteTempsJeu->modifierPosition(0.4f,1.0f);
    panneauTempsJeu->add(texteTempsJeu);

    racineJeu_->add(panneauScores);
    
    HUDElementComparatif* panneauVainqueur = new HUDElementComparatif([](){
        return FacadeModele::getInstance()->obtenirPartieCourante()->partieTerminee() 
            /*&& !GestionnaireAnimations::obtenirInstance()->estJouerReplay()*/;
    
    }, Vecteur4f(0.0f,0.0f,0.0f,0.8f));
    panneauVainqueur->modifierTaille(0.6f, 0.6f);
    panneauVainqueur->modifierPosition(0.2f, 0.2f);

    HUDTexte* textePartieTerminee = new HUDTexte("Game over! The winner is :", Vecteur4f(1.0f,1.0f,1.0f,1.0f), false);
    textePartieTerminee->modifierPosition(0.1f, 0.33f);
    panneauVainqueur->add(textePartieTerminee);

    HUDDynamicText* texteNomVainqueur = new HUDDynamicText( []( std::stringstream& text ) {
            const Partie* game = FacadeModele::getInstance()->obtenirPartieCourante();
            if( game )
            {
                SPPlayerAbstract player = game->obtenirGagnant();
                if( player )
                {
                    text << player->obtenirNom( );
                }
            }
            return Vecteur4f( 1, 1, 1, 1 );
        }, false );
    texteNomVainqueur->modifierPosition(0.3f, 0.66f);
    panneauVainqueur->add(texteNomVainqueur);

    std::string messageContinuer("Press any key to continue...");
    HUDTexte* espacePourContinuer = new HUDTexte(messageContinuer, Vecteur4f(1.0f,1.0f,1.0f,0.9f), false);
    espacePourContinuer->modifierPosition(0.0f, 0.9f);
    panneauVainqueur->add(espacePourContinuer);
    std::string messageContinuer2("Or press Ctrl + R for a video replay!");
    HUDTexte* espacePourContinuer2 = new HUDTexte(messageContinuer2, Vecteur4f(1.0f,1.0f,1.0f,0.9f), false);
    espacePourContinuer2->modifierPosition(0.0f, 1.0f);
    panneauVainqueur->add(espacePourContinuer2);
    
    racineJeu_->add(panneauVainqueur);

    //panneau pour le replay.
    HUDElementComparatif* panneauReplay = new HUDElementComparatif([]() {return false/*GestionnaireAnimations::obtenirInstance()->estJouerReplay()*/;});


    ConteneurVertex2D* pointsReplay = new ConteneurVertex2D;
    pointsReplay->push_back(Vecteur2f(0.0f,0.0f));
    pointsReplay->push_back(Vecteur2f(0.0f,1.0f));
    pointsReplay->push_back(Vecteur2f(1.0f,1.0f));
    pointsReplay->push_back(Vecteur2f(1.0f,0.0f));
    HUDSurfaceGL* imageReplay=new HUDSurfaceGL(GL_QUADS, pointsReplay, Vecteur4f(1.0f,1.0f,1.0f,1.0f));
    imageReplay->assignerTexture(std::string(RazerGameUtilities::NOM_DOSSIER_MEDIA + "replay_frame.png"));
    panneauReplay->add(imageReplay);
    panneauReplay->modifierPosition(0,0);
    panneauReplay->modifierTaille(1,1);
    imageReplay->modifierPosition(0,0);
    imageReplay->modifierTaille(1,1);
    racineJeu_->add(panneauReplay);

    HUDForeverAlone* wForeverAlone = new HUDForeverAlone;
    wForeverAlone->initSurfaces();
    wForeverAlone->modifierVisibilite(false);
    mForeverAlone = wForeverAlone;
    racineJeu_->add(mForeverAlone);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::dessinerHUDJeu()
///
/// Tra�age du HUD.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::dessinerHUDJeu()
{
    // �tats de la lumi�re et de la profondeur
    GLboolean lighting_state, depth_test_state;
    // D�sactiver le test de profondeur et l'�clairage
    glGetBooleanv(GL_LIGHTING, &lighting_state);
    glGetBooleanv(GL_DEPTH_TEST, &depth_test_state);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-mDeltaXOrtho,1+mDeltaXOrtho,1,0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    racineJeu_->repeindre();
    effectuerDecompte();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // PROJECTION
    glPopAttrib(); // ATTRIBUTS

    // R�activer l'�clairage et le test de profondeur (s'il y a lieu)
    if (lighting_state == GL_TRUE) {
        glEnable(GL_LIGHTING);
    }
    if (depth_test_state == GL_TRUE) {
        glEnable(GL_DEPTH_TEST);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::creerHUDJeu()
///
/// Initialisation du HUD.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::creerHUDTournoi()
{
    racineTournoi_ = new HUDElement();
    // tracer l'arbre des parties.
    const unsigned int nbParticipants = Tournoi::nbrJoueurs_;
    const unsigned int nbParties = Tournoi::nbrParties_;
    const float nbEtages = log((float)nbParticipants)/log(2.0f)+1.0f;
    const float espacement = 7.0f/2462.0f;
    const float hauteurCase = 146.4375f/2462.0f;
    const float largeurCase = 1.0f/5.5f;

    //ajustement de la taille du panneau de base.
    racineTournoi_->modifierTaille(0.8f,0.8f);
    racineTournoi_->modifierPosition(0.1f,0.1f);


    unsigned int indexPartie = nbParties - 1;
    //tracer les cases
    for(unsigned int etage=0; etage<nbEtages; ++etage)
    {
        for(unsigned int j=0; j<nbParticipants/pow(2.0f,(int)etage); ++j)
        {
            ConteneurVertex2D* vertexCases = new ConteneurVertex2D;
            vertexCases->push_back(Vecteur2f(0.0f,0.0f));
            vertexCases->push_back(Vecteur2f(0.0f,1.0f));
            vertexCases->push_back(Vecteur2f(1.0f,1.0f));
            vertexCases->push_back(Vecteur2f(1.0f,0.0f));
            HUDSurfaceGL* caseCourante=new HUDSurfaceGL(GL_QUADS, vertexCases, Vecteur4f(0.8f,0.8f,0.8f,0.9f));
            caseCourante->modifierTaille(largeurCase, hauteurCase);
            caseCourante->modifierPosition((etage)*1.1f*largeurCase, ((j+1.0f)*espacement) + (j*hauteurCase)+ (pow(2.0f, (int)etage)-1.0f)*(j+1.0f/2.0f)*(hauteurCase+espacement));

            const unsigned int indexJoueur = j & 1;
            HUDDynamicText* panneauNomJoueur = new HUDDynamicText( [indexJoueur, indexPartie]( std::stringstream& text ) {

                Vecteur4f color( 1, 1, 1, 1 );
                SPPlayerAbstract joueur;
                if( indexPartie > ( Tournoi::nbrParties_ - 1 ) )
                {
                    unsigned int index = 0;
                    joueur = FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie( index )->obtenirGagnant();
                    color = Vecteur4f( 1, 0, 0, 1 ); //red
                }
                else if( indexJoueur )
                {
                    joueur = FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie( indexPartie )->obtenirJoueurDroit();
                }
                else
                {
                    joueur = FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie( indexPartie )->obtenirJoueurGauche();
                }
                if( joueur )
                {
                    Vainqueurs chemin = FacadeModele::getInstance()->obtenirTournoi()->obtenirCheminDernierVainqueur();
                    if( find( chemin.begin(), chemin.end(), Vainqueur( indexPartie, indexJoueur == 1 ? GAGNANT_DROITE : GAGNANT_GAUCHE ) ) != chemin.end() )
                    {
                        color = Vecteur4f( 0, 0, 0, 1 ); //black
                    }

                    text << joueur->obtenirNom() << std::endl;
                }
                return color;
            } );
            panneauNomJoueur->modifierPosition(0.0f, 1.0f);
            caseCourante->add(panneauNomJoueur);

            racineTournoi_->add(caseCourante);
            if(indexJoueur == 1)
            {
                --indexPartie;
            }
        }
    }
    //////////////////////////////////

    for(int etage=0; etage<nbEtages-1; ++etage)
    {
        for(int j=0; j<(nbParticipants/pow(2.0f,etage)); j+=2)
        {
            //tracer les lignes de separation
            ConteneurVertex2D* pointsLignes = new ConteneurVertex2D;
            pointsLignes->push_back(Vecteur2f(0.0f,0.25f));
            pointsLignes->push_back(Vecteur2f(0.5f,0.25f));

            pointsLignes->push_back(Vecteur2f(0.5f,0.25f));
            pointsLignes->push_back(Vecteur2f(0.5f,0.5f));

            pointsLignes->push_back(Vecteur2f(0.5f,0.5f));
            pointsLignes->push_back(Vecteur2f(1.0f,0.5f));


            pointsLignes->push_back(Vecteur2f(0.5f,0.5f));
            pointsLignes->push_back(Vecteur2f(0.5f,0.75f));


            pointsLignes->push_back(Vecteur2f(0.5f,0.75f));
            pointsLignes->push_back(Vecteur2f(0.0f,0.75f));
            ////////////////////

            HUDSurfaceGL* elementLigne = new HUDSurfaceGL(GL_LINES, pointsLignes, Vecteur4f(1.0f, 0.0f, 0.0f, 0.9f));
            elementLigne->modifierTaille(0.1f*largeurCase,pow(2.0f, etage)*(2.0f*hauteurCase+espacement)/2.0f);
            elementLigne->modifierPosition(largeurCase+(etage)*1.1f*largeurCase, (float)(((j+1.0f)*espacement) + (j*hauteurCase)+ (pow(2.0f, etage)-1.0f)*(j+1.0f/2.0)*(hauteurCase+espacement)+hauteurCase/2.0f));
            racineTournoi_->add(elementLigne);
        }
    }
    /////////////////////////////////

    /////////////////////////////////////////////////
    // appuyez sur une touche pour continuer...    //
    /////////////////////////////////////////////////
    std::string messageContinuer("Press any key to continue...");
    HUDTexte* espacePourContinuer = new HUDTexte(messageContinuer, Vecteur4f(1.0f,1.0f,1.0f,0.9f));
    espacePourContinuer->modifierPosition(0.6f, 0.90f);
    racineTournoi_->add(espacePourContinuer);
    std::string messageContinuer2("Or press Ctrl + R for a video replay!");
    HUDTexte* espacePourContinuer2 = new HUDTexte(messageContinuer2, Vecteur4f(1.0f,1.0f,1.0f,0.9f));
    espacePourContinuer2->modifierPosition(0.6f, 0.95f);
    racineTournoi_->add(espacePourContinuer2);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::dessinerHUDTournoi()
///
/// Tra�age du HUD.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::dessinerHUDTournoi()
{
    drawFromRoot( racineTournoi_ );
}
void GestionnaireHUD::drawFromRoot( HUDElement* root )
{
    if( !root ) return;

    // �tats de la lumi�re et de la profondeur
    GLboolean lighting_state, depth_test_state;
    // D�sactiver le test de profondeur et l'�clairage
    glGetBooleanv( GL_LIGHTING, &lighting_state );
    glGetBooleanv( GL_DEPTH_TEST, &depth_test_state );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0, 1, 1, 0 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glLineWidth( 3 );
    root->repeindre( );
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix(); // MODELVIEW
    glMatrixMode( GL_PROJECTION );
    glPopMatrix(); // PROJECTION
    glPopAttrib(); // ATTRIBUTS

    // R�activer l'�clairage et le test de profondeur (s'il y a lieu)
    if( lighting_state == GL_TRUE )
    {
        glEnable( GL_LIGHTING );
    }
    if( depth_test_state == GL_TRUE )
    {
        glEnable( GL_DEPTH_TEST );
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::effectuerDecompte()
///
/// Effectue le d�compte en d�but de partie.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::effectuerDecompte()
{
    NoeudAffichage* decompte = FacadeModele::getInstance()->obtenirDecompte();
    
    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.0f);
    glScalef(0.005f, -0.005f, 0.001f);
    decompte->render();
    glPopMatrix();

    static int anglePause_ = 0;
    if(listePause_ != 0 && FacadeModele::getInstance()->estEnPause())
    {
        anglePause_ = (anglePause_+5)%360;
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor3f(0.0f, 0.0f, 0.7f);

        glTranslatef(0.8f, 0.8f, 0.0f);
        glScalef(0.002f, -0.002f, 0.0001f);
        glRotatef((GLfloat)anglePause_, 0, 0, 1);

        glCallList(listePause_);

        glPopAttrib();
        glPopMatrix();
    }
    else
        anglePause_ = 0;    
    glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDElement* GestionnaireHUD::obtenirRacine( RacineHUD racine )
///
/// accesseur des differentes racines
///
/// @param[in] RacineHUD racine
///
/// @return HUDElement*
///
////////////////////////////////////////////////////////////////////////
HUDElement* GestionnaireHUD::obtenirRacine( RacineHUD racine )
{
    switch(racine)
    {
    case RACINE_JEU: return racineJeu_;
    //case RACINE_EDITION: return racineEdition_;
    case RACINE_TOURNOI: return racineTournoi_;
    };
    return 0;
}

void GestionnaireHUD::setForeverAloneVisibility( bool pVisible )
{
    if( mForeverAlone )
    {
        mForeverAlone->modifierVisibilite( pVisible );
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



