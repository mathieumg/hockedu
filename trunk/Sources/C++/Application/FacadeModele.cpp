///////////////////////////////////////////////////////////////////////////////
/// @file FacadeModele.cpp
/// @author Martin Bisson
/// @date 2007-05-22
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////


// Commentaire Doxygen mis sur la premi�re page de la documentation Doxygen.
/**

@mainpage Projet int�grateur de troisi�me ann�e -- LOG3900

@li <a href="../javadoc/index.html">Documentation Javadoc de la partie Java.</a>
*/
#define _WINSOCKAPI_
#include <windows.h>
#include <cassert>

#include "FreeImage.h"

#include "VueOrtho.h"

#include "Camera.h"
#include "Projection.h"
#include "LignePointillee.h"
#include "RazerGameUtilities.h"

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "AideGL.h"
#include "CompteurAffichage.h"
#include "ConfigScene.h"

#include "FacadeModele.h"

#include "CompteurAffichage.h"
#include "GestionnaireEvenements.h"
#include "SourisEtatDeplacerFenetre.h"
#include "VisiteurCollision.h"
#include "VisiteurSuppression.h"
#include "VisiteurDeplacement.h"
#include "SoundFMOD.h"
#include <algorithm>
#include "DecodeString.h"
#include "JoueurVirtuel.h"
#include "Tournoi.h"
#include "GestionnaireEtatModeJeu.h"
#include "NoeudTable.h"
#include "NoeudAccelerateur.h"
#include "NoeudMaillet.h"
#include "GestionnaireModeles.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudMuret.h"
#include "VisiteurEstSelectione.h"
#include "NoeudBut.h"
#include "NoeudRondelle.h"
#include "Partie.h"
#include "JoueurHumain.h"
#include <wtypes.h>
#include "ArbreNoeudLibre.h"
#include "GestionnaireAnimations.h"
#include "AnimationReprise.h"
#include "VuePerspectiveLibre.h"
#include "VuePerspectiveCiel.h"
#include "VuePerspectiveOrbit.h"
#include "Terrain.h"
#include "HUD\GestionnaireHUD.h"
#include "textfile.h"
#include "VuePerspectiveSplit.h"
#include "ProjectionPerspective.h"
#include "BoiteEnvironnement.h"
#include <iostream>
#include "DebugRenderBox2D.h"
#include "HUDTexte.h"
#include "VisiteurFunction.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "ExceptionJeu.h"

/// Pointeur vers l'instance unique de la classe.
FacadeModele* FacadeModele::instance_ = 0;

/// Cha�ne indiquant le nom du fichier de configuration du projet.
const std::string FacadeModele::FICHIER_TERRAIN_EN_COURS = "MapEnCours.xml";
const std::string FacadeModele::FICHIER_JOUEURS = "joueurs.xml";

int FacadeModele::anglePause_ = 0;
HUDTexte* debugInfo;

//DebugDraw __debugDraw;
HANDLE mutexRender;
HANDLE mutexNoeuds;
bool bRendering = true;
RenderWorkerData renderWorkerData;

//la racine de l'arbre HUD.
#define LARGEUR_FENETRE_OPENGL 10
#define HAUTEUR_FENETRE_OPENGL 10
#define GLSHADERS 1


DWORD WINAPI RenderSceneWorker(LPVOID arg)
{
    RenderWorkerData* data = (RenderWorkerData*)(arg);

    bool returnValue = true;

    if(!wglMakeCurrent(FacadeModele::getInstance()->obtenirHDC(),data->glrc))
    {
        returnValue = false;
    }
    ReleaseMutex(mutexRender);

    FacadeModele::getInstance()->InitOpenGLContext();

    while(bRendering & returnValue)
    {
        if( WaitForSingleObject(mutexRender, INFINITE) == WAIT_ABANDONED )
        {
            returnValue = false;
        }
        if( WaitForSingleObject(mutexNoeuds, INFINITE) == WAIT_ABANDONED )
        {
            returnValue = false;
        }
        FacadeModele::getInstance()->afficher();
        ReleaseMutex(mutexNoeuds);
    }

    wglDeleteContext(data->glrc);

    return returnValue;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::changerFond(int r, int g, int b, float a)
///
/// Change la couleur du fond, utilis�e lors de nos premiers tests.
///
/// @param[in] r : Quantit� de rouge.
/// @param[in] g : Quantit� de vert.
/// @param[in] b : Quantit� de bleu.
/// @param[in] a : Transparence.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::changerFond(int r, int g, int b, float a)
{
    glClearColor((float)(r/255.0), (float)(g/255.0), (float)(b/255.0), a);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Tournoi* FacadeModele::obtenirTournoi()
///
/// Pour obtenir le tournoi en cours.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
Tournoi* FacadeModele::obtenirTournoi()
{
    return tournoi_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeModele* FacadeModele::getInstance()
///
/// Cette fonction retourne un pointeur vers l'instance unique de la
/// classe.  Si cette instance n'a pas �t� cr��e, elle la cr�e.  Cette
/// cr�ation n'est toutefois pas n�cessairement "thread-safe", car
/// aucun verrou n'est pris entre le test pour savoir si l'instance
/// existe et le moment de sa cr�ation.
///
/// @return Un pointeur vers l'instance unique de cette classe.
///
////////////////////////////////////////////////////////////////////////
FacadeModele* FacadeModele::getInstance()
{
   if (instance_ == 0)
      instance_ = new FacadeModele;

   return instance_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::libererInstance()
///
/// Cette fonction lib�re l'instance unique de cette classe.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::libererInstance()
{
    ConfigScene::obtenirInstance()->enregistrerConfiguration();
    // On lib�re les instances des diff�rentes configurations.
    SoundFMOD::libererInstance();
    ConfigScene::libererInstance();
    GestionnaireHUD::libererInstance();
    instance_->libererMemoire();
    delete instance_;
    instance_ = 0;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeModele::FacadeModele()
///
/// Ce constructeur par d�faut ne fait qu'initialiser toutes les
/// variables � une valeur nulle.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
FacadeModele::FacadeModele()
    : hGLRC_(0), hDC_(0), hWnd_(0), vue_(0),zoomElastique_(false),tournoi_(0),cheminTournoi_(""),enJeu_(false),
    partieCourante_(0), /*adversaire_(0),*/ mEditionField(0), mUpdating(false), mRendering(false)
{
    // Il ne faut pas faire d'initialisation de Noeud ici, car le contexte OpenGl n'est pas encore creer

    // Initialisation de la randomisation
    srand( (unsigned int) clock()+(unsigned int) time);

    mEditionField = new Terrain(NULL);

#ifdef WIN32
    CreateDirectoryA(
        "tournoi",
        NULL
        );
#else
    // creation d'un dossier avec un autre API, dunno which yet
#endif


}



////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeModele::~FacadeModele()
///
/// Ce destructeur lib�re les objets du mod�le.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
FacadeModele::~FacadeModele()
{
    if(vue_)
        delete vue_;
    if(mEditionField)
        delete mEditionField;
    if(boiteEnvironnement!=0)
    {
        delete boiteEnvironnement;
        boiteEnvironnement=0;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::InitOpenGLContext()
///
/// Initialise le context OpenGl courant
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::InitOpenGLContext()
{
    glewInit();

    // mod�le de rendu
    glShadeModel( GL_SMOOTH );

    // Initialisation des shaders
#if GLSHADERS
    initialiserNuanceurs();
#endif
    // couleur de l'arri�re-plan
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // activer les etats openGL
    glEnable( GL_NORMALIZE );
    glEnable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );

    // Le cull face
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
    //glEnable( GL_VERTEX_PROGRAM_TWO_SIDE );                // two-side mode en GLSL
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::initialiserOpenGL(HWND hWnd)
///
/// Cette fonction permet d'initialiser le contexte OpenGL.  Elle cr�e
/// un contexte OpenGL sur la fen�tre pass�e en param�tre, initialise
/// FreeImage (utilis�e par le chargeur 3d) et assigne des param�tres
/// du contexte OpenGL.
///
/// @param[in] hWnd : La poign�e ("handle") vers la fen�tre � utiliser.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::initialiserOpenGL(HWND hWnd)
{
    hWnd_ = hWnd;
    bool succes = aidegl::creerContexteGL(hWnd_, hDC_, hGLRC_);
    checkf(succes);

    InitOpenGLContext();

    // FreeImage, utilis�e par le chargeur, doit �tre initialis�e
    FreeImage_Initialise();

    // Initialisation des mod�les
    GestionnaireModeles::obtenirInstance()->initialiser();
    ConfigScene::obtenirInstance();
    SoundFMOD::obtenirInstance();
    GestionnaireHUD::obtenirInstance();

#if !SHIPPING
    debugInfo = new HUDTexte("",Vecteur4f(0,1,1,1));
    debugInfo->modifierVisibilite(true);
    debugInfo->modifierPosition(0.15f,0.15f);
    GestionnaireHUD::obtenirInstance()->obtenirRacine(RACINE_JEU)->add(debugInfo);
#endif

    // On cr�e une vue par d�faut.
    initialiserVue();

    // Initialisation du skybox
    const std::string xpos = RazerGameUtilities::NOM_DOSSIER_MEDIA + "sb2_xpos.png";
    const std::string xneg = RazerGameUtilities::NOM_DOSSIER_MEDIA + "sb2_xneg.png";
    const std::string ypos = RazerGameUtilities::NOM_DOSSIER_MEDIA + "sb2_ypos.png";
    const std::string yneg = RazerGameUtilities::NOM_DOSSIER_MEDIA + "sb2_yneg.png";
    const std::string zpos = RazerGameUtilities::NOM_DOSSIER_MEDIA + "sb2_zpos.png";
    const std::string zneg = RazerGameUtilities::NOM_DOSSIER_MEDIA + "sb2_zneg.png";
    boiteEnvironnement= new utilitaire::BoiteEnvironnement(xpos,xneg,ypos,yneg,zpos,zneg);

    SoundFMOD::obtenirInstance()->playApplicationSong(STARTUP_SONG);
    chargerJoueurs();

    renderThread_ = NULL;
    
//     mutexRender = CreateMutex(
//         NULL, // default security attributes
//         TRUE, // initially owned
//         NULL); // unnamed mutex
//     mutexNoeuds = CreateMutex(
//         NULL, // default security attributes
//         FALSE, // initially not owned
//         NULL); // unnamed mutex
// 
//     bRendering = true;
//     //RenderWorkerData* data = (RenderWorkerData*)malloc(sizeof(RenderWorkerData));
//     renderWorkerData.terrain = &mFieldName;
//     renderWorkerData.enjeu = &enJeu_;
//     renderWorkerData.dc = hDC_;
//     renderWorkerData.glrc = wglCreateContext(hDC_);
// 
//     if(renderWorkerData.glrc)
//     {
//         if(wglShareLists(hGLRC_,renderWorkerData.glrc))
//         {
//             renderThread_ = CreateThread(NULL, 0, RenderSceneWorker, &renderWorkerData,NULL,NULL);
//         }
// 
//     }
//     if(!renderWorkerData.glrc || !renderThread_)
//     {
//         if(renderWorkerData.glrc)
//         {
//             wglDeleteContext(renderWorkerData.glrc);
//         }
//         //free(data);
//         CloseHandle(mutexRender);
//     }
//     else
//     {
//         WaitForSingleObject(mutexRender, INFINITE);
//     }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::chargerJoueurs( const std::string& nomFichier /*= ""*/, ConteneurJoueur* joueurs /*= 0*/ )
///
/// Charge les joueurs � partir d'un fichier XML.
///
/// @param[in] const std::string & nomFichier : nom du fichier ou enregistrer
/// @param[in] ConteneurJoueur * joueurs : conteneur de joueurs
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::chargerJoueurs( const std::string& nomFichier /*= ""*/, ConteneurJoueur* joueurs /*= 0*/ )
{
    std::string fichierACharger;

    // Si le nom du fichier est vide, on utilise celui par d�faut.
    if(nomFichier == "")
        fichierACharger = FICHIER_JOUEURS;
    else
        fichierACharger = nomFichier;

    if(joueurs == 0)
        joueurs = &profilsVirtuels_;

    if(!utilitaire::fichierExiste(fichierACharger))
    {
        enregistrerJoueurs();
        return;
    }
    XmlDocument* document = XMLUtils::LoadDocument(fichierACharger.c_str());
    if( !document )
    {
        utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier");
        return;
    }

    // On enregistre les diff�rentes configurations.
    ConfigScene::obtenirInstance()->lireDOM((XmlNode&)*document,*joueurs);

    XMLUtils::FreeDocument(document);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::enregistrerJoueurs( const std::string& nomFichier /*= ""*/, ConteneurJoueur* joueurs /*= 0*/ ) const
///
/// Enregistre les joueurs dans un fichier XML.
///
/// @param[in] const std::string & nomFichier : nom du fichier ou enregistrer
/// @param[in] ConteneurJoueur * joueurs : conteneur de joueurs
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::enregistrerJoueurs( const std::string& nomFichier /*= ""*/, ConteneurJoueur* joueurs /*= 0*/ )
{
    std::string fichierAEnregistrer;

    // Si le nom du fichier est vide, on utilise celui par d�faut.
    if(nomFichier == "")
        fichierAEnregistrer = FICHIER_JOUEURS;
    else
        fichierAEnregistrer = nomFichier;

    if(joueurs == 0)
        joueurs = &profilsVirtuels_;

    XmlDocument* document = XMLUtils::CreateDocument("1.0", "", "");

    // Creation du noeud du terrain
    ConfigScene::obtenirInstance()->creerDOM((XmlNode&)*document,*joueurs);

    // �crire dans le fichier
    XMLUtils::SaveDocument(document,fichierAEnregistrer.c_str());
    XMLUtils::FreeDocument(document);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::chargerTournoi( std::string nomFichier )
///
/// Chargement d'un tournoi a partir d'un fichier XML
///
/// @param[in] std::string nomFichier : nom du fichier a charger
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::chargerTournoi(std::string nomFichier)
{
    if(utilitaire::fichierExiste(nomFichier))
    {
        XmlDocument* document = XMLUtils::LoadDocument(nomFichier.c_str());
        if( !document )
        {
            utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier de tournoi");
            return;
        }
        Tournoi* tournoi = new Tournoi();
        
        XmlElement* elem = XMLUtils::FirstChildElement((XmlElement*)document,"Tournoi");
        if(elem && tournoi->initialisationXML(elem,&profilsVirtuels_))
        {
            if(tournoi_ != 0)
                delete tournoi_;
            tournoi_ = tournoi;
            cheminTournoi_ = nomFichier;
        }
        else
        {
            delete tournoi;
            tournoi_ = 0;
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::enregistrerTournoi( Tournoi* tournoi )
///
/// Enregistre un tournoi dans un fichier XML.
///
/// @param[in] Tournoi * tournoi : le tournoi � enregistrer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::enregistrerTournoi( Tournoi* tournoi )
{
    if(tournoi == 0)
        tournoi = tournoi_;

    TiXmlDocument document;
    // �crire la d�claration XML standard...
    TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
    document.LinkEndChild(declaration);
    // On enregistre les diff�rentes configurations.
    document.LinkEndChild(tournoi->creerTournoiXML());
    // �crire dans le fichier
    document.SaveFile( (("tournoi/" + tournoi->obtenirNom()) + ".xml").c_str() );
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::libererOpenGL()
///
/// Cette fonction lib�re le contexte OpenGL et d�sinitialise FreeImage.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::libererOpenGL()
{
    utilitaire::CompteurAffichage::libererInstance();
    
    bRendering = false;
    if(renderThread_)
    {
        WaitForSingleObject(renderThread_,1000);
        CloseHandle(renderThread_);
        renderThread_ = NULL;
        CloseHandle(mutexRender);
    }

    bool succes = aidegl::detruireContexteGL(hWnd_, hDC_, hGLRC_);
    assert(succes);

    FreeImage_DeInitialise();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::libererMemoire(  )
///
/// Liberation de la m�moire
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::libererMemoire()
{
    // Pointeur Intelligent, on ne libere plus leur memoire
    //for_each(joueurs_.begin(),joueurs_.end(),utilitaire::LibererMappe());
    if(tournoi_ != 0)
        delete tournoi_;
    if(partieCourante_ && !partieCourante_->faitPartieDunTournoi())
        delete partieCourante_; 

#if BOX2D_INTEGRATED  
    if(DebugRenderBox2D::mInstance)delete DebugRenderBox2D::mInstance;
    DebugRenderBox2D::mInstance = NULL;
#endif
    if(GestionnaireEvenements::etatCourant_) delete GestionnaireEvenements::etatCourant_;
    GestionnaireEvenements::etatCourant_ = NULL;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::SignalRender()
///
/// Indique qu'il est temps de faire un rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::SignalRender()
{
    if(renderThread_)
    {
        if( WaitForSingleObject(renderThread_,0) == WAIT_OBJECT_0)
        {
            // thread termin� trop tot
            renderThread_ = NULL;
            SignalRender();
        }
        else
        {
            ReleaseMutex(mutexRender);
        }
    }
    else
    {
        afficher();
    }
}

void FacadeModele::ActivateShaders()
{
#if GLSHADERS
    glUseProgram( progPhong_ );
    //glActiveTexture( GL_TEXTURE0 );
    //Vecteur3f positionCamera = vue_->obtenirCamera(currentCam).obtenirPosition();
    //glUniform3fARB( glGetUniformLocationARB( progPhong_, "eyeVec" ), positionCamera[VX], positionCamera[VY], positionCamera[VZ] );
    //glUniform1i( glGetUniformLocation( progPhong_, "colorMap" ), 0 );
#endif
}

void FacadeModele::DeActivateShaders()
{
#if GLSHADERS
    glUseProgram(0);
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::afficher() const
///
/// Cette fonction affiche le contenu de la sc�ne.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::afficher( )
{
    utilitaire::CompteurAffichage::obtenirInstance()->signalerAffichage();
    mRendering = true;
 
    // Affichage specifique aux etats
    GestionnaireEvenements::afficher();
    DrawSelectionRectangle();

    for(int i=(int)mUIRunnables.size()-1; i>=0; --i)
    {
        Runnable* pRun = mUIRunnables.front();
        mUIRunnables.pop();
        pRun->Run();
        if(pRun->KeepAlive())
        {
            mUIRunnables.push(pRun);
        }
        else
        {
            delete pRun;
        }
    }
    
    // Compte de l'affichage
    utilitaire::CompteurAffichage::obtenirInstance()->signalerAffichage();

    // �change les tampons pour que le r�sultat du rendu soit visible.
    ::SwapBuffers( hDC_ );
    mRendering = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::afficherBase() const
///
/// Cette fonction affiche la base du contenu de la sc�ne, c'est-�-dire
/// qu'elle met en place l'�clairage et affiche les objets.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::afficherBase() const
{
    if(partieCourante_)
    {
        partieCourante_->afficher();
    }
    // Afficher la sc�ne
    else if(getEditionField())
    {
        getEditionField()->renderField();
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::rafraichirFenetre() const
///
/// Cette fonction rafra�chit le contenu de la fen�tre, c'est-�-dire
/// qu'elle indique � Windows que la fen�tre doit �tre repeinte.  Un
/// �v�nement sera donc g�n�r� puis trait� plus tard, ce qui �vite de
/// faire plusieurs affichages sans raison.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::rafraichirFenetre() const
{
    // Indique � Windows que la fen�tre doit �tre rafra�chie.
    //::InvalidateRect(hWnd_, 0, FALSE);

    // Finalement, on choisit de simplement appeler afficher() pour �viter
    // que le fond ne soit redessin� par Windows ou Java.

    // Or, comme il y a en permanence une boucle qui r�affiche, on ne fait
    // rien.

    //afficher();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::reinitialiserTerrain()
///
/// Cette fonction r�initialise la sc�ne � un �tat "vide".
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::reinitialiserTerrain()
{
    getEditionField()->reinitialiser();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::animer( const float& temps)
///
/// Cette fonction effectue les diff�rents calculs d'animations
/// n�cessaires pour le mode jeu, tel que les diff�rents calculs de
/// physique du jeu.
///
/// @param[in] temps : Intervalle de temps en sec sur lequel effectuer le calcul.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::animer( const float& temps)
{
    mUpdating = true;
    
    float tempsReel = temps;

    for(int i=(int)mUpdateRunnables.size()-1; i>=0; --i)
    {
        Runnable* pRun = mUpdateRunnables.front();
        mUpdateRunnables.pop();
        pRun->Run();
        if(pRun->KeepAlive())
        {
            mUpdateRunnables.push(pRun);
        }
        else
        {
            delete pRun;
        }
    }

    GestionnaireEvenements::miseAJourEvenementsRepetitifs(tempsReel);
    GestionnaireEvenements::animer(tempsReel);
    bool replaying = false;
    if(GestionnaireAnimations::obtenirInstance()->estJouerReplay())
        replaying = true;

    GestionnaireAnimations::obtenirInstance()->animer(tempsReel*1000);// Prends le temps en ms

    if(replaying && obtenirPartieCourante() && !obtenirPartieCourante()->partieTerminee() && !GestionnaireAnimations::obtenirInstance()->estJouerReplay())
    {
        obtenirPartieCourante()->obtenirGameTime()->unPause();
    }

    mUpdating = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::deplacerSouris(Vecteur2i deplacement)
///
/// Fonction qui d�place la vue.
///
/// @param[in] deplacement : le vecteur de d�placement de la cam�ra.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::deplacerSouris( Vecteur2i deplacement )
{

    obtenirVue()->deplacerXYSouris(deplacement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::deplacerFleches(Vecteur2i deplacement)
///
/// Fonction qui d�place la vue.
///
/// @param[in] deplacement : le vecteur de d�placement de la cam�ra.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::deplacerFleches( Vecteur2i deplacement )
{
    obtenirVue()->deplacerXYFleches(deplacement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::orbit( Vecteur2i deplacement )
///
/// Fonction qui effecte un orbit de la vue.
///
/// @param[in] deplacement : le vecteur de d�placement de la cam�ra.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::orbit( Vecteur2i deplacement )
{
    obtenirVue()->rotaterXY(deplacement);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::zoom(int nbCoches)
///
/// Fonction qui d�place la vue.
///
/// @param[in] nbCoche : Le nombre de coches parcourus par la roulette
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::zoom(int nbCoches)
{
    // Si le nombre de coche est n�gatif, c'est qu'on roule vers l'avant.
    if(nbCoches < 0)
        obtenirVue() -> zoomerIn();
    else if(nbCoches > 0)
        obtenirVue() -> zoomerOut();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::zoomDoux(int nbCoches)
///
/// Zoom doux pour le zoom proportionnel a la souris
///
/// @param[in] nbCoche : Le nombre de mouvements parcourus par la souris
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::zoomDoux(int nbCoches)
{
    // Si le nombre de coche est n�gatif, c'est qu'on roule vers l'avant.
    if(nbCoches > 0)
        for(int i=0; i<nbCoches; i++)
            obtenirVue() -> zoomerInSmooth();
    else if(nbCoches < 0)
        for(int i=0; i>nbCoches; i--)
            obtenirVue() -> zoomerOutSmooth();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::modifierVariableZoomElastique( bool actif, Vecteur2i coin1, Vecteur2i coin2 )
///
/// S'occupe d'afficher le rectangle �lastique.
///
/// @param[in] bool actif : Indique si on active le zoom �lastique
/// @param[in] Vecteur2i coin1 : Le coin en haut � gauche
/// @param[in] Vecteur2i coin2 : Le coin en bas � droite
///
/// @return Aucune
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::modifierVariableZoomElastique(bool actif, Vecteur2i coin1, Vecteur2i coin2)
{
    coinElastique1_ = coin1;
    coinElastique2_ = coin2;
    zoomElastique_ = actif;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::passageModeEdition()
///
/// Passage au mode �dition.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::passageModeEdition()
{
    // Indique que nous ne somme plus en train de jouer le tournoi
    if(tournoi_)
        tournoi_->modifierEstEnCours(false);
    if(partieCourante_)
    {
        if(!partieCourante_->faitPartieDunTournoi())
        {
            delete partieCourante_;
        }
    }
    partieCourante_ = 0;

    enJeu_ = false;
    RazerGameUtilities::LoadFieldFromFile(FICHIER_TERRAIN_EN_COURS,*mEditionField);
    getEditionField()->setTableControlPointVisible(true);

    return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::passageModeTournoi(  )
///
/// Permet de passer 
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::passageModeTournoi()
{
#ifdef WIN32
    static auto loadingCursor = LoadCursor(NULL,IDC_WAIT);
    auto NormalCursor = GetCursor();
    SetCursor(loadingCursor);
#endif

    // Attend que tous les modeles soit charger
    while(GestionnaireModeles::obtenirInstance()->isStillLoadingModel()){}

#ifdef WIN32
    SetCursor(NormalCursor);
#endif


    if(!tournoi_)
    {
        utilitaire::afficherErreur("Le modele ne contient pas de tournoi a jouer");
        return false;
    }
    if(partieCourante_)
    {
        if(!partieCourante_->faitPartieDunTournoi())
        {
            delete partieCourante_;
        }
        else
        {
            checkf(0, "TODO::Enlever la partie de la hash_map de partie courantes");
        }
    }

    GestionnaireAnimations::obtenirInstance()->viderBufferReplay();
    partieCourante_ = tournoi_->obtenirPartieCourante();
    partieCourante_->getReadyToPlay();

    partieCourante_->miseAuJeu(true);
    enJeu_ = true;

    obtenirVue()->centrerCamera(partieCourante_->getField()->GetTableWidth());
    
    tournoi_->modifierEstEnCours(true);
#ifdef WIN32
    // If the no button was pressed ...
    if (partieCourante_->partieVirtuelle() && MessageBoxA(0,
        "Voulez-vous observer la partie entre les 2 joueurs virtuels?", 
        "Simulation de partie", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL) == IDNO )
    {
        partieCourante_->terminerSi2AI();
    }
#else
    partieCourante_->terminerSi2AI();
#endif
    
    return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::passageModeJeu()
///
/// Passage au mode jeu.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::passageModeJeu()
{
    // Le java fait la demande pour valider la map pour que ce soit fait avant de demander contre kel joueur jouer
//  if(!verifierValiditeMap())
//  {
//      return false;
//  }

#ifdef WIN32
    static auto loadingCursor = LoadCursor(NULL,IDC_WAIT);
    auto NormalCursor = GetCursor();
    SetCursor(loadingCursor);
#endif

    // Attend que tous les modeles soit charger
    while(GestionnaireModeles::obtenirInstance()->isStillLoadingModel()){}

#ifdef WIN32
    SetCursor(NormalCursor);
#endif

    // Indique que nous ne somme plus en train de jouer le tournoi
    if(tournoi_)
        tournoi_->modifierEstEnCours(false);

    GestionnaireAnimations::obtenirInstance()->viderBufferReplay();
    if(!adversaire_)
        adversaire_ = SPJoueurAbstrait(new JoueurHumain("Joueur Droit"));
    partieCourante_ = new Partie(SPJoueurAbstrait(new JoueurHumain("Joueur Gauche")),adversaire_);
    //partieCourante_ = new Partie(new JoueurVirtuel("Joueur Gauche",225),new JoueurVirtuel("Joueur Droit",225));
    partieCourante_->setFieldName(FICHIER_TERRAIN_EN_COURS);
    partieCourante_->getReadyToPlay();
    partieCourante_->miseAuJeu(true);
    enJeu_ = true;
    // On enregistre apres avoir desactiver les points pour ne pas les voir si on reinitialise la partie
    obtenirVue()->centrerCamera(partieCourante_->getField()->GetTableWidth());

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::passageMenuPrincipal()
///
/// Passage au menu principal, on d�salloue certains �l�ments pour �viter les fuites de m�moires.
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////////////////////////////
bool FacadeModele::passageMenuPrincipal()
{
    // Indique que nous ne somme plus en train de jouer le tournoi
    if(tournoi_)
        tournoi_->modifierEstEnCours(false);

    if(partieCourante_)
    {
        if(!partieCourante_->faitPartieDunTournoi())
        {
            delete partieCourante_;
        }
    }
    partieCourante_ = 0;

    enJeu_ = false;
    selectionArbre(false);
    return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::reinitialiserPartie(  )
///
/// Permet de reinitialiser la partie courante
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::reinitialiserPartie()
{
    if(partieCourante_)
    {
        partieCourante_->reinitialiserPartie();
        partieCourante_->miseAuJeu(true);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::initialiserVue()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::initialiserVue()
{



//  vue_ = new vue::VueOrtho(
//      vue::Camera(
//         Vecteur3(0, 0, 150), Vecteur3(0, 0, 0),
//      Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
//      0, 500, 0, 500,
//      -1000, 10000, /*ZoomInMax*/60, /*ZoomOutMax*/1500, 1.25,
//      -100, 100, -100, 100
//      );
    
    
//  vue_ = new vue::VuePerspectiveLibre(
//      vue::Camera(Vecteur3(0, -0.0001, 200), Vecteur3(0, 0, 0),
//      Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
//      0, 400, 0, 400,
//      180, 10000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
//      -150, 150, -150, 150);

//  vue_ = new vue::VuePerspectiveCiel(
//      vue::Camera(Vecteur3(0, -100, 200), Vecteur3(0, 0, 0),
//      Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
//      0, 400, 0, 400,
//      180, 10000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
//      -150, 150, -150, 150);


    vue_ = new vue::VuePerspectiveOrbit(
        vue::Camera(Vecteur3(0, -0.0001f, 300), Vecteur3(0, 0, 0),
        Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
        0, 400, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25f,
        -150, 150, -150, 150);

    

    /*AnimationFrame* frame[6];
    frame[0] = new AnimationFrame(0, Vecteur3(-150, -75, 100));
    frame[1] = new AnimationFrame(1000, Vecteur3(150, -75, 100));
    frame[2] = new AnimationFrame(2000, Vecteur3(150, 75, 100));
    frame[3] = new AnimationFrame(3000, Vecteur3(-150, 75, 100));
    frame[4] = new AnimationFrame(4000, Vecteur3(-150, -75, 100));
    frame[5] = new AnimationFrame(5000, Vecteur3(150, 0, 0));


    Animation* animation = new Animation(CAMERALIBRE, BEZIER);
    for(int i=0; i<6; i++)
        animation->ajouterFrame(frame[i]);


    animation->ajouterObjet((ObjetAnimable*)&(vue_->obtenirCamera()));
    GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
    */

    


}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::visiterArbre( VisiteurNoeud* visiteur )
///
/// Launch a visitor on the field
///
/// @param[in] VisiteurNoeud * visiteur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::acceptVisitor( VisiteurNoeud& visitor )
{
    getEditionField()->acceptVisitor(visitor);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::convertirClotureAVirtuelle( int x, int y, Vecteur3& point ) const
///
/// Permet la convertion facile des coordonee clotures en coordonnes virtuelles
///
/// @param[in]      x     : La position @a X du point en coordonn�e
///                         d'affichage.
/// @param[in]      y     : La position @a Y du point en coordonn�e
///                         d'affichage.
/// @param[in, out] point : point transform� @a (x,y) le @a z est le m�me
///                         que le plan
/// @return Faux s'il est impossible de convertir le point en virtuel
///         puisque le plan est parall�le � la direction de la cam�ra.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FacadeModele::convertirClotureAVirtuelle( int x, int y, Vecteur3& point ) const
{
    return vue_->convertirClotureAVirtuelle(x, y, point);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::selectionArbre( bool selectionner )
///
/// Permet de selectionner ou de deselectionner l'arbre de rendu en entier
///
/// @param[in]      selectionner    : true = selection, false = deselection
/// 
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::selectionArbre( bool selectionner )
{
    getEditionField()->setTableItemsSelection(selectionner);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::validerPosition() const
///
/// Effectue les tests sur les collisions
///
/// @param[in]  void
///
/// @return bool    : position valide si true
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::validerPositionNoeud(NoeudAbstrait* noeudAValider, bool flag /* = false*/)
{
    return getEditionField() && getEditionField()->IsNodeAtValidEditionPosition(noeudAValider,flag);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::positionOccupe() const
///
/// Effectue les tests sur les collisions
///
/// @param[in]  void
///
/// @return bool    : position valide si true
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::pointOccupe( Vecteur2 positionSouris )
{
    Vecteur3 positionVirt;
    FacadeModele::convertirClotureAVirtuelle((int)positionSouris[VX], (int)positionSouris[VY], positionVirt);
    VisiteurCollision visiteurColision(Vecteur2(positionVirt[VX], positionVirt[VY]), false);
    acceptVisitor(visiteurColision);
    // Retourne true s'il y a un objet a la position cliquee
    return visiteurColision.collisionPresente();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 FacadeModele::convertirDistanceClotureAVirtuelle( const Vecteur2i& v ) const
///
/// Conversion d'une distance de coordonnees cloture a virtuelle
///
/// @param[in]  v   : vecteur contenant la distance (x,y) en coordonnees cloture
///
/// @return bool    : conversion reussie
///
////////////////////////////////////////////////////////////////////////
Vecteur2 FacadeModele::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const
{
    return vue_->convertirDeplacementClotureAVirtuelle(v, v2);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::possedeSelection()
///
/// Retourne true si un noeud est selectionne dans l'arbre de rendu
///
/// @param[in]  void
///
/// @return bool    : un noeud est selectionne
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::possedeSelection()
{
    return getEditionField() && getEditionField()->IsAnyNodeSelected();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::insideLimits( NoeudAbstrait* noeud )
///
/// V�rification pour voir si un noeud est dans les limites de la zone edition
///
/// @param[in] NoeudAbstrait * noeud : noeud � v�rifier
///
/// @return bool Vrai si le noeud est dans les limites de la zone
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::insideLimits( NoeudAbstrait* noeud )
{
    return !!getEditionField() && getEditionField()->insideLimits(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string FacadeModele::obtenirTypeNoeudSelectionne(  )
///
/// retourne le typeRetour des noeuds selectionn�
///
///
/// @return std::string : typeRetour de noeud si unique ou "" si vide ou multiple
///
////////////////////////////////////////////////////////////////////////
std::string FacadeModele::obtenirTypeNoeudSelectionne()
{
    VisiteurEstSelectione visiteur;
    acceptVisitor(visiteur);
    ConteneurNoeuds* noeudsSelectionnes = visiteur.obtenirListeNoeuds();
    std::string typeRetour;
    if(!noeudsSelectionnes->empty())
        typeRetour = (*noeudsSelectionnes)[0]->obtenirType();
    else
        return "";
    for(unsigned int i = 0; i < noeudsSelectionnes->size(); i++)
    {
        if((*noeudsSelectionnes)[i]->obtenirType() != typeRetour)
        {
            typeRetour = "";
            break;
        }
    }
    return typeRetour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::ajouterJoueur( SPJoueurAbstrait joueur )
///
/// Ajout d'un Joueur
///
/// @param[in] SPJoueurAbstrait joueur : Pointeur Intelligent sur le joueur � ajouter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::ajouterJoueur( SPJoueurAbstrait joueur )
{
    if(joueur != 0)
    {
//      if((joueurs_.find(joueur->obtenirNom())) != joueurs_.end())
//      {
//          //delete joueurs_[joueur->obtenirNom()];
//          joueurs_[joueur->obtenirNom()] = joueur;
//      }
//      else
        {
            // Si le joueur existait deja, il sera ecraser et liberer si personne pointe dessus
            profilsVirtuels_[joueur->obtenirNom()] = joueur;
        }
        enregistrerJoueurs();
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::supprimerJoueur( std::string nom )
///
/// Suppression d'un joueur dans le conteneur de joueur
///
/// @param[in] std::string nom
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::supprimerJoueur(std::string nom)
{
    ConteneurJoueur::iterator iter;
    if((iter = profilsVirtuels_.find(nom) ) != profilsVirtuels_.end())
    {
        //delete joueurs_[nom];
        profilsVirtuels_.erase(iter);
    }
    enregistrerJoueurs();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurAbstrait* FacadeModele::obtenirJoueur( std::string nom )
///
/// Accesseur d'un profil de joueur
///
/// @param[in] std::string nom : nom du joueur que l'on desire acceder
///
/// @return JoueurAbstrait*
///
////////////////////////////////////////////////////////////////////////
SPJoueurAbstrait FacadeModele::obtenirJoueur( std::string nom )
{
    if(profilsVirtuels_.find(nom) != profilsVirtuels_.end())
        return profilsVirtuels_[nom];

    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn ConteneurJoueursTries FacadeModele::obtenirListeJoueurs()
///
/// Obtention de la liste tri�es des joueurs
///
/// @return ConteneurJoueursTries : le set de joueurs
///
////////////////////////////////////////////////////////////////////////
ConteneurJoueursTries FacadeModele::obtenirListeNomsJoueurs()
{
    ConteneurJoueur::iterator iter;
    ConteneurJoueursTries listeJoueurs;

    for(iter = profilsVirtuels_.begin(); iter != profilsVirtuels_.end(); iter++)
    {
        listeJoueurs.insert(iter->first);
    }

    return listeJoueurs;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::verifierValiditeMap(Terrain* terrain)
///
/// V�rifie si un terrain est valide lorsqu'on passe en mode jeu
///
/// @param[in] Terrain* terrain : le terrain � valider.
///
/// @return bool : vrai si le terrain est valide
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::verifierValiditeMap( Terrain* terrain/*= 0 */ )
{
    /// Si le parametre est egal a 0, on assigne l'arbre du Modele
    if(!terrain)
    {
        terrain = getEditionField();
        if(!terrain)
            return false;
    }
    return terrain->verifierValidite();
}


//////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::creerTerrainParDefaut()
///
/// Cr�ation d'un terrain par d�faut.
/// Celui-ci a les dimemsions de base, 2 maillets et une rondelle
///
/// @return Un pointeur vers l'arbre cr��.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::creerTerrainParDefaut( )
{
    GestionnaireEvenements::modifierEtat(ETAT_MODE_EDITION);
    mEditionField->creerTerrainParDefaut(FICHIER_TERRAIN_EN_COURS);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet* FacadeModele::obtenirMailletJoueur1() const
///
/// Retourne le maillet du joueur 1
///
/// @return NoeudMaillet* : maillet du joueur 1
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet* FacadeModele::obtenirMailletJoueurGauche() const
{
    NoeudMaillet* maillet = 0;
    if(getEditionField())
    {
        if(getEditionField()->getTable())
        {
            NoeudComposite* g = (NoeudComposite*)getEditionField()->getTable()->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
            if(g)
            {
                for(unsigned int i=0; i<g->obtenirNombreEnfants(); ++i)
                {
                    NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->chercher(i));
                    if(m->getPosition()[VX]<=0)
                        maillet = m;
                }

            }
        }
    }
    return maillet;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet* FacadeModele::obtenirMailletJoueur2() const
///
/// Retourne le maillet du joueur 2
///
/// @return NoeudMaillet* : maillet du joueur 2
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet* FacadeModele::obtenirMailletJoueurDroit() const
{
    NoeudMaillet* maillet = 0;
    if(getEditionField())
    {
        if(getEditionField()->getTable())
        {
            NoeudComposite* g = (NoeudComposite*)getEditionField()->getTable()->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
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
    }
    return maillet;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::obtenirAttributsNoeudSelectionne()
///
/// Retourne un objet modificateur contenant les valeurs correspondant a celles de l'objet selectionne
///
/// @return jobject : Objet modificateur qui contient les atributs de l'objet selectionne
///
////////////////////////////////////////////////////////////////////////
jobject FacadeModele::obtenirAttributsNoeudSelectionne(JNIEnv* env)
{
    VisiteurEstSelectione visiteur;
    acceptVisitor(visiteur);
    ConteneurNoeuds* listeNoeud = visiteur.obtenirListeNoeuds();


    float coeffRebondBandes[8];
    float largeurTable;
    float longueurTable;
    float friction;
    if(getEditionField())
    {
        checkf(getEditionField()->getZoneEdition(),"terrain sans zone �dition");
        longueurTable = getEditionField()->getZoneEdition()->obtenirLimiteExtLongueur();
        largeurTable  = getEditionField()->getZoneEdition()->obtenirLimiteExtLargeur();
        
        NoeudTable* table = getEditionField()->getTable();
        if(!table)
            throw ExceptionJeu("Impossible de trouver la table de jeu");

        friction = table->obtenirCoefFriction();

        for(int i=0; i<8; i++)
        {
            coeffRebondBandes[i] = table->obtenirCoefRebond(i);
        }
    }

    jclass classe = env->FindClass("ca/polymtl/inf2990/Jeu/ModificateurProprieteNoeud");
    jmethodID constructeur = env->GetMethodID(classe, "<init>", "()V"); // Obtient l'id du constructeur
    jmethodID modifierFrictionId = env->GetMethodID(classe, "modifierCoefFriction", "(D)V");
    jmethodID modifierRebondId = env->GetMethodID(classe, "modifierCoefRebond", "(D)V");
    jmethodID modifierAccellerationId = env->GetMethodID(classe, "modifierBonusAccel", "(D)V");
    jmethodID modifierPositionXId = env->GetMethodID(classe, "modifierPositionX", "(D)V");
    jmethodID modifierPositionYId = env->GetMethodID(classe, "modifierPositionY", "(D)V");
    jmethodID modifierEchelleId = env->GetMethodID(classe, "modifierEchelle", "(D)V");
    jmethodID modifierRotationId = env->GetMethodID(classe, "modifierRotation", "(I)V");
    jmethodID modifierZoneEditionXId = env->GetMethodID(classe, "modifierZoneEditionX", "(D)V");
    jmethodID modifierZoneEditionYId = env->GetMethodID(classe, "modifierZoneEditionY", "(D)V");
    jmethodID modifierCoefRebondBandesId = env->GetMethodID(classe, "modifierCoefRebondBandes", "(ID)V");
    

    jobject modificateurAEnvoyer = env->NewObject(classe, constructeur);

    //Vecteur3 pos((float)DBL_MAX, (float)DBL_MAX, (float)DBL_MAX);
    Vecteur3 pos(0,0,0);
    int rotation = -1;
    Vecteur3 echelleTotale(0, 0, 0);
    float echelle = -1;
    float rebond = -1;
    float acceleration = -1;

    std::string type;
    bool trouve = false,ok = true;
    ConteneurNoeuds::iterator iter = listeNoeud->begin();
    for (; iter != listeNoeud->end() ; iter++)
    {
        std::string type2 = (*iter)->obtenirType();
        if(type2!="but_milieu")
        {
            if(!trouve)
            {
                type = (*iter)->obtenirType();
                trouve = true;
            }
            else
            {
                if(type2 != type)
                    ok = false;
            }
        }
    }

    // On laisse les valeurs par defaut s'il y a plus ou moins qu'un noeud de selectionner
    // Java va l'interpreter comme si le champ en question doit etre desactive s'il recoit les valeurs par defaut
    if(trouve && ok)
    {
        // On accede uniquement le premier element de la liste
        NoeudAbstrait* noeudATraiter = (*listeNoeud)[0];

        pos = noeudATraiter->getPosition();
        rotation = (int)noeudATraiter->obtenirAngle();
        if(rotation<0)
            rotation+=360;

        noeudATraiter->obtenirEchelleCourante(echelleTotale);
        echelle = max(max(echelleTotale[VX], echelleTotale[VY]), echelleTotale[VZ]);

        if(type=="muret")
            echelle = echelleTotale[VX]/10.0f;
        if(type=="point")
        {
            if(noeudATraiter->obtenirNombreEnfants()>0)
            {
                NoeudAbstrait* enfant = noeudATraiter->chercher(0);
                if(enfant->obtenirType()=="but_milieu")
                {
                    Vecteur3 echelleCouranteBut;
                    enfant->obtenirEchelleCourante(echelleCouranteBut);
                    echelle = echelleCouranteBut[VX]/10.0f;
                }
                else
                {
                    echelle = -1.0;
                }
            }
            else
            {
                echelle = -1.0;
            }
            rotation = -1;
        }

        NodeWallAbstract* muret = dynamic_cast<NodeWallAbstract*>(noeudATraiter);
        if (muret)
        {
            rebond = muret->getReboundRatio();
        }
        else
        {
            rebond = -1;
        }

        NoeudAccelerateur* accelerateur = dynamic_cast<NoeudAccelerateur*>(noeudATraiter);
        if (accelerateur)
        {
            acceleration = accelerateur->obtenirBonusAccel();
        }
        else
        {
            acceleration = -1;
        }
    }

    env->CallVoidMethod(modificateurAEnvoyer,modifierFrictionId,friction);
    env->CallVoidMethod(modificateurAEnvoyer,modifierRebondId,rebond);
    env->CallVoidMethod(modificateurAEnvoyer,modifierAccellerationId,acceleration);
    env->CallVoidMethod(modificateurAEnvoyer,modifierEchelleId,echelle);
    env->CallVoidMethod(modificateurAEnvoyer,modifierRotationId,rotation);
    env->CallVoidMethod(modificateurAEnvoyer,modifierPositionXId,pos[VX]);
    env->CallVoidMethod(modificateurAEnvoyer,modifierPositionYId,pos[VY]);
    env->CallVoidMethod(modificateurAEnvoyer,modifierZoneEditionXId,longueurTable);
    env->CallVoidMethod(modificateurAEnvoyer,modifierZoneEditionYId,largeurTable);
    for(int i=0; i<8; i++)
    {
        env->CallVoidMethod(modificateurAEnvoyer,modifierCoefRebondBandesId,i,coeffRebondBandes[i]);
    }

    return modificateurAEnvoyer;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::initialiserTournoi( JoueursParticipant joueurs )
///
/// Initialise un tournoi.
///
/// @param[in] JoueursParticipant joueurs : Les joueurs du tournoi
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::initialiserTournoi( JoueursParticipant joueurs, std::string terrain )
{
    if(tournoi_ == NULL)
        tournoi_ = new Tournoi();
    tournoi_->initialisation(joueurs,terrain);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::modifierAdversaire(JoueurAbstrait* val) 
///
/// Modifier l'adversaire pour une partie
///
/// @param[in] JoueurAbstrait* val : Le joueur qui sera l'adversaire.
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::modifierAdversaire(SPJoueurAbstrait val) 
{ 
//  if(adversaire_ != NULL) 
//      delete adversaire_; 
    adversaire_ = val; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::resetHighlightFlags( )
///
/// Enleve tous les flags restants dans l'arbre de rendu
///
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::resetHighlightFlags()
{
    VisiteurFunction visitor([](NoeudAbstrait* n)
    {
        n->modifierSurligner(false);
    });
    mEditionField->acceptVisitor(visitor);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::appliquerVue()
///
/// Application de la projection et la camera
///
/// @param[in]  quelViewport :  index du viewport a utiliser
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::appliquerVue(int quelViewport/* = 1*/)
{
    vue::Vue* vueCourante = obtenirVue();
    vueCourante->appliquerVue(quelViewport);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::togglePause()
///
/// Change l'etat de pause
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::togglePause()
{
    if(partieCourante_ && !GestionnaireAnimations::obtenirInstance()->estJouerReplay())
        partieCourante_->modifierEnPause(!partieCourante_->estEnPause());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::modifierEnPause( bool val )
///
/// Modificateur de pause de la partie
///
/// @param[in] bool val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::modifierEnPause( bool val )
{
    if(partieCourante_)
        partieCourante_->modifierEnPause(val);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::estEnPause() const
///
/// Indique si la partie est en pause
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::estEnPause() const
{
    if(partieCourante_)
        return partieCourante_->estEnPause(); 
    // S'il n'y a pas de partie courante, alors on ne peut etre en pause
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle* FacadeModele::obtenirRondelle(  )
///
/// Accesseur de la rondelle
///
///
/// @return NoeudRondelle*
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle* FacadeModele::obtenirRondelle() const
{
    if(getEditionField())
    {
        return getEditionField()->getPuck();
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle* FacadeModele::modifierVue( vue::Vue* nouvelleVue )
///
/// Modifie la vue (changement de mode de camera)
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::modifierVue( vue::Vue* nouvelleVue )
{
    delete vue_;
    vue_ = nouvelleVue;
}
void FacadeModele::afficherProgramInfoLog( GLuint obj, const char* message )
{
#if !SHIPPING 
    // afficher le message d'en-t�te
    cout << message << endl;

    // afficher le message d'erreur, le cas �ch�ant
    int infologLength = 0;
    glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

    if ( infologLength > 1 )
    {
        char* infoLog = new char[infologLength+1];
        int charsWritten  = 0;
        glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
        //glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
        cout << infoLog << endl;
        delete[] infoLog;
    }
    else
    {
        cout << "Aucune erreur :-)" << endl;
    }
#endif
}
void FacadeModele::afficherShaderInfoLog( GLuint obj, const char* message )
{
#if !SHIPPING 
    // afficher le message d'en-t�te
    cout << message << endl;

    // afficher le message d'erreur, le cas �ch�ant
    int infologLength = 0;
    glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

    if ( infologLength > 1 )
    {
        char* infoLog = new char[infologLength+1];
        int charsWritten  = 0;
        glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
        cout << infoLog << endl;
        delete[] infoLog;
    }
    else
    {
        cout << "Aucune erreur :-)" << endl;
    }
#endif
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void initialiserNuanceurs(  )
///
/// Fonction d'initialisation des shaders
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::initialiserNuanceurs()
{
    // d�claration des cha�nes qui devront recevoir le code des nuanceurs
    // lire les fichiers de nuanceurs de sommets et de fragments
    const char *ns = textFileRead( "nuanceurSommets.glsl" );
    const char *nf = textFileRead( "nuanceurFragments.glsl" );

    // cr�er le programme
    progPhong_ = glCreateProgram();

    // cr�er les nuanceurs de sommets et de fragments
    GLuint nuanceurSommets = glCreateShader( GL_VERTEX_SHADER );
    GLuint nuanceurFragments = glCreateShader( GL_FRAGMENT_SHADER );

    // associer le fichier de sommets au nuanceur de sommets
    glShaderSource( nuanceurSommets, 1, &ns, NULL );
    glCompileShader( nuanceurSommets );
    glAttachShader( progPhong_, nuanceurSommets );
    afficherShaderInfoLog( nuanceurSommets, "nuanceurSommets" );

    // associer le fichier de fragments au nuanceur de fragments
    glShaderSource( nuanceurFragments, 1, &nf, NULL );
    glCompileShader( nuanceurFragments );
    glAttachShader( progPhong_, nuanceurFragments );
    afficherShaderInfoLog( nuanceurFragments, "nuanceurFragments" );

    // linker le programme
    glLinkProgram( progPhong_ );
    afficherProgramInfoLog( progPhong_, "progPhong_" );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::DrawSelectionRectangle()
///
/// Affiche le rectangle elastique
/// Le dessin du rectangle elastique doit ce faire apres le swapbuffer sinon on ne le voit pas
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::DrawSelectionRectangle() const
{
    if(zoomElastique_)
        aidegl::dessinerRectangleElastique(coinElastique1_,coinElastique2_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float FacadeModele::getTableWidth()
///
/// /*Description*/
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float FacadeModele::getTableWidth()
{
    return mEditionField->GetTableWidth();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAffichage* FacadeModele::obtenirDecompte()
///
/// /*Description*/
///
///
/// @return NoeudAffichage*
///
////////////////////////////////////////////////////////////////////////
NoeudAffichage* FacadeModele::obtenirDecompte()
{
    return obtenirPartieCourante()->obtenirDecompte();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::RunOnRenderThread( Runnable* run )
///
/// /*Description*/
///
/// @param[in] Runnable * run
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::RunOnRenderThread( Runnable* run, bool pForceQueue /*= false*/ )
{
    if(mUpdating || pForceQueue)
    {
        mUIRunnables.push(run);
    }
    else
    {
        run->Run();
        if(run->KeepAlive())
        {
            mUIRunnables.push(run);
        }
        else
        {
            delete run;
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::RunOnUpdateThread( Runnable* run )
///
/// /*Description*/
///
/// @param[in] Runnable * run
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::RunOnUpdateThread( Runnable* run, bool pForceQueue /*= false*/ )
{
    if(mRendering || pForceQueue)
    {
        mUpdateRunnables.push(run);
    }
    else
    {
        run->Run();
        if(run->KeepAlive())
        {
            mUpdateRunnables.push(run);
        }
        else
        {
            delete run;
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::duplicateSelection()
///
/// duplicate nodes selected that can be duplicated
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::duplicateSelection()
{
    if(getEditionField())
    {
        getEditionField()->duplicateSelection();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn ConteneurNoeuds FacadeModele::getSelectedNodes()
///
/// retrieves node selected on the field
///
///
/// @return ConteneurNoeuds
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::getSelectedNodes(ConteneurNoeuds& pSelectedNodes) const
{
    if(getEditionField())
    {
        getEditionField()->getSelectedNodes(pSelectedNodes);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int FacadeModele::transmitEvent( EventCodes pCode )
///
/// Send event to the controller
///
/// @param[in] EventCodes pCode
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::transmitEvent( EventCodes pCode, ... )
{
    va_list args;
    va_start(args, pCode);
    GestionnaireReseau::obtenirInstance()->transmitEvent(pCode,args);
    va_end(args);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int FacadeModele::obtenirNbNoeudSelect()
///
/// /*Description*/
///
///
/// @return unsigned int
///
////////////////////////////////////////////////////////////////////////
unsigned int FacadeModele::obtenirNbNoeudSelect()
{
    VisiteurEstSelectione v;
    acceptVisitor(v);
    return (unsigned int)v.obtenirListeNoeuds()->size();
}











///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////