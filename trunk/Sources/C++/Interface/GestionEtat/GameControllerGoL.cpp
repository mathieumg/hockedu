//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerGoL.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GameControllerGoL.h"
#include "FacadeModele.h"
#include "ConfigScene.h"

#include "Partie.h"
#include "Terrain.h"
#include "GestionnaireHUD.h"
#include "GameControllerAbstract.h"
#include "SourisEtatAbstrait.h"
#include "SoundFMOD.h"
#include "GameManager.h"
#include "HUDMultiligne.h"
#include "HUDDynamicText.h"
#include "GoL/GolPattern.h"
#include "GoL/GolUtils.h"
#include "Vue/VueOrtho.h"

const float lengthDot = 20;
int delai = 100;

////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerGoL::GameControllerGoL( GestionnaireEvenements* contexte )
///
/// Constructeur qui initialise les valeurs requises pour faire fonctionner le gestionnaire. 
/// Place l'état par défaut à Sélection.
///
/// @param[in] GestionnaireEvenements * contexte
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameControllerGoL::GameControllerGoL() :
GameControllerAbstract()
{
    vue::Camera cameraCourante = FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture( xMinCourant, xMaxCourant, yMinCourant, yMaxCourant );


    vue::VueOrtho* nouvelleVue = new vue::VueOrtho(
        cameraCourante,
        0, 300, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/1500000, 1.25,
        -150, 150, -150, 150 );
    nouvelleVue->redimensionnerFenetre( Vecteur2i( xMinCourant, yMinCourant ), Vecteur2i( xMaxCourant, yMaxCourant ) );

    FacadeModele::getInstance()->modifierVue( nouvelleVue );

    mPattern = GolUtils::decodeRLEFile( "..\\media\\GoLPattern\\jslife\\c2-extended\\c2-0006.lif" );
    if( !mPattern )
    {
        mPattern = new GolPattern();
        Vecteur2i p;
        p.X() = 0;
        p.Y() = 0;
#define addPos(x) for(int i=0; i<x; ++i) {p.X()++;mPattern->revive(p);}
        addPos( 8 );
        p.X()++;
        addPos( 5 );
        p.X() += 3;
        addPos( 3 );
        p.X() += 6;
        addPos( 7 );
        p.X() += 1;
        addPos( 5 );
#undef addpos
    }

    mTool = GOL_TOOL_CREATE;
    modifierEtatSouris( ETAT_SOURIS_DEPLACER_FENETRE );

    mHUDRoot = new HUDElement();
    HUDMultiligne* controls = new HUDMultiligne( "Controls (Right click to to tool)\n\
Space : Pause/Unpause game evolution\n\
u : Evolve faster\n\
d : Evolve slower\n\
c : Clear all\n\
e : Eraser Tool\n\
m : Creater Tool", Vecteur4f( 1, 1, 1, 0.7f ) );
    controls->modifierPosition( 0, 0.6f );
    controls->modifierTaille( 1, 0.4f );
    mHUDRoot->add( controls );

    HUDDynamicText* pauseStatus = new HUDDynamicText( [&]( std::stringstream& text ) {
        if( mTimer.isPaused() )
        {
            text << "Paused";
        }
        return Vecteur4f( 1, 1, 1, 0.7f );
    },false );
    pauseStatus->modifierPosition( 0.45f, 0.5f );
    mHUDRoot->add( pauseStatus );

    HUDDynamicText* fpsStatus = new HUDDynamicText( [&]( std::stringstream& text ) {
        if( delai > 16 )
        {
            const float fps = 1 / ( delai/1000.f );
            text << "Speed: " << fps << "fps";
        }
        else
        {
            text << "Maximum Speed";
        }
        return Vecteur4f( 1, 1, 1, 0.7f );
    }, false );
    fpsStatus->modifierPosition( 0, 0.05f );
    mHUDRoot->add( fpsStatus );


    HUDDynamicText* toolText = new HUDDynamicText( [&]( std::stringstream& text ) {
        text << "Right Click button : ";
        switch( mTool )
        {
        case GOL_TOOL_CREATE:
            text << "Create new dots";
            break;
        case GOL_TOOL_ERASE:
            text << "Erase dots";
            break;
        default:
            break;
        }
        return Vecteur4f( 1, 1, 1, 0.7f );
    } );
    toolText->modifierPosition( 0.7f, 0.05f );
    mHUDRoot->add( toolText );

    mHUDRoot->modifierTaille( 1, 1 );
    mHUDRoot->modifierPosition( 0, 0 );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerGoL::~GameControllerGoL()
///
/// Destructeur virtuel
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
GameControllerGoL::~GameControllerGoL( )
{
    delete mPattern;
    delete mHUDRoot;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerGoL::toucheEnfoncee(EvenementClavier& evenementClavier)
{
	ToucheClavier touche = evenementClavier.obtenirTouche();
    switch( touche  )
    {
    case VJAK_SPACE:
        mTimer.togglePause();
        break;
    case VJAK_C:
        mPattern->clear();
        break;
    case VJAK_M:
        mTool = GOL_TOOL_CREATE;
        break;
    case VJAK_E:
        mTool = GOL_TOOL_ERASE;
        break;
    default:
        break;
    }
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerGoL::toucheRelachee( EvenementClavier& evenementClavier )
{
	ToucheClavier touche = evenementClavier.obtenirTouche();
}


bool enfonce = false;
Vecteur2i posEnfonce;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerGoL::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisEnfoncee(evenementSouris);
    }
    if( evenementSouris.obtenirBouton() == BOUTON_SOURIS_DROIT )
    {
        enfonce = true;
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle( evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], position );
        position /= lengthDot;

        Vecteur2i pos( (int)position.X(), (int)position.Y() );
        if( mTool == GOL_TOOL_ERASE)
        {
            mPattern->kill( pos );
        }
        else if(mTool == GOL_TOOL_CREATE )
        {
            mPattern->revive( pos );
        }
        posEnfonce = pos;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerGoL::sourisRelachee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisRelachee(evenementSouris);
    }
    if( evenementSouris.obtenirBouton() == BOUTON_SOURIS_DROIT )
    {
        enfonce = false;
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::sourisDeplacee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerGoL::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisDeplacee(evenementSouris);
    }

    if( enfonce )
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle( evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], position );
        position /= lengthDot;

        Vecteur2i pos( (int)position.X(), (int)position.Y() );
        if( pos != posEnfonce )
        {
            if( mTool == GOL_TOOL_ERASE )
            {
                mPattern->kill( pos );
            }
            else if( mTool == GOL_TOOL_CREATE )
            {
                mPattern->revive( pos );
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris)
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerGoL::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	GameControllerAbstract::rouletteSouris(evenementRouletteSouris);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::animer( const float& temps )
///
/// Animation du mode jeu.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerGoL::animer( const float& temps )
{
    static int inc = 0;
    if( EventManager::IsKeyPressed( VJAK_U ) )
    {
        inc++;
        if( delai > 5 )
        {
            delai--;
            if( inc > 60 )
            {
                delai -= 5;
            }
        }
    }
    else if( EventManager::IsKeyPressed( VJAK_D ) )
    {
        inc++;
        delai++;
        if( inc > 60 )
        {
            delai += 5;
        }
    }
    else
        inc = 0;

	SoundFMOD::obtenirInstance()->change_song_if_end();	

    const float elapsed = mTimer.Elapsed_Time_ms();
    if( !mTimer.isPaused( ) && elapsed >= delai )
    {
        mTimer.reset_Time();
        mTimer.adjustTime( elapsed - delai );
        mPattern->evolve();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerGoL::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerGoL::afficher()
{

    auto vue = FacadeModele::getInstance()->obtenirVue();
    if( vue )
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        const int nbViewPort = vue->obtenirNbViewports();
        for( int currentCam = 1; currentCam <= nbViewPort; currentCam++ )
        {
            glEnable( GL_LIGHTING );
            vue->appliquerVue( currentCam );

            // On rafraichi la lumière
            ConfigScene::obtenirInstance()->rafraichirLumiere();

            glPushMatrix();
            glPushAttrib( GL_ALL_ATTRIB_BITS );

            glDisable( GL_LIGHTING );
            glEnable( GL_POINT_SMOOTH );
            glEnable( GL_LINE_SMOOTH );
            glDisable( GL_BLEND );
            glColor3f( 1, 1, 1 );
            //glPointSize( 6 );
            glBegin( GL_QUADS );
            const Cells& alives = mPattern->getAliveCells();
            for( auto it = alives.cbegin(); it != alives.cend(); ++it )
            {
                const float x = it->first[VX] * lengthDot, y = it->first[VY] * lengthDot;
                glVertex2f( x, y );
                glVertex2f( x + lengthDot, y );
                glVertex2f( x + lengthDot, y + lengthDot );
                glVertex2f( x, y + lengthDot );
            }
            glEnd();
            glEnable( GL_BLEND );
            glPopAttrib();
            glPopMatrix();

            GestionnaireHUD::obtenirInstance()->drawFromRoot( mHUDRoot );

        }
    }    
}

void GameControllerGoL::miseAJourEvenementsRepetitifs( float deltaTemps )
{
	GameControllerAbstract::CameraMovementFromKeyPressed(deltaTemps);
}

void GameControllerGoL::loadPattern( char* path )
{
    GolPattern* pattern = GolUtils::decodeRLEFile( path );
    if( pattern )
    {
        delete mPattern;
        mPattern = pattern;
    }
}









///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




