///////////////////////////////////////////////////////////////////////////////
/// @file FacadeModele.cpp
/// @author Martin Bisson
/// @date 2007-05-22
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////


// Commentaire Doxygen mis sur la première page de la documentation Doxygen.
/**

@mainpage Projet intégrateur de deuxième année -- INF2990

@li <a href="../javadoc/index.html">Documentation Javadoc de la partie Java.</a>
*/

#include <windows.h>
#include <cassert>

#include "FreeImage.h"

#include "FacadeModele.h"

#include "VueOrtho.h"

#include "Camera.h"
#include "Projection.h"
#include "LignePointillee.h"

#include "Utilitaire.h"
#include "AideGL.h"
#include "ArbreRenduINF2990.h"
#include "CompteurAffichage.h"
#include "ConfigScene.h"

// Remlacement de EnveloppeXML/XercesC par TinyXML
// Julien Gascon-Samson, été 2011
#include "tinyxml.h"

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
#include "Box2D/Box2D.h"
#include "BoiteEnvironnement.h"
#include <iostream>
#include "DebugRenderBox2D.h"

/// Pointeur vers l'instance unique de la classe.
FacadeModele* FacadeModele::instance_ = 0;

/// Chaîne indiquant le nom du fichier de configuration du projet.
const std::string FacadeModele::FICHIER_TERRAIN_EN_COURS = "MapEnCours.xml";
const std::string FacadeModele::FICHIER_JOUEURS = "joueurs.xml";

int FacadeModele::anglePause_ = 0;

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

    if(!wglMakeCurrent(FacadeModele::obtenirInstance()->obtenirHDC(),data->glrc))
    {
        returnValue = false;
    }
    ReleaseMutex(mutexRender);

    FacadeModele::obtenirInstance()->InitOpenGLContext();

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
        FacadeModele::obtenirInstance()->afficher();
        ReleaseMutex(mutexNoeuds);
    }

    wglDeleteContext(data->glrc);

    return returnValue;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::changerFond(int r, int g, int b, float a)
///
/// Change la couleur du fond, utilisée lors de nos premiers tests.
///
/// @param[in] r : Quantité de rouge.
/// @param[in] g : Quantité de vert.
/// @param[in] b : Quantité de bleu.
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
/// @fn FacadeModele* FacadeModele::obtenirInstance()
///
/// Cette fonction retourne un pointeur vers l'instance unique de la
/// classe.  Si cette instance n'a pas été créée, elle la crée.  Cette
/// création n'est toutefois pas nécessairement "thread-safe", car
/// aucun verrou n'est pris entre le test pour savoir si l'instance
/// existe et le moment de sa création.
///
/// @return Un pointeur vers l'instance unique de cette classe.
///
////////////////////////////////////////////////////////////////////////
FacadeModele* FacadeModele::obtenirInstance()
{
   if (instance_ == 0)
      instance_ = new FacadeModele;

   return instance_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::libererInstance()
///
/// Cette fonction libère l'instance unique de cette classe.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::libererInstance()
{
	ConfigScene::obtenirInstance()->enregistrerConfiguration();
	// On libère les instances des différentes configurations.
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
/// Ce constructeur par défaut ne fait qu'initialiser toutes les
/// variables à une valeur nulle.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
FacadeModele::FacadeModele()
	: hGLRC_(0), hDC_(0), hWnd_(0), vue_(0),zoomElastique_(false),tournoi_(0),cheminTournoi_(""),enJeu_(false),
	partieCourante_(0), /*adversaire_(0),*/ terrain_(0)
{
	// Il ne faut pas faire d'initialisation de Noeud ici, car le contexte OpenGl n'est pas encore creer

	// Initialisation de la randomisation
	srand( (unsigned int) clock()+(unsigned int) time);

#ifdef WIN32
	CreateDirectoryA(
		"tournoi",
		NULL
		);
#else
	// creation d'un dossier avec un autre API, dunno which yet
#endif
    b2Vec2 gravity;
    mWorld = new b2World(gravity);
    mDebugRenderBox2D = new DebugRenderBox2D();
    mWorld->SetDebugDraw(mDebugRenderBox2D);
    mDebugRenderBox2D->AppendFlags(b2Draw::e_shapeBit);
    NoeudAbstrait::setWorld(mWorld);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeModele::~FacadeModele()
///
/// Ce destructeur libère les objets du modèle.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
FacadeModele::~FacadeModele()
{
	if(vue_)
		delete vue_;
	if(terrain_)
		delete terrain_;
	if(boiteEnvironnement!=0)
	{
		delete boiteEnvironnement;
		boiteEnvironnement=0;
	}
    if(mWorld)
    {
        delete mWorld;
        mWorld = NULL;
    }
    delete mDebugRenderBox2D;
    mDebugRenderBox2D = NULL;
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

    // modèle de rendu
    glShadeModel( GL_SMOOTH );

    // Initialisation des shaders
#ifdef GLSHADERS
    initialiserNuanceurs();
#endif
    // couleur de l'arrière-plan
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
/// Cette fonction permet d'initialiser le contexte OpenGL.  Elle crée
/// un contexte OpenGL sur la fenêtre passée en paramètre, initialise
/// FreeImage (utilisée par le chargeur 3d) et assigne des paramètres
/// du contexte OpenGL.
///
/// @param[in] hWnd : La poignée ("handle") vers la fenêtre à utiliser.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::initialiserOpenGL(HWND hWnd)
{
	hWnd_ = hWnd;
	bool succes = aidegl::creerContexteGL(hWnd_, hDC_, hGLRC_);
	assert(succes);

    InitOpenGLContext();

    // FreeImage, utilisée par le chargeur, doit être initialisée
    FreeImage_Initialise();

	// Initialisation des modèles
    CheckTime(GestionnaireModeles::obtenirInstance(););
	ConfigScene::obtenirInstance();
	SoundFMOD::obtenirInstance();

	//ConfigScene::obtenirInstance()->obtenirLumiere(0)->allumerLumiere();
 	//ConfigScene::obtenirInstance()->obtenirLumiere(1)->allumerLumiere();
 	//ConfigScene::obtenirInstance()->obtenirLumiere(2)->allumerLumiere();

	// On crée une vue par défaut.
	initialiserVue();

	// Initialisation du skybox
	const std::string xpos = "media/sb2_xpos.png";
	const std::string xneg = "media/sb2_xneg.png";
	const std::string ypos = "media/sb2_ypos.png";
	const std::string yneg = "media/sb2_yneg.png";
	const std::string zpos = "media/sb2_zpos.png";
	const std::string zneg = "media/sb2_zneg.png";
	boiteEnvironnement= new utilitaire::BoiteEnvironnement(xpos,xneg,ypos,yneg,zpos,zneg);

	SoundFMOD::obtenirInstance()->playApplicationSong(STARTUP_SONG);
	chargerJoueurs();


	// Création de l'arbre de rendu.  À moins d'être complètement certain
	// d'avoir une bonne raison de faire autrement, il est plus sage de créer
	// l'arbre après avoir créé le contexte OpenGL.
	// Création d'un terrain de base pointant sur le fichier de terrain en cours
	terrain_ = new Terrain();
    
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
//     renderWorkerData.terrain = &terrain_;
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
/// @fn void FacadeModele::chargerTerrain() const
///
/// Cette fonction charge la configuration à partir d'un fichier XML si
/// ce dernier existe.  Sinon, le fichier de configuration est généré à
/// partir de valeurs par défaut directement dans le code.
///
/// @param[in] nomFichier : Le nom du fichier à charger.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::chargerTerrain( const std::string& nomFichier /*= ""*/, Terrain* terrain /*= 0*/ )
{
	// En place pour eviter des cas speciaux avec la souris (soit des ajout d'objets liberer 2 fois)
	//GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
	std::string fichierACharger;

	// Si le nom du fichier est vide, on utilise celui par défaut.
	if(nomFichier == "")
		fichierACharger = FICHIER_TERRAIN_EN_COURS;
	else
		fichierACharger = nomFichier;

	
	// Si le parametre est null, on lui assigne le terrain du modele
	if(!terrain)
	{
		terrain = obtenirTerrain();
	}
	// Si le terrain du modele est null, on en cree un
	if(!terrain)
	{
		terrain = new Terrain();
	}
	
	// Vérification de l'existence du ficher
	if ( !utilitaire::fichierExiste(fichierACharger) ) 
	{
		// Si on est en jeu on s'assure d'avoir une table valide
		if(enJeu_)
			terrain->creerTerrainParDefaut(fichierACharger);
		else
			terrain->initialiser(fichierACharger);
		// Si le fichier n'existe pas, on le crée.
		enregistrerTerrain(fichierACharger);
	}
	// si le fichier existe on le lit
	else 
	{
		TiXmlDocument document;

		// Lire à partir du fichier de configuration
		if( !document.LoadFile ( fichierACharger.c_str() ) )
		{
			utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier");
			// Si on est en jeu on s'assure d'avoir une table valide
			if(enJeu_)
				terrain->creerTerrainParDefaut(fichierACharger);
			else
				terrain->initialiser(fichierACharger);
			// Si le fichier n'existe pas, on le crée.
			enregistrerTerrain(fichierACharger);
		}
		else
		{
			if(!terrain->initialiserXml((TiXmlElement*)&document))
				terrain->initialiser(fichierACharger);
			if(!enJeu_)
			{
				for (int i = 0; i < 8 ; i++)
				{
					obtenirTerrain()->obtenirTable()->obtenirPoint(i)->assignerAffiche(true);
				}
			}
			ajusterElementSurTableEnCollision();
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::enregistrerTerrain() const
///
/// Cette fonction génère un fichier XML de configuration à partir de
/// valeurs par défaut directement dans le code.
///
/// @param[in] nomFichier : Le nom du fichier à enregistrer.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::enregistrerTerrain( const std::string& nomFichier /*= ""*/, Terrain* terrain /*= 0*/ ) 
{
	std::string fichierAEnregistrer;
	
	// Si le nom du fichier est vide, on utilise celui par défaut.
	if(nomFichier == "")
		fichierAEnregistrer = FICHIER_TERRAIN_EN_COURS;
	else
		fichierAEnregistrer = nomFichier;

	// Si le parametre est null, on lui assigne le terrain du modele
	if(!terrain)
	{
		terrain = obtenirTerrain();
	}
	// Si le terrain du modele est null, on l'initialise avant de continuer
	if(!terrain)
	{
		terrain = new Terrain();
		terrain->initialiser(fichierAEnregistrer);
	}


	TiXmlDocument document;

	// Écrire la déclaration XML standard...
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
	document.LinkEndChild(declaration);

	// Creation du noeud du terrain
	document.LinkEndChild( terrain->creerNoeudXML());

	// Écrire dans le fichier
	document.SaveFile( fichierAEnregistrer.c_str() );
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::chargerJoueurs( const std::string& nomFichier /*= ""*/, ConteneurJoueur* joueurs /*= 0*/ )
///
/// Charge les joueurs à partir d'un fichier XML.
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

	// Si le nom du fichier est vide, on utilise celui par défaut.
	if(nomFichier == "")
		fichierACharger = FICHIER_JOUEURS;
	else
		fichierACharger = nomFichier;

	if(joueurs == 0)
		joueurs = &profilsVirtuels_;
	TiXmlDocument document;

	if(!utilitaire::fichierExiste(fichierACharger))
	{
		enregistrerJoueurs();
		return;
	}
	if( !document.LoadFile ( fichierACharger.c_str() ) )
	{
		utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier");
		return;
	}

	// On enregistre les différentes configurations.
	ConfigScene::obtenirInstance()->lireDOM(document,*joueurs);

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

	// Si le nom du fichier est vide, on utilise celui par défaut.
	if(nomFichier == "")
		fichierAEnregistrer = FICHIER_JOUEURS;
	else
		fichierAEnregistrer = nomFichier;

	if(joueurs == 0)
		joueurs = &profilsVirtuels_;
	TiXmlDocument document;
	// Écrire la déclaration XML standard...
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
	document.LinkEndChild(declaration);

	// On enregistre les différentes configurations.
	ConfigScene::obtenirInstance()->creerDOM(document,*joueurs);
	// Écrire dans le fichier
	document.SaveFile( fichierAEnregistrer.c_str() );
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
		TiXmlDocument document;
		if( !document.LoadFile ( nomFichier.c_str() ) )
		{
			utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier de tournoi");
			return;
		}
		Tournoi* tournoi = new Tournoi();
		TiXmlElement* elem = document.FirstChildElement("Tournoi");
		if(tournoi->initialisationXML(elem,&profilsVirtuels_))
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
/// @param[in] Tournoi * tournoi : le tournoi à enregistrer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::enregistrerTournoi( Tournoi* tournoi )
{
	if(tournoi == 0)
		tournoi = tournoi_;

	TiXmlDocument document;
	// Écrire la déclaration XML standard...
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
	document.LinkEndChild(declaration);
	// On enregistre les différentes configurations.
	document.LinkEndChild(tournoi->creerTournoiXML());
	// Écrire dans le fichier
	document.SaveFile( (("tournoi/" + tournoi->obtenirNom()) + ".xml").c_str() );
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::libererOpenGL()
///
/// Cette fonction libère le contexte OpenGL et désinitialise FreeImage.
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
/// Liberation de la mémoire
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
            // thread terminé trop tot
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

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::afficher() const
///
/// Cette fonction affiche le contenu de la scène.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::afficher( )
{

	// Efface l'ancien rendu
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	for(int i=1; i<=obtenirVue()->obtenirNbViewports(); i++)
	{
		appliquerVue(i);

		glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

		// ICI APPLIQUER LES PATENTE DE SHADER
		//glUniform1i( glGetUniformLocation( progPhong_, "colorMap" ), 0 );
		//glMatrixMode( GL_MODELVIEW );
		boiteEnvironnement->afficher(vue_->obtenirCamera(i).obtenirPosition(),5000.0);

#ifdef GLSHADERS
		glUseProgram( progPhong_ );
		glActiveTexture( GL_TEXTURE0 );
		Vecteur3f positionCamera = vue_->obtenirCamera(i).obtenirPosition();
		glUniform3fARB( glGetUniformLocationARB( progPhong_, "eyeVec" ), positionCamera[VX], positionCamera[VY], positionCamera[VZ] );
		glUniform1i( glGetUniformLocation( progPhong_, "colorMap" ), 0 );
#endif

		// On rafraichi la lumière
		ConfigScene::obtenirInstance()->rafraichirLumiere();

		afficherBase();

#ifdef GLSHADERS
		glUseProgram(0);
#endif
	}
	glDisable(GL_LIGHTING);

    DrawSelectionRectangle();
    mWorld->DrawDebugData();
    //mDebugRenderBox2D->DrawString(150,150,"test %d",this->temps_.Elapsed_Time_sec());

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

	if(vue_->obtenirNbViewports()>1)
	{
		// Il faut remettre le viewport full pour le hud

		obtenirVue()->obtenirProjection().mettreAJourCloture();

	}
	// Affichage specifique aux etats
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->afficher();
	
	appliquerVue(1);

	glEnable(GL_LIGHTING);

	// Compte de l'affichage
	utilitaire::CompteurAffichage::obtenirInstance()->signalerAffichage();

	// Échange les tampons pour que le résultat du rendu soit visible.
	::SwapBuffers( hDC_ );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::afficherBase() const
///
/// Cette fonction affiche la base du contenu de la scène, c'est-à-dire
/// qu'elle met en place l'éclairage et affiche les objets.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::afficherBase() const
{
    // Afficher la scène
    terrain_->afficherTerrain(!enJeu_);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::rafraichirFenetre() const
///
/// Cette fonction rafraîchit le contenu de la fenêtre, c'est-à-dire
/// qu'elle indique à Windows que la fenêtre doit être repeinte.  Un
/// événement sera donc généré puis traité plus tard, ce qui évite de
/// faire plusieurs affichages sans raison.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::rafraichirFenetre() const
{
	// Indique à Windows que la fenêtre doit être rafraîchie.
	//::InvalidateRect(hWnd_, 0, FALSE);

	// Finalement, on choisit de simplement appeler afficher() pour éviter
	// que le fond ne soit redessiné par Windows ou Java.

	// Or, comme il y a en permanence une boucle qui réaffiche, on ne fait
	// rien.

	//afficher();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::reinitialiserTerrain()
///
/// Cette fonction réinitialise la scène à un état "vide".
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::reinitialiserTerrain()
{
	terrain_->reinitialiser();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::animer( const float& temps)
///
/// Cette fonction effectue les différents calculs d'animations
/// nécessaires pour le mode jeu, tel que les différents calculs de
/// physique du jeu.
///
/// @param[in] temps : Intervalle de temps en sec sur lequel effectuer le calcul.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::animer( const float& temps)
{
	utilitaire::CompteurAffichage::obtenirInstance()->signalerAffichage();
	
	float tempsReel = temps;//temps_.Elapsed_Time_sec();
// 	if(tempsReel > 0.5)
// 	{
// 		temps_.reset_Time();
// 		return;
// 	}
// 	if(tempsReel < 0.015)
// 		return;
// 	temps_.reset_Time();

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

	GestionnaireEvenements::obtenirInstance()->miseAJourEvenementsRepetitifs(tempsReel);
	// Mise à jour des objets
	terrain_->animerTerrain(tempsReel);
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->animer(tempsReel);
	bool replaying = false;
	if(GestionnaireAnimations::obtenirInstance()->estJouerReplay())
		replaying = true;
	GestionnaireAnimations::obtenirInstance()->animer(tempsReel*1000);// Prends le temps en ms
	if(replaying && obtenirPartieCourante() && !obtenirPartieCourante()->partieTerminee() && !GestionnaireAnimations::obtenirInstance()->estJouerReplay())
	{
		obtenirPartieCourante()->obtenirGameTime()->unPause();
	}


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::deplacerSouris(Vecteur2i deplacement)
///
/// Fonction qui déplace la vue.
///
/// @param[in] deplacement : le vecteur de déplacement de la caméra.
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
/// Fonction qui déplace la vue.
///
/// @param[in] deplacement : le vecteur de déplacement de la caméra.
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
/// @param[in] deplacement : le vecteur de déplacement de la caméra.
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
/// Fonction qui déplace la vue.
///
/// @param[in] nbCoche : Le nombre de coches parcourus par la roulette
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::zoom(int nbCoches)
{
	// Si le nombre de coche est négatif, c'est qu'on roule vers l'avant.
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
	// Si le nombre de coche est négatif, c'est qu'on roule vers l'avant.
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
/// S'occupe d'afficher le rectangle élastique.
///
/// @param[in] bool actif : Indique si on active le zoom élastique
/// @param[in] Vecteur2i coin1 : Le coin en haut à gauche
/// @param[in] Vecteur2i coin2 : Le coin en bas à droite
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
/// Passage au mode édition.
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
	if(enJeu_)
	{
		chargerTerrain();
	}
	else
		terrain_->initialiser(FICHIER_TERRAIN_EN_COURS);
	if(partieCourante_ && !partieCourante_->faitPartieDunTournoi())
		delete partieCourante_;
	partieCourante_ = 0;

	enJeu_ = false;
	for (int i = 0; i < 8 ; i++)
	{
		obtenirArbreRenduINF2990()->obtenirTable()->obtenirPoint(i)->assignerAffiche(true);
	}

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
	if(!tournoi_)
	{
		utilitaire::afficherErreur("Le modele ne contient pas de tournoi a jouer");
		return false;
	}
	if(partieCourante_ && !partieCourante_->faitPartieDunTournoi())
		delete partieCourante_;

	GestionnaireAnimations::obtenirInstance()->viderBufferReplay();
	partieCourante_ = tournoi_->obtenirPartieCourante();

	// On charge le terrain du tournoi
	if(tournoi_->obtenirTerrain() == "")
	{
		// Terrain par defaut
		creerTerrainParDefaut();
	}
	else
		chargerTerrain(tournoi_->obtenirTerrain());

	if(!verifierValiditeMap())
	{
		utilitaire::afficherErreur("La table du tournoi n'est pas valide");
		return false;
	}

	try
	{
		partieCourante_->assignerControlesMaillet(obtenirMailletJoueurGauche(),obtenirMailletJoueurDroit(),obtenirRondelle());
	}
	catch(std::logic_error& e)
	{
		utilitaire::afficherErreur(e.what());
		return false;
	}

	partieCourante_->miseAuJeu(true);
	
	obtenirArbreRenduINF2990()->deselectionnerTout();
	enJeu_ = true;
	for (int i = 0; i < 8 ; i++)
	{
		obtenirArbreRenduINF2990()->obtenirTable()->obtenirPoint(i)->assignerAffiche(false);
	}

	// On enregistre dans le fichier de la partie courante apres avoir desactiver les points pour ne pas les voir si on reinitialise la partie
	enregistrerTerrain();
	obtenirVue()->obtenirProjection().centrerAZero();
	obtenirVue()->obtenirProjection().mettreAJourProjection();
	
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
// 	if(!verifierValiditeMap())
// 	{
// 		return false;
// 	}

	// Indique que nous ne somme plus en train de jouer le tournoi
	if(tournoi_)
		tournoi_->modifierEstEnCours(false);

	GestionnaireAnimations::obtenirInstance()->viderBufferReplay();
	if(!adversaire_)
		adversaire_ = SPJoueurAbstrait(new JoueurHumain("Joueur Droit"));
	partieCourante_ = new Partie(SPJoueurAbstrait(new JoueurHumain("Joueur Gauche")),adversaire_);
	//partieCourante_ = new Partie(new JoueurVirtuel("Joueur Gauche",225),new JoueurVirtuel("Joueur Droit",225));
	partieCourante_->modifierTerrain(FICHIER_TERRAIN_EN_COURS);
	try
	{
		partieCourante_->assignerControlesMaillet(obtenirMailletJoueurGauche(),obtenirMailletJoueurDroit(),obtenirRondelle());
	}
	catch(std::logic_error& e)
	{
		utilitaire::afficherErreur(e.what());
		return false;
	}
	
	partieCourante_->miseAuJeu(true);
	
	obtenirArbreRenduINF2990()->deselectionnerTout();
	enJeu_ = true;
	for (int i = 0; i < 8 ; i++)
	{
		obtenirArbreRenduINF2990()->obtenirTable()->obtenirPoint(i)->assignerAffiche(false);
	}

	// On enregistre apres avoir desactiver les points pour ne pas les voir si on reinitialise la partie
	enregistrerTerrain();
	obtenirVue()->obtenirProjection().centrerAZero();
	obtenirVue()->obtenirProjection().mettreAJourProjection();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::passageMenuPrincipal()
///
/// Passage au menu principal, on désalloue certains éléments pour éviter les fuites de mémoires.
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////////////////////////////
bool FacadeModele::passageMenuPrincipal()
{
	// Indique que nous ne somme plus en train de jouer le tournoi
	if(tournoi_)
		tournoi_->modifierEstEnCours(false);

	if(partieCourante_ && !partieCourante_->faitPartieDunTournoi())
		delete partieCourante_;
	partieCourante_ = 0;
	enJeu_ = false;
	obtenirArbreRenduINF2990()->deselectionnerTout();
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
	if(partieCourante_ != 0)
	{
		partieCourante_->reinitialiserPartie();
		chargerTerrain(partieCourante_->obtenirCheminTerrain());
		if(!verifierValiditeMap())
		{
			/// comportement inconnu
		}
		try
		{
			partieCourante_->assignerControlesMaillet(obtenirMailletJoueurGauche(),obtenirMailletJoueurDroit(),obtenirRondelle());
		}
		catch(std::logic_error& e)
		{
			utilitaire::afficherErreur(e.what());
			return;
		}
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



// 	vue_ = new vue::VueOrtho(
// 		vue::Camera(
//         Vecteur3(0, 0, 150), Vecteur3(0, 0, 0),
// 		Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
// 		0, 500, 0, 500,
// 		-1000, 10000, /*ZoomInMax*/60, /*ZoomOutMax*/1500, 1.25,
// 		-100, 100, -100, 100
// 		);
	
	
// 	vue_ = new vue::VuePerspectiveLibre(
// 		vue::Camera(Vecteur3(0, -0.0001, 200), Vecteur3(0, 0, 0),
// 		Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
// 		0, 400, 0, 400,
// 		180, 10000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
// 		-150, 150, -150, 150);

// 	vue_ = new vue::VuePerspectiveCiel(
// 		vue::Camera(Vecteur3(0, -100, 200), Vecteur3(0, 0, 0),
// 		Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
// 		0, 400, 0, 400,
// 		180, 10000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
// 		-150, 150, -150, 150);


	vue_ = new vue::VuePerspectiveOrbit(
		vue::Camera(Vecteur3(0, -0.0001, 300), Vecteur3(0, 0, 0),
		Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
		0, 400, 0, 400,
		180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
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




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::selection( VisiteurSelection &visiteur )
///
/// Effectue la selection dans l'arbre de rendu
///
/// @param[in]	visiteur	: visiteur de selection
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::visiterArbre( VisiteurNoeud* visiteur )
{
	obtenirArbreRenduINF2990()->accueillirVisiteurNoeud(*visiteur);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::convertirClotureAVirtuelle( int x, int y, Vecteur3& point ) const
///
/// Permet la convertion facile des coordonee clotures en coordonnes virtuelles
///
/// @param[in]      x     : La position @a X du point en coordonnée
///                         d'affichage.
/// @param[in]      y     : La position @a Y du point en coordonnée
///                         d'affichage.
/// @param[in, out] point : point transformé @a (x,y) le @a z est le même
///                         que le plan
/// @return Faux s'il est impossible de convertir le point en virtuel
///         puisque le plan est parallèle à la direction de la caméra.
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
/// @param[in]      selectionner	: true = selection, false = deselection
/// 
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FacadeModele::selectionArbre( bool selectionner )
{
	if(selectionner)
		obtenirArbreRenduINF2990()->selectionnerTout();
	else
		obtenirArbreRenduINF2990()->deselectionnerTout();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::validerPosition() const
///
/// Effectue les tests sur les collisions
///
/// @param[in]	void
///
/// @return bool	: position valide si true
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::validerPositionNoeud(NoeudAbstrait* noeudAValider, bool flag /* = false*/)
{
	VisiteurCollision visiteur(noeudAValider,flag);
	visiterArbre(&visiteur);
	return !(visiteur.collisionPresente()) && insideLimits(noeudAValider);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::positionOccupe() const
///
/// Effectue les tests sur les collisions
///
/// @param[in]	void
///
/// @return bool	: position valide si true
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::pointOccupe( Vecteur2 positionSouris )
{
	Vecteur3 positionVirt;
	FacadeModele::convertirClotureAVirtuelle((int)positionSouris[VX], (int)positionSouris[VY], positionVirt);
	VisiteurCollision visiteurColision(Vecteur2(positionVirt[VX], positionVirt[VY]), false);
	obtenirArbreRenduINF2990()->accueillirVisiteurNoeud(visiteurColision);
	// Retourne true s'il y a un objet a la position cliquee
	return visiteurColision.collisionPresente();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 FacadeModele::convertirDistanceClotureAVirtuelle( const Vecteur2i& v ) const
///
/// Conversion d'une distance de coordonnees cloture a virtuelle
///
/// @param[in]	v	: vecteur contenant la distance (x,y) en coordonnees cloture
///
/// @return bool	: conversion reussie
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
/// @param[in]	void
///
/// @return bool	: un noeud est selectionne
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::possedeSelection()
{
	return obtenirArbreRenduINF2990()->possedeSelection();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::insideLimits( NoeudAbstrait* noeud )
///
/// Vérification pour voir si un noeud est dans les limites de la zone edition
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return bool Vrai si le noeud est dans les limites de la zone
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::insideLimits( NoeudAbstrait* noeud )
{
	if(terrain_)
		return terrain_->insideLimits(noeud);
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::ajouterElementSurTable( NoeudAbstrait* n )
///
/// Méthode pour ajouter un élément sur la table
///
/// @param[in] NoeudAbstrait * n : noeud à ajouter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::ajouterElementSurTable( NoeudAbstrait* n )
{
	elementSurTable_.push_back(n);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::ajusterElementSurTableEnCollision()
///
/// Algorithme pour que les noeud en collisions se repositionne correctement
///
///
/// @return bool vrai si la méthode à réussi à trouver un emplacement valide pour tous les noeuds de la table
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::ajusterElementSurTableEnCollision( const unsigned int& nbIteration /*= 10*/ )
{
	bool tableValide = true;

	for(unsigned int iter = 0; iter < nbIteration ; ++iter)
	{
		for (unsigned int i = 0; i < elementSurTable_.size() ; ++i)
		{
			if(!ajusterElementEnCollision(elementSurTable_[i], 2))
				// On invalide la table seulement si nous somme dans la dernière itération
				if(iter == nbIteration-1)
					tableValide = false;
		}
	}
	return tableValide;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool FacadeModele::ajusterElementEnCollision( NoeudAbstrait* noeud, const unsigned int& nbIterations /*= 10*/, NoeudAbstrait* racine /*= 0*/ )
///
/// Permet de déplacer un noeud pour tenter de le positionner à un endroit où il n'est pas en collision
///
/// @param[in] NoeudAbstrait * noeud : noeud a replacer
/// @param[in] NoeudAbstrait * racine : racine de l'arbre contenant les éléments 
///										qui peuvent être en collision avec le noeud, par défaut on utilise l'arbre de rendu du modèle
/// @param[in] const unsigned int & nbIterations : nombre d'itérations pour les tentative de repositionnement
///
/// @return bool vrai si la méthode à réussi à trouver un emplacement valide au noeud selon le nombre d'itération
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::ajusterElementEnCollision( NoeudAbstrait* noeud, const unsigned int& nbIterations /*= 10*/, NoeudAbstrait* racine /*= 0*/ )
{
	// Si l'un des noeud est null, on ne fait pas le travail
	if(!noeud)
		return true;
	if(!racine)
		racine = obtenirArbreRenduINF2990();
	if(!racine)
		return true;

	// Avant les tests de collision, on regarde d'abord si le noeud est dans les limites
	if(!insideLimits(noeud))
	{
		// Repositionne au centre de la table
		noeud->assignerPositionRelative(Vecteur3());
	}

	VisiteurCollision v(noeud,false);
	racine->accueillirVisiteurNoeud(v);
	
	unsigned int tentative = 0;
	while( v.collisionPresente() && ++tentative <= nbIterations)
	{
		ConteneurNoeuds liste;
		ConteneurDetailsCollision details = v.obtenirConteneurDetailsCollision();
		v.obtenirListeCollision(liste);
		for (int j = 0; j < liste.size()  ; j++)
		{
			//Vecteur3 deplacement(elementSurTable_[i]->obtenirPositionAbsolue() - liste[j]->obtenirPositionAbsolue());
			Vecteur3 deplacement((details[j].direction*details[j].enfoncement)*-1);
			if(deplacement.norme() == 0)
				deplacement = Vecteur3(1.0,1.0);
			//deplacement.normaliser();
			VisiteurDeplacement vDeplacement(deplacement,true);
			noeud->accueillirVisiteurNoeud(vDeplacement);
		}
		if(!insideLimits(noeud))
		{
			// Repositionne au centre de la table
			noeud->assignerPositionRelative(Vecteur3());
		}
		v.reinitialiser();
		racine->accueillirVisiteurNoeud(v);
	}

	// Il faut la partie égale de la comparaison, car le ++tentative 
	// de la condition while ne ce fera pas s'il n'y a plus de collision
	return tentative <= nbIterations;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn std::string FacadeModele::obtenirTypeNoeudSelectionne(  )
///
/// retourne le typeRetour des noeuds selectionné
///
///
/// @return std::string : typeRetour de noeud si unique ou "" si vide ou multiple
///
////////////////////////////////////////////////////////////////////////
std::string FacadeModele::obtenirTypeNoeudSelectionne()
{
	VisiteurEstSelectione visiteur;
	obtenirArbreRenduINF2990()->accueillirVisiteurNoeud(visiteur);
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
/// @fn void FacadeModele::suprrimerElementSurTable( NoeudAbstrait* n )
///
/// Méthode pour supprimer un élément sur la table
///
/// @param[in] NoeudAbstrait * n : noeud à supprimer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::supprimerElementSurTable( NoeudAbstrait* n )
{
	ConteneurNoeuds::iterator iter = find(elementSurTable_.begin(),elementSurTable_.end(),n);
	if(iter != elementSurTable_.end())
		elementSurTable_.erase(iter);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::ajouterJoueur( SPJoueurAbstrait joueur )
///
/// Ajout d'un Joueur
///
/// @param[in] SPJoueurAbstrait joueur : Pointeur Intelligent sur le joueur à ajouter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::ajouterJoueur( SPJoueurAbstrait joueur )
{
	if(joueur != 0)
	{
// 		if((joueurs_.find(joueur->obtenirNom())) != joueurs_.end())
// 		{
// 			//delete joueurs_[joueur->obtenirNom()];
// 			joueurs_[joueur->obtenirNom()] = joueur;
// 		}
// 		else
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
/// Obtention de la liste triées des joueurs
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
/// @fn bool FacadeModele::verifierValiditeMap(ArbreRenduINF2990* arbre)
///
/// Vérifie si un terrain est valide lorsqu'on passe en mode jeu
///
/// @param[in] ArbreRenduINF2990* arbre : L'arbre de rendu représentant le terrain.
///
/// @return ConteneurJoueursTries : le set de joueurs
///
////////////////////////////////////////////////////////////////////////
bool FacadeModele::verifierValiditeMap( Terrain* terrain/*= 0 */ )
{
	//GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
	/// Si le parametre est egal a 0, on assigne l'arbre du Modele
	terrain = terrain == 0 ? obtenirTerrain() : terrain;
	if(!terrain)
		return false;
	return terrain->verifierValidite();
}


//////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::creerTerrainParDefaut()
///
/// Création d'un terrain par défaut.
/// Celui-ci a les dimemsions de base, 2 maillets et une rondelle
///
/// @return Un pointeur vers l'arbre créé.
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::creerTerrainParDefaut()
{
	if(!terrain_)
		terrain_ = new Terrain();
	obtenirTerrain()->creerTerrainParDefaut(FICHIER_TERRAIN_EN_COURS);	
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
	if(obtenirTerrain())
	{
		if(obtenirTerrain()->obtenirTable())
		{
			NoeudComposite* g = (NoeudComposite*)obtenirTerrain()->obtenirTable()->obtenirGroupe(ArbreRenduINF2990::NOM_MAILLET);
			if(g)
			{
				for(unsigned int i=0; i<g->obtenirNombreEnfants(); ++i)
				{
					NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->chercher(i));
					if(m->obtenirPositionAbsolue()[VX]<=0)
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
	if(obtenirTerrain())
	{
		if(obtenirTerrain()->obtenirTable())
		{
			NoeudComposite* g = (NoeudComposite*)obtenirTerrain()->obtenirTable()->obtenirGroupe(ArbreRenduINF2990::NOM_MAILLET);
			if(g)
			{
				for(unsigned int i=0; i<g->obtenirNombreEnfants(); ++i)
				{
					NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->chercher(i));
					if(m->obtenirPositionAbsolue()[VX]>0)
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
	obtenirArbreRenduINF2990()->accueillirVisiteurNoeud(visiteur);
	ConteneurNoeuds* listeNoeud = visiteur.obtenirListeNoeuds();


	double coeffRebondBandes[8];
	double largeurTable;
	double longueurTable;
	double friction;
	if(obtenirTerrain())
	{
		longueurTable = obtenirTerrain()->obtenirZoneEdition().obtenirLimiteExtLongueur();
		largeurTable  = obtenirTerrain()->obtenirZoneEdition().obtenirLimiteExtLargeur();

		NoeudTable* table = dynamic_cast<NoeudTable*>(obtenirArbreRenduINF2990()->chercher(ArbreRenduINF2990::NOM_TABLE));
		if(!table)
			throw std::runtime_error("Impossible de trouver la table de jeu");


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

	Vecteur3 pos(DBL_MAX, DBL_MAX, DBL_MAX);
	int rotation = -1;
	Vecteur3 echelleTotale(0, 0, 0);
	double echelle = -1;
	double rebond = -1;
	double acceleration = -1;

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

		pos = noeudATraiter->obtenirPositionAbsolue();
		rotation = (int)noeudATraiter->obtenirAngle();
		if(rotation<0)
			rotation+=360;

		noeudATraiter->obtenirEchelleCourante(echelleTotale);
		echelle = max(max(echelleTotale[VX], echelleTotale[VY]), echelleTotale[VZ]);

		if(type=="muret")
			echelle = echelleTotale[VX]/10.0;
		if(type=="point")
		{
			if(noeudATraiter->obtenirNombreEnfants()>0)
			{
				NoeudAbstrait* enfant = noeudATraiter->chercher(0);
				if(enfant->obtenirType()=="but_milieu")
				{
					Vecteur3 echelleCouranteBut;
					enfant->obtenirEchelleCourante(echelleCouranteBut);
					echelle = echelleCouranteBut[VX]/10.0;
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

		NoeudMuret* muret = dynamic_cast<NoeudMuret*>(noeudATraiter);
		if (muret)
		{
			rebond = muret->obtenirCoefRebond();
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
// 	if(adversaire_ != NULL) 
// 		delete adversaire_; 
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
	for (int i = 0; i < elementSurTable_.size() ; i++)
	{
		elementSurTable_[i]->modifierSurligner(false);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn inline const ArbreRenduINF2990* FacadeModele::obtenirArbreRenduINF2990() const
///
/// Cette fonction retourne l'arbre de rendu de la scène (version constante
/// de la fonction).
///
/// @return L'arbre de rendu de la scène.
///
////////////////////////////////////////////////////////////////////////
const ArbreRenduINF2990* FacadeModele::obtenirArbreRenduINF2990() const
{
	return terrain_->obtenirArbreRendu();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn inline ArbreRenduINF2990* FacadeModele::obtenirArbreRenduINF2990()
///
/// Cette fonction retourne l'arbre de rendu de la scène (version non constante
/// de la fonction).
///
/// @return L'arbre de rendu de la scène.
///
////////////////////////////////////////////////////////////////////////
ArbreRenduINF2990* FacadeModele::obtenirArbreRenduINF2990()
{
	if(terrain_)
		return terrain_->obtenirArbreRendu();
	else
		return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::appliquerVue()
///
/// Application de la projection et la camera
///
/// @param[in]	quelViewport :	index du viewport a utiliser
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeModele::appliquerVue(int quelViewport/* = 1*/)
{
	vue::Vue* vueCourante = obtenirVue();

	// Positionne la caméra
	glMatrixMode( GL_MODELVIEW );
	vueCourante->appliquerViewport(quelViewport);
	glLoadIdentity();
	vueCourante->appliquerCamera(quelViewport); // gluLookAt
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	Vecteur2i dimCloture = Vecteur2i(viewport[2], viewport[3]);
	Vecteur2 dimFenetre = vueCourante->obtenirProjection().obtenirDimensionFenetre();
	vueCourante->obtenirProjection().ajusterRapportAspect(dimCloture, dimFenetre); // APPLIQUE AUSSI LA PERSPECTIVE
	glMatrixMode (GL_MODELVIEW);			
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
	if(obtenirTerrain())
	{
		return obtenirTerrain()->obtenirRondelle();
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
	// afficher le message d'en-tête
	cout << message << endl;

	// afficher le message d'erreur, le cas échéant
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
}
void FacadeModele::afficherShaderInfoLog( GLuint obj, const char* message )
{
	// afficher le message d'en-tête
	cout << message << endl;

	// afficher le message d'erreur, le cas échéant
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
	// déclaration des chaînes qui devront recevoir le code des nuanceurs
	// lire les fichiers de nuanceurs de sommets et de fragments
	const char *ns = textFileRead( "nuanceurSommets.glsl" );
	const char *nf = textFileRead( "nuanceurFragments.glsl" );

	// créer le programme
	progPhong_ = glCreateProgram();

	// créer les nuanceurs de sommets et de fragments
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
/// @fn double FacadeModele::obtenirLargeurZoneEdition()
///
/// /*Description*/
///
///
/// @return double
///
////////////////////////////////////////////////////////////////////////
double FacadeModele::obtenirLargeurZoneEdition()
{
    return 2.0*obtenirTerrain()->obtenirZoneEdition().obtenirLimiteExtLargeur();
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











///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////