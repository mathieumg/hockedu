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
#include "FacadeCSharp.h"
#include "GameManager.h"
#include "HUDMultiligne.h"
#include "HUDDynamicText.h"


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
    mTool = GOL_TOOL_CREATE;
    modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
    Vecteur2 p;
    p.X() = 0;
    p.Y() = 0;
#define addPos(x) for(int i=0; i<x; ++i) {p.X()++;mAlives[p] = 0;}
    addPos( 8 );
    p.X()++;
    addPos( 5 );
    p.X( )+=3;
    addPos( 3 );
    p.X( ) += 6;
    addPos( 7 );
    p.X() += 1;
    addPos( 5 );
#undef addpos

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
        mAlives.clear();
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
        auto it = mAlives.find( pos );
        if( mTool == GOL_TOOL_ERASE && it != mAlives.end() )
        {
            mAlives.erase( it );
        }
        else if(mTool == GOL_TOOL_CREATE )
        {
            mAlives[pos] = 0;
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
            auto it = mAlives.find( pos );
            if( mTool == GOL_TOOL_ERASE && it != mAlives.end() )
            {
                mAlives.erase( it );
            }
            else if( mTool == GOL_TOOL_CREATE )
            {
                mAlives[pos] = 0;
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


class CoordContour : public Vecteur2i
{
public:
    CoordContour( Vecteur2i pos ) :Vecteur2i( pos.X() - 1, pos.Y() ), turn_( 1 )
    {
    }

    CoordContour operator++( int )
    {
        switch( turn_ )
        {
        case 0: vect[VX]--; break;
        case 1: vect[VY]--; break;		//	   >>>
        case 2: vect[VX]++; break;		//	 ^ XXX v
        case 3: vect[VX]++; break;		//	 <-X0X v
        case 4: vect[VY]++; break;		//	 ^ XXX v
        case 5: vect[VY]++; break;		//	   <<< 
        case 6: vect[VX]--; break;
        case 7: vect[VX]--; break;
        default: break;
        }
        turn_++;

        return *this;
    }

    bool ContourTerminer()
    {
        return turn_ > 8;
    }
private:
    int turn_;

};

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

    if( !mTimer.isPaused( ) && mTimer.Elapsed_Time_ms( ) > delai )
    {
        mTimer.reset_Time();
        // Update adjacence
        for( Cells::iterator it = mAlives.begin(); it != mAlives.end(); ++it )
        {
            for( CoordContour c( it->first ); !c.ContourTerminer(); c++ )
            {
                if( mAlives.find( c ) != mAlives.end() )
                {
                    ++it->second;
                }
                else
                {
                    // update maybes
                    Cells::iterator it2 = mMaybes.find( c );
                    if( it2 == mMaybes.end() )
                    {
                        mMaybes[c] = 1;
                    }
                    else
                    {
                        ++it2->second;
                    }
                }
            }
        }

        // kill cells
        for( Cells::iterator it = mAlives.begin(); it != mAlives.end(); /* no increment */ )
        {
            const int adj = it->second;
            if( (unsigned int)( adj - 2 ) > 1 ) // only if adj is 2 or 3
            {
                mAlives.erase( it++ );
            }
            else
            {
                // reset
                it->second = 0;
                ++it;
            }
        }

        // resume life
        for( Cells::const_iterator it = mMaybes.begin(); it != mMaybes.end(); ++it )
        {
            const int adj = it->second;
            if( adj == 3 ) // only if adj is 3
            {
                mAlives[it->first] = 0;
            }
        }
        mMaybes.clear();
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
            for( Cells::iterator it = mAlives.begin(); it != mAlives.end(); ++it )
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









///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




