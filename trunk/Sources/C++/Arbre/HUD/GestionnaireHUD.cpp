//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireHUD.cpp
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireHUD.h"
#include "HUDSurfaceGL.h"
#include "HUDTexte.h"
#include "HUDMultiligne.h"
#include "HUDElementComparatif.h"
#include "gl/GLU.h"
#include "Utilitaire.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "Tournoi.h"
#include "GestionnaireModeles.h"
#include "GestionnaireEvenements.h"
#include "SourisEtatAbstrait.h"
#include "GestionnaireEtatAbstrait.h"
#include "GestionnaireAnimations.h"

// Déclarations de base pour le singleton
SINGLETON_DECLARATION_CPP(GestionnaireHUD);

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireHUD::GestionnaireHUD()
///
/// Constructeur par défaut.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GestionnaireHUD::GestionnaireHUD()
{
	creerHUDJeu();
	creerHUDTournoi();
	creerHUDEdition();

	GestionnaireModeles::obtenirInstance()->obtenirListe("pause",listePause_);
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
	racineJeu_->vider();
	delete racineJeu_;
	racineEdition_->vider();
	delete racineEdition_;
	racineTournoi_->vider();
	delete racineTournoi_;
}

typedef std::vector<NomEtatSouris> ConteneurNomEtatSouris;
typedef std::vector<std::string> ConteneurString;

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
	/*
	HUDTexte* nomOutilComponent = new HUDTexte(TypeTexte::NOM_OUTIL_MODE_EDITION, Vecteur4(1,0,0,1));
	HUDTexte* aideOutilComponent = new HUDTexte(TypeTexte::AIDE_OUTIL_MODE_EDITION, Vecteur4(1,1,1,1));
	
	nomOutilComponent->modifierPosition(0.01,0.05);
	aideOutilComponent->modifierPosition(0.01,0.95);
	*/

	//la liste de tous les outils java.


	ConteneurNomEtatSouris etats;
	ConteneurString nomsOutils;
	ConteneurString infos;
	//,ETAT_SOURIS_DEPLACER_FENETRE
	etats.push_back(ETAT_SOURIS_DEPLACER_FENETRE);
	nomsOutils.push_back("Déplacer la caméra");
	infos.push_back("Cliquez-glissez la souris pour déplacer la vue.");

	//,ETAT_SOURIS_ZOOM_ELASTIQUE		
	etats.push_back(ETAT_SOURIS_ZOOM_ELASTIQUE);
	nomsOutils.push_back("Zoom élastique");
	infos.push_back("Fonction qui n'existe plus. Désolé.");

	//,ETAT_SOURIS_ZOOM_PROPORTIONNEL	
	etats.push_back(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
	nomsOutils.push_back("Zoom proportionnel");
	infos.push_back("Cliquez-glissez la souris pour ajuster le zoom de la vue.");

	//,ETAT_SOURIS_TRANSFORMATION_ROTATION	
	etats.push_back(ETAT_SOURIS_TRANSFORMATION_ROTATION);
	nomsOutils.push_back("Rotation d'un objet");
	infos.push_back("Cet outil effectue une rotation sur tous les objets sélectionnés.\nCliquez-glissez de haut en bas afin d'effectuer la transformation.");

	//,ETAT_SOURIS_TRANSFORMATION_ECHELLE	
	etats.push_back(ETAT_SOURIS_TRANSFORMATION_ECHELLE);
	nomsOutils.push_back("Mise à l'échelle d'un objet");
	infos.push_back("Cet outil effectue une mise à l'échelle sur tous les objets sélectionnés.\nCliquez-glissez de haut en bas afin d'effectuer la transformation.");

	//,ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT	
	etats.push_back(ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT);
	nomsOutils.push_back("Déplacer un objet");
	infos.push_back("Cet outil déplace tous les objets sélectionnés en direction de la souris.\nCliquez-glissez d'effectuer le déplacement.");

	//,ETAT_SOURIS_SELECTION			
	etats.push_back(ETAT_SOURIS_SELECTION);
	nomsOutils.push_back("Sélection d'objets");
	infos.push_back("Sélectionne des objets afin de pouvoir les transformer par la suite.\nUn clic sur un objet sélectionne ce dernier.\nClic-déplacer crée une zone de sélection, dans laquelle tout sera sélectionné.\nCtrl-clic ajoute des éléments au groupe.");

	//,ETAT_SOURIS_AJOUTER_PORTAIL	
	etats.push_back(ETAT_SOURIS_AJOUTER_PORTAIL);
	nomsOutils.push_back("Ajout de portails");
	infos.push_back("Cliquez pour ajouter un portail.\nL'élément sera placé uniquement si l'endroit est valide.");

	//,ETAT_SOURIS_AJOUTER_MURET	
	etats.push_back(ETAT_SOURIS_AJOUTER_MURET);
	nomsOutils.push_back("Ajout de murets");
	infos.push_back("Ajoute des murets.\nLe premier clic place le début du muret, le second clic place la fin du muret.\nLe muret sera tracé entre les deux clics.\nL'élément sera placé uniquement si l'endroit est valide.");

	//,ETAT_SOURIS_AJOUTER_MAILLET			
	etats.push_back(ETAT_SOURIS_AJOUTER_MAILLET);
	nomsOutils.push_back("Ajout de maillets");
	infos.push_back("Cliquez pour ajouter des maillets.\nVous ne pouvez placer que deux maillets sur la surface de jeu.\nL'élément sera placé uniquement si l'endroit est valide.");

	//,ETAT_SOURIS_AJOUTER_RONDELLE			
	etats.push_back(ETAT_SOURIS_AJOUTER_RONDELLE);
	nomsOutils.push_back("Ajout de la rondelle");
	infos.push_back("Cliquez pour ajouter une rondelle.\nVous ne pouvez placer qu'une seule rondelle.\nL'élément sera placé uniquement si l'endroit est valide.");

	//,ETAT_SOURIS_AJOUTER_ACCELERATEUR
	etats.push_back(ETAT_SOURIS_AJOUTER_ACCELERATEUR);
	nomsOutils.push_back("Ajout d'un accélérateur");
	infos.push_back("Cliquez pour ajouter un accélérateur.\nL'élément sera placé uniquement si l'endroit est valide.");

	//,ETAT_SOURIS_ORBIT
	etats.push_back(ETAT_SOURIS_ORBIT);
	nomsOutils.push_back("Changer la rotation de la caméra");
	infos.push_back("Cliquez-glissez la roulette de la souris pour déplacer la vue autour de la surface de jeu.\nLa roulette de la souris effectue un zoom.\nCliquer-glisser déplace autour du centre de rotation.(Camera Orbite)\n Ou autour de la camera en mode libre");



	for(int i=0;i<etats.size() && i < nomsOutils.size() && i < infos.size();++i)
	{
		HUDElementComparatif* panneauDeFond = new HUDElementComparatif([=]() -> bool {
            NomEtatSouris indexOutilSelectionne;
            if(!GestionnaireEvenements::obtenirInstance()->obtenirEtat()->GetMouseStateName(indexOutilSelectionne))
				return false;
			return indexOutilSelectionne==etats[i];
		});
		panneauDeFond->modifierTaille(1.0f,1.0f);
		panneauDeFond->modifierPosition(0.0f,0.0f);

		HUDTexte* nom = new HUDTexte(nomsOutils[i], Vecteur4f(1.0f, 1.0f, 0.0f, 1.0f));
		nom->modifierPosition(0.01f,0.05f);
		panneauDeFond->add(nom);

		HUDMultiligne* description = new HUDMultiligne(infos[i], Vecteur4f(1.0f, 1.0f, 0.0f, 1.0f));
		description->modifierPosition(0.0f,0.0f);
		description->modifierTaille(1.0f,1.0f);
		panneauDeFond->add(description);

		racineEdition_->add(panneauDeFond);
	}

	HUDTexte* messageReset= new HUDTexte("Appuyez sur '0' pour replacer la caméra", Vecteur4f(1.0f,1.0f,0.0f,1.0f));
	messageReset->modifierPosition(0.65f,0.95f);
	racineEdition_->add(messageReset);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::dessinerHUDEdition()
///
/// Traçage du HUD.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::dessinerHUDEdition()
{
	// États de la lumière et de la profondeur
	GLboolean lighting_state, depth_test_state;
	// Désactiver le test de profondeur et l'éclairage
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

	// Réactiver l'éclairage et le test de profondeur (s'il y a lieu)
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
	std::string policeGrosTexte = "game_over_big_text";

	// ATTENTION Chaque surface doit avoir son propre conteneur de vertexes.

	// Surface pour les scores
	HUDElementComparatif* panneauScores = new HUDElementComparatif([](){return !GestionnaireAnimations::obtenirInstance()->estJouerReplay();});
	panneauScores->modifierPosition(0.05f, 0.01f);
	panneauScores->modifierTaille(0.9f,0.1f);

	//Surface pour le joueur de gauche
	ConteneurVertex2D* listeDesVertexes01=new ConteneurVertex2D;
	listeDesVertexes01->push_back(Vecteur2f(1.0f,0.5f));
	listeDesVertexes01->push_back(Vecteur2f(1.0f,0.0f));
	for(int angle = 90; angle <= 270; ++angle)
	{
		listeDesVertexes01->push_back(Vecteur2(0.1f+(0.1f*cos(utilitaire::DEG_TO_RAD(angle))), 0.5f-(0.5f*sin(utilitaire::DEG_TO_RAD(angle)))));
	}
	listeDesVertexes01->push_back(Vecteur2f(1.0f,1.0f));
	HUDSurfaceGL *surfaceJoueurGauche = new HUDSurfaceGL(GL_TRIANGLE_FAN, listeDesVertexes01, Vecteur4f(0.0f,0.0f,1.0f,0.6f));
	surfaceJoueurGauche->modifierPosition(0.0f, 0.0f);
	surfaceJoueurGauche->modifierTaille(0.5f, 1.0f);

	HUDTexte* nomJoueurGauche=new HUDTexte(NOM_JOUEUR_GAUCHE, Vecteur4f(1.0f,1.0f,1.0f,1.0f), policeGrosTexte);
	nomJoueurGauche->modifierPosition(0.3f,0.4f);
	surfaceJoueurGauche->add(nomJoueurGauche);

	HUDTexte* scoreJoueurGauche=new HUDTexte(POINTAGE_JOUEUR_GAUCHE, Vecteur4f(1.0f,1.0f,1.0f,1.0f), policeGrosTexte);
	scoreJoueurGauche->modifierPosition(0.4f,0.9f);
	surfaceJoueurGauche->add(scoreJoueurGauche);

	panneauScores->add(surfaceJoueurGauche);

	// Surface pour le joueur de droite
	listeDesVertexes01=new ConteneurVertex2D;
	listeDesVertexes01->push_back(Vecteur2f(0.0f,1.0f));
	for(int angle = 270; angle >= 90; --angle)
	{
		listeDesVertexes01->push_back(Vecteur2(1.0f-0.1f-(0.1f*cos(utilitaire::DEG_TO_RAD(angle))), 0.5f-(0.5f*sin(utilitaire::DEG_TO_RAD(angle)))));
	}
	listeDesVertexes01->push_back(Vecteur2f(0.0f,0.0f));
	listeDesVertexes01->push_back(Vecteur2(0.0f,0.5f));

	HUDSurfaceGL *surfaceJoueurDroit = new HUDSurfaceGL(GL_TRIANGLE_FAN, listeDesVertexes01, Vecteur4f(1.0f,0.0f,0.0f,0.6f));
	surfaceJoueurDroit->modifierPosition(0.5f, 0.0f);
	surfaceJoueurDroit->modifierTaille(0.5f, 1.0f);

	HUDTexte* nomJoueurDroit=new HUDTexte(NOM_JOUEUR_DROIT, Vecteur4f(1.0f,1.0f,1.0f,1.0f), policeGrosTexte);
	nomJoueurDroit->modifierPosition(0.5f,0.4f);
	surfaceJoueurDroit->add(nomJoueurDroit);

	HUDTexte* scoreJoueurDroit=new HUDTexte(POINTAGE_JOUEUR_DROIT, Vecteur4f(1.0f,1.0f,1.0f,1.0f), policeGrosTexte);
	scoreJoueurDroit->modifierPosition(0.6f,0.9f);
	surfaceJoueurDroit->add(scoreJoueurDroit);

	panneauScores->add(surfaceJoueurDroit);

	// Symbole au milieu.
	listeDesVertexes01 = new ConteneurVertex2D;
	listeDesVertexes01->push_back(Vecteur2f(0.5f, 0.5f));
	for(int i=360;i>=0;--i)
		listeDesVertexes01->push_back(Vecteur2f((float)(0.5f+0.5f*cos(utilitaire::DEG_TO_RAD(i))),(float)(0.5f+0.5f*sin(utilitaire::DEG_TO_RAD(i)))));
	HUDSurfaceGL *cercleDuCentre = new HUDSurfaceGL(GL_TRIANGLE_FAN, listeDesVertexes01,Vecteur4f(1.0f,1.0f,1.0f,1.0f));
	cercleDuCentre->modifierPosition(0.47f,0.0f);
	cercleDuCentre->modifierTaille(0.06f,0.7f);
	cercleDuCentre->assignerTexture(std::string("media/razer-logo_opaque.png"));
	panneauScores->add(cercleDuCentre);

	// Chronomètre sous le symbole.
	listeDesVertexes01 = new ConteneurVertex2D;
	listeDesVertexes01->push_back(Vecteur2f(0.0f, 0.0f));
	listeDesVertexes01->push_back(Vecteur2f(0.0f, 1.0f));
	listeDesVertexes01->push_back(Vecteur2f(1.0f, 1.0f));
	listeDesVertexes01->push_back(Vecteur2f(1.0f, 0.0f));

	HUDSurfaceGL *panneauTempsJeu = new HUDSurfaceGL(GL_QUADS, listeDesVertexes01,Vecteur4f(0.0f,0.0f,0.0f,1.0f));
	panneauTempsJeu->modifierPosition(0.4f,0.7f);
	panneauTempsJeu->modifierTaille(0.2f,0.3f);
	panneauScores->add(panneauTempsJeu);

	HUDTexte* texteTempsJeu=new HUDTexte(TEMPS_JEU, Vecteur4f(1.0f,1.0f,1.0f,1.0f));
	texteTempsJeu->modifierPosition(0.4f,1.0f);
	panneauTempsJeu->add(texteTempsJeu);

	racineJeu_->add(panneauScores);
	
	HUDElementComparatif* panneauVainqueur = new HUDElementComparatif([](){
		return FacadeModele::obtenirInstance()->obtenirPartieCourante()->partieTerminee() 
			&& !GestionnaireAnimations::obtenirInstance()->estJouerReplay();
	
	}, Vecteur4f(0.0f,0.0f,0.0f,0.8f));
	panneauVainqueur->modifierTaille(0.6f, 0.6f);
	panneauVainqueur->modifierPosition(0.2f, 0.2f);

	HUDTexte* textePartieTerminee = new HUDTexte("Partie terminée! Le gagnant est:", Vecteur4f(1.0f,1.0f,1.0f,1.0f), policeGrosTexte);
	textePartieTerminee->modifierPosition(0.1f, 0.33f);
	panneauVainqueur->add(textePartieTerminee);

	HUDTexte* texteNomVainqueur = new HUDTexte(NOM_VAINQUEUR, Vecteur4f(1.0f,1.0f,1.0f,1.0f), policeGrosTexte);
	texteNomVainqueur->modifierPosition(0.3f, 0.66f);
	panneauVainqueur->add(texteNomVainqueur);

	std::string messageContinuer("Appuyez sur une touche pour continuer...");
	HUDTexte* espacePourContinuer = new HUDTexte(messageContinuer, Vecteur4f(1.0f,1.0f,1.0f,0.9f), policeGrosTexte);
	espacePourContinuer->modifierPosition(0.0f, 0.9f);
	panneauVainqueur->add(espacePourContinuer);
	std::string messageContinuer2("Ou appuyer sur R pour une reprise video");
	HUDTexte* espacePourContinuer2 = new HUDTexte(messageContinuer2, Vecteur4f(1.0f,1.0f,1.0f,0.9f), policeGrosTexte);
	espacePourContinuer2->modifierPosition(0.0f, 1.0f);
	panneauVainqueur->add(espacePourContinuer2);
	
	racineJeu_->add(panneauVainqueur);

	//panneau pour le replay.
	HUDElementComparatif* panneauReplay = new HUDElementComparatif([]()	{return GestionnaireAnimations::obtenirInstance()->estJouerReplay();});

	ConteneurVertex2D* pointsReplay = new ConteneurVertex2D;
	pointsReplay->push_back(Vecteur2f(0.0f,0.0f));
	pointsReplay->push_back(Vecteur2f(0.0f,1.0f));
	pointsReplay->push_back(Vecteur2f(1.0f,1.0f));
	pointsReplay->push_back(Vecteur2f(1.0f,0.0f));
	HUDSurfaceGL* imageReplay=new HUDSurfaceGL(GL_QUADS, pointsReplay, Vecteur4f(1.0f,1.0f,1.0f,1.0f));
	imageReplay->assignerTexture(std::string("media/replay_frame.png"));
	panneauReplay->add(imageReplay);
	panneauReplay->modifierPosition(0,0);
	panneauReplay->modifierTaille(1,1);
	imageReplay->modifierPosition(0,0);
	imageReplay->modifierTaille(1,1);
	racineJeu_->add(panneauReplay);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::dessinerHUDJeu()
///
/// Traçage du HUD.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::dessinerHUDJeu()
{
	// États de la lumière et de la profondeur
	GLboolean lighting_state, depth_test_state;
	// Désactiver le test de profondeur et l'éclairage
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
	racineJeu_->repeindre();
	effectuerDecompte();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); // MODELVIEW
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // PROJECTION
	glPopAttrib(); // ATTRIBUTS

	// Réactiver l'éclairage et le test de profondeur (s'il y a lieu)
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

			unsigned int indexJoueur = j & 1;
			HUDTexte* panneauNomJoueur = new HUDTexte(indexPartie, indexJoueur);
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
	std::string messageContinuer("Appuyez sur une touche pour continuer...");
	HUDTexte* espacePourContinuer = new HUDTexte(messageContinuer, Vecteur4f(1.0f,1.0f,1.0f,0.9f));
	espacePourContinuer->modifierPosition(0.6f, 0.90f);
	racineTournoi_->add(espacePourContinuer);
	std::string messageContinuer2("Ou appuyer sur R pour une reprise video");
	HUDTexte* espacePourContinuer2 = new HUDTexte(messageContinuer2, Vecteur4f(1.0f,1.0f,1.0f,0.9f));
	espacePourContinuer2->modifierPosition(0.6f, 0.95f);
	racineTournoi_->add(espacePourContinuer2);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::dessinerHUDTournoi()
///
/// Traçage du HUD.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::dessinerHUDTournoi()
{
	
	// États de la lumière et de la profondeur
	GLboolean lighting_state, depth_test_state;
	// Désactiver le test de profondeur et l'éclairage
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
	glLineWidth(3);
	racineTournoi_->repeindre();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); // MODELVIEW
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // PROJECTION
	glPopAttrib(); // ATTRIBUTS

	// Réactiver l'éclairage et le test de profondeur (s'il y a lieu)
	if (lighting_state == GL_TRUE) {
		glEnable(GL_LIGHTING);
	}
	if (depth_test_state == GL_TRUE) {
		glEnable(GL_DEPTH_TEST);
	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireHUD::effectuerDecompte()
///
/// Effectue le décompte en début de partie.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireHUD::effectuerDecompte()
{
	NoeudAffichage* decompte = FacadeModele::obtenirInstance()->obtenirDecompte();
	
	glPushMatrix();
	glTranslatef(0.5f, 0.5f, 0.0f);
	glScalef(0.005f, -0.005f, 0.001f);
	decompte->afficher();
	glPopMatrix();

	static int anglePause_ = 0;
	if(listePause_ != 0 && FacadeModele::obtenirInstance()->estEnPause())
	{
		anglePause_ = (anglePause_+5)%360;
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glColor3f(0.0f, 0.0f, 0.7f);

		glTranslatef(0.8f, 0.8f, 0.0f);
		glScalef(0.002f, -0.002f, 0.0001f);
		glRotated(anglePause_, 0, 0, 1);

		glCallList(listePause_);

		glPopAttrib();
		glPopMatrix();
	}
	else
		anglePause_ = 0;	glMatrixMode(GL_MODELVIEW);
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
	case RACINE_EDITION: return racineEdition_;
	case RACINE_TOURNOI: return racineTournoi_;
	};
	return 0;
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



