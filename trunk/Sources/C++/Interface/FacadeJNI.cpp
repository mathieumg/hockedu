///////////////////////////////////////////////////////////////////////////////
/// @file FacadeJNI.cpp
/// @author Martin Bisson
/// @date 2007-08-14
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "glew.h"

#include "jawt_md.h"
#include "JAWT_Info.h"
#include "FreeImage.h"

#include "FacadeJNI.h"
#include "Vecteur.h"
#include "FacadeModele.h"
#include "Vue.h"
#include "AideGL.h"
#include "ArbreRenduINF2990.h"
#include "CompteurAffichage.h"
#include "BancTests.h"

#include "GestionnaireEvenements.h"

#include "Evenements/EvenementClavier.h"
#include "Evenements/EvenementRouletteSouris.h"
#include "Evenements/EvenementSouris.h"
#include "ConfigSceneTest.h"
#include "UsineNoeudMaillet.h"
#include "UsineNoeudRondelle.h"
#include "VisiteurSelection.h"
#include "UtilitaireINF2990.h"
#include "JoueurVirtuel.h"
#include "JoueurHumain.h"
#include "INF2990TypeDef.h"
#include "VisiteurModifierProprieteNoeud.h"
#include "SoundFMOD.h"
#include "Tournoi.h"
#include "NoeudPoint.h"
#include "Terrain.h"
#include "NoeudTable.h"
#include "RepartiteurActions.h"



////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Lancement_fctC_1animer(JNIEnv*, jclass, jfloat temps)
///
/// Cette fonction effectue les différents calculs d'animations
/// nécessaires pour le mode jeu, tel que les différents calculs de
/// physique du jeu.
///
/// @param[in] temps : Intervalle de temps sur lequel effectuer le calcul.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Lancement_fctC_1animer
	(JNIEnv*, jclass, jfloat temps)
{
	FacadeModele::obtenirInstance()->animer(temps);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jint JNICALL Java_ca_polymtl_inf2990_Lancement_fctC_1executerTests(JNIEnv *, jclass)
///
/// Cette fonction permet d'exécuter l'ensemble des tests unitaires
///
/// @return 0 si tous les tests ont réussi, 1 si au moins un test a échoué
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jint JNICALL Java_ca_polymtl_inf2990_Lancement_fctC_1executerTests
	(JNIEnv *, jclass)
{
	UsineNoeudMaillet::bypassLimitePourTest = true;
	UsineNoeudRondelle::bypassLimitePourTest = true;
	bool reussite = BancTests::obtenirInstance()->executer();

	UsineNoeudMaillet::bypassLimitePourTest = false;
	UsineNoeudRondelle::bypassLimitePourTest = false;
	// Visual Studio interprète le code de retour 0 comme une réussite et le code
	// de retour 1 comme un échec. Nous transmettons le code de retour à Java
	// qui le transmet directement comme code de sortie du programme.
	return reussite ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_OpenGL_dessinerOpenGL(JNIEnv* env, jclass, jobject canvas)
///
/// Cette fonction affiche la scène dans la fenêtre identifiée par le
/// canvas AWT passé en paramètre.
///
/// @param[in] env    : L'environnement Java.
/// @param[in] canvas : Le canvas dans lequel créer le contexte OpenGL.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_OpenGL_dessinerOpenGL
	(JNIEnv* env, jclass, jobject canvas)
{
	// Ce code vérouille le canvas et ne doit donc pas être enlevé, même s'il
	// semble inutile à première vue.
	JAWT_Info info(env, canvas);
	HWND hWnd = (HWND)info.getHWND();

	if(hWnd == 0)
		return;

	// Affiche la scène.
	FacadeModele::obtenirInstance()->SignalRender();

	// Temporaire: pour détecter les erreurs OpenGL
	aidegl::verifierErreurOpenGL();
}




////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_OpenGL_initialiserOpenGL(JNIEnv* env, jclass, jobject canvas)
///
/// Cette fonction initialise un contexte OpenGL dans la fenêtre
/// identifiée par le canvas AWT passé en paramètre.  Cette fonction doit
/// être la première à être appelée, car la création de l'objet du modèle
/// C++ s'attend à avoir un contexte OpenGL valide.
///
/// @param[in] env    : L'environnement Java.
/// @param[in] canvas : Le canvas dans lequel créer le contexte OpenGL.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_OpenGL_initialiserOpenGL
  (JNIEnv* env, jclass, jobject canvas)
{
   // Obtenir la poignée ("handle") vers la fenêtre.
   JAWT_Info info(env, canvas);
   HWND hWnd = (HWND)info.getHWND();
	
   // On pourrait lancer une exception à la place...
   if(hWnd == 0)
      return;

	FacadeModele::obtenirInstance()->initialiserOpenGL(hWnd);
}




////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_OpenGL_libererOpenGL(JNIEnv* env, jclass, jobject canvas)
///
/// Cette fonction libère le contexte OpenGL dans la fenêtre identifiée
/// par le canvas AWT passé en paramètre.  Cette fonction doit être la
/// dernière à être appelée, car elle libère également l'objet du modèle
/// C++.
///
/// @param[in] env    : L'environnement Java.
/// @param[in] canvas : Le canvas dans lequel créer le contexte OpenGL.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_OpenGL_libererOpenGL
  (JNIEnv* env, jclass, jobject canvas)
{
   // Ce code vérouille le canvas et ne doit donc pas être enlevé, même s'il
   // semble inutile à première vue.
   JAWT_Info info(env, canvas);
   HWND hWnd = (HWND)info.getHWND();

   // On pourrait lancer une exception à la place...
   if(hWnd == 0)
      return;

   FacadeModele::obtenirInstance()->libererOpenGL();
	
	
   // Désinitialisation de la façade.  Le fait de le faire après la
   // désinitialisation du contexte OpenGL aura pour conséquence que la
   // libération des listes d'affichages, par exemple, sera faite une fois que
   // le contexte n'existera plus, et sera donc sans effet.
   FacadeModele::libererInstance();
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_keyTyped(JNIEnv * env, jobject, jobject keyEvent)
///
/// Fonction qui redirige un événement lié à une touche tapée.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] keyEvent: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_keyTyped( JNIEnv * env, jobject, jobject keyEvent)
{
	//Inutilisé
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_keyPressed(JNIEnv * env, jobject, jobject keyEvent)
/// Fonction qui redirige un événement lié à une touche enfoncée.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] keyEvent: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_keyPressed(JNIEnv * env, jobject, jobject keyEvent)
{
	GestionnaireEvenements::obtenirInstance()->toucheEnfoncee(EvenementClavier(env, keyEvent));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_keyReleased(JNIEnv * env, jobject, jobject keyEvent)
/// Fonction qui redirige un événement lié à une touche relâchée.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] keyEvent: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_keyReleased( JNIEnv * env, jobject, jobject keyEvent)
{
	GestionnaireEvenements::obtenirInstance()->toucheRelachee(EvenementClavier(env, keyEvent));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseClicked( JNIEnv * env, jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris cliqué.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseClicked( JNIEnv * env, jobject, jobject evenementSouris)
{
	//Inutilisé.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mousePressed( JNIEnv * env, jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris enfoncé.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mousePressed( JNIEnv * env, jobject, jobject evenementSouris)
{
	GestionnaireEvenements::obtenirInstance()->sourisEnfoncee(EvenementSouris(env, evenementSouris));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseReleased( JNIEnv * env, jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris relâché.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseReleased( JNIEnv * env, jobject, jobject evenementSouris)
{
	GestionnaireEvenements::obtenirInstance()->sourisRelachee(EvenementSouris(env, evenementSouris));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseEntered( JNIEnv * env, jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris entrée.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseEntered( JNIEnv * env, jobject, jobject evenementSouris)
{
	//Inutilisé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseExited( JNIEnv * env , jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris sortie.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseExited( JNIEnv * env , jobject, jobject evenementSouris)
{
	//Inutilisé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseDragged( JNIEnv * env, jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris déplacée en enfonçant un bouton.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseDragged( JNIEnv * env, jobject, jobject evenementSouris)
{
	GestionnaireEvenements::obtenirInstance()->sourisDeplacee(EvenementSouris(env, evenementSouris));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseMoved( JNIEnv * env, jobject, jobject evenementSouris)
/// Fonction qui redirige un événement lié à un bouton de souris déplacée.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseMoved( JNIEnv * env, jobject, jobject evenementSouris)
{
	GestionnaireEvenements::obtenirInstance()->sourisDeplacee(EvenementSouris(env, evenementSouris));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseWheelMoved( JNIEnv * env, jobject, jobject mouseWheelEvent)
/// Fonction qui redirige un événement lié à un mouvement de la roulette de la souris
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] mouseWheelEvent: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_mouseWheelMoved( JNIEnv * env, jobject, jobject mouseWheelEvent)
{
	GestionnaireEvenements::obtenirInstance()->rouletteSouris(EvenementRouletteSouris(env, mouseWheelEvent));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_actionPerformed( JNIEnv * env, jobject, jobject evenementAction)
/// Fonction qui gère les événements d'action
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementAction: L'objet Java associé à l'événement
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_GestionnaireEvenements_actionPerformed( JNIEnv * env, jobject, jobject evenementAction)
{
	jclass classe = env->GetObjectClass(evenementAction);

	// Appel de getActionCommand()
	jmethodID getActionCommand = env -> GetMethodID(classe, "getActionCommand", "()Ljava/lang/String;");
	jstring chaine = (jstring)(env ->CallObjectMethod(evenementAction, getActionCommand));

	std::string chaineCpp = utilitaire::obtenirChaineISO(env,chaine);
	if(chaineCpp == "SUPPRIMER" || chaineCpp == "EDITEUR_NOUVEAU" || chaineCpp == "REINITIALISER_PARTIE")
	{
		// Si on est dans le cas de suppression et qu'il n'y a pas de sélection.
		if(chaineCpp == "SUPPRIMER"  && !FacadeModele::obtenirInstance()->possedeSelection())
			return;
		
		bool pause = FacadeModele::obtenirInstance()->estEnPause();
		FacadeModele::obtenirInstance()->modifierEnPause(true);
		//Appel de Fenetre.validerAction(chaine). Quitte la méthode si il reçoit "false".
		jclass classe = env -> FindClass("ca/polymtl/inf2990/Fenetre");
		jmethodID methode = env -> GetStaticMethodID(classe, "validerAction", "(Ljava/lang/String;)Z");
		if(!env->CallStaticBooleanMethod(classe, methode, chaine))
		{
			FacadeModele::obtenirInstance()->modifierEnPause(pause);
			return;
		}
		FacadeModele::obtenirInstance()->modifierEnPause(pause);
	}

	RepartiteurActions::obtenirInstance()->appelerMethodeAction(chaineCpp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_BarresOutils_EvenementsSelectionneurFichier_sauvegarderFichier(JNIEnv * env, jobject, jstring chaine)
/// Fonction qui se charge d'enregistrer la configuration courante.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] chaine: La chaine java contenant le nom du fichier
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_BarresOutils_EvenementsSelectionneurFichier_sauvegarderFichier(JNIEnv * env, jobject, jstring chaine)
{

	std::string nomFichier = utilitaire::obtenirChaineISO(env, chaine);
	FacadeModele::obtenirInstance() -> enregistrerTerrain(nomFichier);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void JNICALL Java_ca_polymtl_inf2990_BarresOutils_EvenementsSelectionneurFichier_chargerFichier(JNIEnv * env, jobject, jstring chaine)
/// Fonction qui se charge de charger la configuration à partir d'un fichier.
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] chaine: La chaine java contenant le nom du fichier
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_BarresOutils_EvenementsSelectionneurFichier_chargerFichier(JNIEnv * env, jobject, jstring chaine)
{
	std::string nomFichier = utilitaire::obtenirChaineISO(env, chaine);
	FacadeModele::obtenirInstance() -> chargerTerrain(nomFichier);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Fenetre_redimensionnerFenetre(JNIEnv*, jclass, jint largeur, jint hauteur)
///
/// Cette fonction doit être appelée lorsque la fenêtre est
/// redimensionnée afin d'ajuster les paramètres de la machine à états
/// d'OpenGL pour correspondre aux nouvelles dimensions de la fenêtre.
///
/// @param[in] largeur : La nouvelle largeur de la fenêtre.
/// @param[in] hauteur : La nouvelle hauteur de la fenêtre.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Fenetre_redimensionnerFenetre(JNIEnv*, jclass, jint largeur, jint hauteur)
{
	FacadeModele::obtenirInstance()->obtenirVue()->redimensionnerFenetre(
		Vecteur2i(0,0),
		Vecteur2i(largeur, hauteur)
		);
	FacadeModele::obtenirInstance()->rafraichirFenetre();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn jboolean JNICALL Java_ca_polymtl_inf2990_Fenetre_objetEstSousCurseur( JNIEnv *, jclass, jint x, jint y )
///
/// Cette fonction doit être appelée lorsqu'un menu contextuel doit être
/// affiché pour détecter si un objet est situé sous le curseur de la souris.
///
/// @param[in] x : La postion en x de la souris.
/// @param[in] y : La position en y de la souris.
///
/// @return Si un objet est situé sous le curseur de la souris.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_EtatModeEdition_objetEstSousCurseur( JNIEnv *, jclass, jint x, jint y )
{
	return FacadeModele::obtenirInstance()->pointOccupe(Vecteur2(x,y));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_JoueurVirtuel_envoyerJoueurVirtuelJNI( JNIEnv *, jobject, jobject )
///
/// Creation d'un joueur virtuel provenant de l'environnement Java
///
/// @param[in] env : L'environnement de la JVM associé au thread ayant engendré l'appel de la méthode
/// @param[in] evenementSouris: L'objet Java associé à l'événement
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_JoueurAbstrait_envoyerJNI (JNIEnv * env, jobject joueur)
{
	jclass classe = env->GetObjectClass(joueur);
	jmethodID obtenirTypeNumeral = env->GetMethodID(classe, "obtenirTypeNumeral", "()I");
	int type = env->CallIntMethod(joueur, obtenirTypeNumeral);

	if(type == JOUEUR_VIRTUEL)
	{
		SPJoueurAbstrait joueurVirtuel = SPJoueurAbstrait(new JoueurVirtuel(env, joueur));
		FacadeModele::obtenirInstance()->modifierAdversaire(joueurVirtuel);
	}
	else if(type == JOUEUR_HUMAIN)
	{
		SPJoueurAbstrait joueurHumain = SPJoueurAbstrait(new JoueurHumain(env, joueur));
		FacadeModele::obtenirInstance()->modifierAdversaire(joueurHumain);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_ModificateurProprieteNoeud_envoyerModificateurPropriete( JNIEnv *, jobject )
///
/// Methode qui envoie l'objet contenant les modifications
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jobject : l'objet contenant les attributs a assigner
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_ModificateurProprieteNoeud_envoyerModificateurPropriete( JNIEnv * env, jobject obj)
{
	FacadeModele::obtenirInstance()->visiterArbre(&VisiteurModifierProprieteNoeud(env,obj));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_obtenirJoueur(JNIEnv *, jclass, jstring)
///
/// Methode qui retourne un joueur selon le nom.
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jobject : l'objet contenant les attributs a assigner
/// @param[in] jstring : le nom du joueur
///
/// @return JNIEXPORT jobject : joueur abstrait
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_obtenirJoueur(JNIEnv * env, jclass, jstring leopard)
{
	std::string nom = utilitaire::obtenirChaineISO(env, leopard);
	SPJoueurAbstrait joueur = FacadeModele::obtenirInstance()->obtenirJoueur(nom);
	if(joueur == NULL)
		return NULL;

	if(joueur->obtenirType()==JOUEUR_VIRTUEL)//Check de joueur virtuel
	{
		SPJoueurVirtuel joueurVirtuel = dynamic_pointer_cast<JoueurVirtuel>(joueur);
		jint vitesse = (jint)(joueurVirtuel->obtenirVitesse()/500.0*100.0);
		jint probEchec = joueurVirtuel->obtenirProbabiliteEchec();	

		jclass classe = env->FindClass("ca/polymtl/inf2990/Jeu/JoueurVirtuel");
		jmethodID constructeur = env->GetMethodID(classe,"<init>","(Ljava/lang/String;II)V");
		
		return (env)->NewObject(classe,constructeur,leopard, vitesse,probEchec);
	}

	//SPJoueurHumain joueurHumain = boost::shared_polymorphic_cast<JoueurHumain,JoueurAbstrait>(joueur);
	//JoueurHumain* joueurHumain = dynamic_cast<JoueurHumain*>(joueur);
	jclass classe = env->FindClass("ca/polymtl/inf2990/Jeu/JoueurHumain");
	jmethodID constructeur = env->GetMethodID(classe,"<init>","(Ljava/lang/String;)V");

	return (env)->NewObject(classe,constructeur, leopard);
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_ajouterJoueur(JNIEnv *, jclass, jobject)
///
/// Methode qui sauvegarde un profil de joueur.
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jobject : l'objet contenant les attributs a assigner
/// @param[in] jobject : le joueur à sauvegarder
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_ajouterJoueur(JNIEnv * env, jclass, jobject joueur)
{
	jclass classe=env->GetObjectClass(joueur);
	
	// Obtention du type
	jmethodID methode = env->GetMethodID(classe, "obtenirTypeNumeral", "()I");
	jint type = (jint)env->CallObjectMethod(joueur, methode);

	if(type == 0)
	{
		SPJoueurAbstrait joueurHumain(new JoueurHumain(env, joueur));
		FacadeModele::obtenirInstance()->ajouterJoueur(joueurHumain);
	}
	else
	{
		SPJoueurAbstrait joueurVirtuel(new JoueurVirtuel(env, joueur));
		FacadeModele::obtenirInstance()->ajouterJoueur(joueurVirtuel);
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_supprimerJoueur(JNIEnv *, jclass, jstring)
///
/// Methode qui supprime un joueur selon son nom.
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jstring : le nom du joueur a supprimer
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_supprimerJoueur(JNIEnv * env, jclass, jstring jNom)
{
	std::string nom = utilitaire::obtenirChaineISO(env, jNom);
	FacadeModele::obtenirInstance()->supprimerJoueur(nom);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_obtenirJoueurs(JNIEnv * env, jclass)
///
/// Methode qui envoie au Java la liste des joueurs triés.
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jobject : l'objet contenant les attributs a assigner
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_obtenirJoueurs(JNIEnv * env, jclass)
{
	ConteneurJoueursTries listeJoueursTries;
	ConteneurJoueursTries::iterator iter;

	listeJoueursTries = FacadeModele::obtenirInstance()->obtenirListeNomsJoueurs();

	jclass classe = env->FindClass("java/util/ArrayList");
	jmethodID constructeur = env->GetMethodID(classe, "<init>", "()V");
	jmethodID inserer = env->GetMethodID(classe, "add", "(Ljava/lang/Object;)Z");
	jobject arrayList = env->NewObject(classe, constructeur);

	jstring leopard;

	for(iter = listeJoueursTries.begin(); iter != listeJoueursTries.end(); iter++)
	{
		leopard = env->NewStringUTF((*iter).c_str());
		env->CallBooleanMethod(arrayList, inserer, (jobject)leopard);
	}
	
	return arrayList;
}	


////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_BarresOutils_BarreEdition_obtenirValeursEdition(JNIEnv * env, jclass modificateur)
///
/// Methode qui envoie au Java les valeurs d'édition
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jclass : la classe à modifier
///
/// @return jobject : les valeurs d'édition
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_BarresOutils_BarreEdition_obtenirValeursEdition(JNIEnv * env, jclass modificateur)
{
	return FacadeModele::obtenirInstance()->obtenirAttributsNoeudSelectionne(env);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_obtenirTouches (JNIEnv * env, jclass)
///
/// Methode qui envoie au Java les touches de déplacements du joueur 2.
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jclass : la qui appelle cette méthode
///
/// @return jobject arrayList : les valeurs d'édition
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jintArray JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_obtenirTouches (JNIEnv * env, jclass)
{
	// Initialisations pour l'array de retourne
	jclass classe_array = env->FindClass("java/util/ArrayList");
	jmethodID constructeur = env->GetMethodID(classe_array, "<init>", "()V");
	jobject arrayList = env->NewObject(classe_array, constructeur);
	jmethodID inserer = env->GetMethodID(classe_array, "add", "(Ljava/lang/Object;)Z");

	jint tableauInt[4];	// Les jint des touches
	tableauInt[0] = ConfigScene::obtenirInstance()->obtenirToucheHaut();
	tableauInt[1] = ConfigScene::obtenirInstance()->obtenirToucheBas();
	tableauInt[2] = ConfigScene::obtenirInstance()->obtenirToucheGauche();
	tableauInt[3] = ConfigScene::obtenirInstance()->obtenirToucheDroite();
	
	jintArray tableau = env->NewIntArray(4);

	env->SetIntArrayRegion(tableau, 0, 4, tableauInt);
	return tableau;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_modifierTouches (JNIEnv * env, jclass, jint toucheHaut, jint toucheBas, jint toucheGauche, jint toucheDroite)
///
/// Methode qui modifie les touches de contrôle pour le 2e joueur.
///
/// @param[in] JNIEnv * : pointeur sur l'environnement JNI
/// @param[in] jclass : la classe appellant la méthode
/// @param[in] jint toucheHaut : la touche pour bouger le maillet vers le haut
/// @param[in] jint toucheBas : la touche pour bouger le maillet vers le bas
/// @param[in] jint toucheGauche : la touche pour bouger le maillet vers le gauche
/// @param[in] jint toucheDroite : la touche pour bouger le maillet vers le droite
///
/// @return jobject : les valeurs d'édition
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_modifierTouches (JNIEnv * env, jclass, jint toucheHaut, jint toucheBas, jint toucheGauche, jint toucheDroite)
{
	ConfigScene::obtenirInstance()->modifierToucheHaut(toucheHaut);
	ConfigScene::obtenirInstance()->modifierToucheBas(toucheBas);
	ConfigScene::obtenirInstance()->modifierToucheGauche(toucheGauche);
	ConfigScene::obtenirInstance()->modifierToucheDroite(toucheDroite);
}

////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Etats_GestionnaireEtats_genererTerrainParDefaut( JNIEnv *, jobject )
///
/// Génère un terrain par défaut si aucun n'a été défini.
///
/// @param[in] JNIEnv *
/// @param[in] jobject
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Etats_GestionnaireEtats_genererTerrainParDefaut( JNIEnv *, jobject )
{
	FacadeModele::obtenirInstance()->creerTerrainParDefaut();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_GestionnaireEtats_validerTable( JNIEnv *, jobject )
///
/// Appelle la fonction de validation de la table qui vérifie la présence de la rondelle et des deux maillets.
///
/// @param[in] JNIEnv *
/// @param[in] jobject
///
/// @return JNIEXPORT jboolean JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_GestionnaireEtats_validerTable( JNIEnv *, jobject )
{
	return FacadeModele::obtenirInstance()->verifierValiditeMap();
}

// Énumération des différents états du logiciels
enum Etats
{
	MODE_PARTIE_RAPIDE, MODE_TOURNOI, MODE_EDITION, MENU_PRINCIPAL,
	MODIFIER_JOUEUR, CHOIX_JOUEUR, CREATION_TOURNOI, CHOIX_TOURNOI,
	LISTE_PROFILS, MODE_OPTION_PARTIE_RAPIDE, MODE_OPTION_TOURNOI,
	MODE_OPTIONS, MODE_JOUER
};

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_GestionnaireEtats_changerEtat( JNIEnv *, jobject, jint )
///
/// Gère la modification d'état.
///
/// @param[in] JNIEnv *
/// @param[in] jobject
/// @param[in] jint etat : L'état dans lequel il faut aller.
///
/// @return JNIEXPORT jboolean JNICALL indique si le changement d'etat a reussi
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_GestionnaireEtats_changerEtat( JNIEnv *, jobject, jint etat)
{
	switch(etat)
	{
	case MODE_PARTIE_RAPIDE:
		return RepartiteurActions::obtenirInstance()->actionBoutonAllerModeJeu();
	case MODE_EDITION:
		return RepartiteurActions::obtenirInstance()->actionBoutonAllerModeEdition();
	case MODE_TOURNOI:
		return RepartiteurActions::obtenirInstance()->actionBoutonAllerModeTournoi();
	case MENU_PRINCIPAL:
		return RepartiteurActions::obtenirInstance()->actionBoutonAllerMenuPrincipal();
	default:
		return true;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_creerNouveauTournoi(JNIEnv *env, jclass, jobject tournoiJava)
///
/// Fait les appels pour initialiser un nouveau tournoi.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
/// @param[in] jobject tournoiJava : Le tournoir à créer.
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsJoueursJNI_creerNouveauTournoi(JNIEnv *env, jclass, jobject tournoiJava)
{
	// Obtention de la classe
	jclass classeTournoi = env->GetObjectClass(tournoiJava);

	// Obtention de l'array des joueurs et de sa taille
	jmethodID obtenirListeJoueurs = env->GetMethodID(classeTournoi, "obtenirListeDesJoueurs", "()Ljava/util/ArrayList;");
	jmethodID obtenirNbrJoueurs = env->GetMethodID(classeTournoi, "obtenirNombreDeJoueurs", "()I");
	jobject listeJoueurs = env->CallObjectMethod(tournoiJava, obtenirListeJoueurs);
	jint nbrJoueurs = env->CallIntMethod(tournoiJava, obtenirNbrJoueurs);

	jclass classeArrayJoueur = env->FindClass("java/util/ArrayList");
	jmethodID obtenirElement = env->GetMethodID(classeArrayJoueur, "get", "(I)Ljava/lang/Object;");

	// Éventuelle liste des joueurs
	JoueursParticipant joueurs;

	// Parcours de l'array et remplissage du vecteur
	for(int i = 0; i < nbrJoueurs; ++i)
	{
		jobject joueur = env->CallObjectMethod(listeJoueurs, obtenirElement, i);
		jclass classeJoueur = env->GetObjectClass(joueur);
		jmethodID obtenirType = env->GetMethodID(classeJoueur, "obtenirTypeNumeral", "()I");

		jint typeDuJoueur = env->CallIntMethod(joueur, obtenirType);

		// Allocation dynamique selon le type du joueur
		if(typeDuJoueur == 0)
			joueurs.push_back(SPJoueurAbstrait(new JoueurHumain(env, joueur) ) );
		else
		{	
			// Obtention du nom
			jmethodID obtenirNom = env->GetMethodID(classeJoueur, "obtenirNom", "()Ljava/lang/String;");
			jstring nom = (jstring)env->CallObjectMethod(joueur, obtenirNom);

			SPJoueurAbstrait jv = FacadeModele::obtenirInstance()->obtenirJoueur(utilitaire::obtenirChaineISO(env, nom));
			if(jv)
			{
				joueurs.push_back(jv);
			}
			else
				joueurs.push_back(SPJoueurAbstrait(new JoueurVirtuel(env, joueur) ) );
		}
	}
	// Obtenir le terrain du tournoi
	jmethodID obtenirTerrain = env->GetMethodID(classeTournoi, "obtenirTerrain", "()Ljava/lang/String;");
	jobject terrain=env->CallObjectMethod(tournoiJava, obtenirTerrain);
	std::string nomDuTerrain;
	if(terrain != NULL)
	{
		nomDuTerrain = utilitaire::obtenirChaineISO(env, (jstring)terrain);
		//tournoiCpp->modifierTerrain(nomDuTerrain);
	}

	// Initialisation du tournoi
	FacadeModele::obtenirInstance()->initialiserTournoi(joueurs,nomDuTerrain);
	Tournoi* tournoiCpp = FacadeModele::obtenirInstance()->obtenirTournoi();

	//obtenir le nom : 
	jmethodID obtenirNom = env->GetMethodID(classeTournoi, "obtenirNom", "()Ljava/lang/String;");
	jobject nom=env->CallObjectMethod(tournoiJava, obtenirNom);
	std::string nomDuTournoi = utilitaire::obtenirChaineISO(env, (jstring)nom);
	tournoiCpp->modifierNom(nomDuTournoi);

	

	// Enregistrement du tournoi
	FacadeModele::obtenirInstance()->enregistrerTournoi(tournoiCpp);
}


////////////////////////////////////////
// Méthodes pour la radio
////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_jouerMusiqueRadio(JNIEnv *, jclass)
///
/// Action PLAY sur la radio, tout simplement.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_jouerMusiqueRadio(JNIEnv *, jclass)
{
	if(SoundFMOD::obtenirInstance()->estEnPause())
		SoundFMOD::obtenirInstance()->togglePlaying();
	else
		SoundFMOD::obtenirInstance()->restartSong();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_arreterMusiqueRadio(JNIEnv *, jclass)
///
/// Action STOP sur la radio, soit arrêter la musique et retourner au début.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_arreterMusiqueRadio(JNIEnv *, jclass)
{
	SoundFMOD::obtenirInstance()->stop();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_pauserMusiqueRadio(JNIEnv *, jclass)
///
/// Action PAUSE sur la radio, soit arrêter la musique mais rester au même endroit.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_pauserMusiqueRadio(JNIEnv *, jclass)
{
	SoundFMOD::obtenirInstance()->togglePlaying();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_avancerChansonRadio(JNIEnv *, jclass)
///
/// Action FORWARD sur la radio, soit jouer la prochaine chanson de la playlist.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_avancerChansonRadio(JNIEnv *, jclass)
{
	SoundFMOD::obtenirInstance()->next_Song();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_reculerChansonRadio(JNIEnv *, jclass)
///
/// Action PREVIOUS sur la radio, soit jouer reculer de chanson dans la playlist.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_reculerChansonRadio(JNIEnv *, jclass)
{
	SoundFMOD::obtenirInstance()->previous_Song();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_placerVolumeRadio(JNIEnv *, jclass, jint volume)
///
/// Change le volume de la radio selon la valeur de "volume" passée en paramêtres. (Valeur entre 0 et 100)
///
/// @param[in] JNIEnv *
/// @param[in] jclass
/// @param[in] jint volume : le volume (en pourcentage 0 - 100) désiré pour la radio.
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_placerVolumeRadio(JNIEnv *, jclass, jint volume)
{
	SoundFMOD::obtenirInstance()->setPlaylistVolume(volume/100.0f);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_chargerCanal(JNIEnv *, jclass, jstring canal)
///
/// Dit a FMOD qu'on veut faire jouer le canal dans la radio.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
/// @param[in] jstring canal : le canal desire.
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_chargerCanal(JNIEnv * env, jclass, jstring canal)
{
	SoundFMOD::obtenirInstance()->modifierPlaylistActuelle(utilitaire::obtenirChaineISO(env, canal));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jstring JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_obtenirCanalCourant(JNIEnv *, jclass)
///
/// Retourne le nom du canal qui joue presentement dans la radio.
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jstring JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_obtenirCanalCourant(JNIEnv * env, jclass)
{

	 std::string message = SoundFMOD::obtenirInstance()->obtenirNomCanalCourant();
	 jstring retour = env->NewStringUTF(message.c_str());
	 return retour;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_obtenirCanaux(JNIEnv *, jclass)
///
/// Retourne la liste des canaux (playlists).
///
/// @param[in] JNIEnv * env : L'environnement Java.
/// @param[in] jclass
///
/// @return JNIEXPORT jobject JNICALL : Un arrayList des canaux
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_obtenirCanaux(JNIEnv * env, jclass)
{
	std::vector<std::string> nomsCanaux =  ConfigScene::obtenirInstance()->obtenirListeCanaux();

	jclass classe = env->FindClass("java/util/ArrayList");
	jmethodID constructeur = env->GetMethodID(classe, "<init>", "()V");
	jmethodID inserer = env->GetMethodID(classe, "add", "(Ljava/lang/Object;)Z");
	jobject arrayList = env->NewObject(classe, constructeur);

	jstring leopard;

	std::vector<std::string>::iterator iter;

	for(iter = nomsCanaux.begin(); iter != nomsCanaux.end(); iter++)
	{
		leopard = env->NewStringUTF((*iter).c_str());
		env->CallBooleanMethod(arrayList, inserer, (jobject)leopard);
	}

	return arrayList;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_obtenirCanal(JNIEnv *, jclass, jstring)
///
/// Retourne la liste des musiques d'un canal.
///
/// @param[in] JNIEnv * env : L'environnement Java.
/// @param[in] jclass
/// @param[in] jstring nomCanal : le nom du canal dont ont veut les musiques.
///
/// @return JNIEXPORT jobject JNICALL : Le canal.
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_obtenirCanal(JNIEnv * env, jclass, jstring nomCanal)
{
	std::string nom = utilitaire::obtenirChaineISO(env, nomCanal);
	
	NomsPlaylists canal = ConfigScene::obtenirInstance()->obtenirCanal(nom);
	
	
	jclass classe = env->FindClass("java/util/ArrayList");
	jmethodID constructeur = env->GetMethodID(classe, "<init>", "()V");
	jmethodID inserer = env->GetMethodID(classe, "add", "(Ljava/lang/Object;)Z");
	jobject arrayList = env->NewObject(classe, constructeur);

	jstring leopard;

	NomsPlaylists::iterator iter;

	for(iter = canal.begin(); iter != canal.end(); iter++)
	{
		leopard = env->NewStringUTF((*iter).c_str());
		env->CallBooleanMethod(arrayList, inserer, (jobject)leopard);
	}	

	jclass classe2 = env->FindClass("ca/polymtl/inf2990/Jeu/CanalRadio");
	jmethodID constructeur2 = env->GetMethodID(classe2,"<init>","()V");
	jmethodID assignerNom = env->GetMethodID(classe2, "assignerNomDuCanal", "(Ljava/lang/String;)V");
	jmethodID assignerCanal = env->GetMethodID(classe2, "assignerListeDesChansons", "(Ljava/util/ArrayList;)V");
	jobject valeurDeRetour = env->NewObject(classe2, constructeur2);
	env->CallVoidMethod(valeurDeRetour, assignerNom, nomCanal);
	env->CallVoidMethod(valeurDeRetour, assignerCanal, arrayList);

	return valeurDeRetour;

}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_supprimerCanal(JNIEnv *, jclass, jstring)
///
/// Supprime un canal
///
/// @param[in] JNIEnv * env : L'environnement Java.
/// @param[in] jclass
/// @param[in] jstring nomCanal : le nom du canal à supprimer.
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_supprimerCanal(JNIEnv * env, jclass, jstring nomCanal)
{
	std::string nom = utilitaire::obtenirChaineISO(env, nomCanal);
	ConfigScene::obtenirInstance()->supprimerCanal(nom);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_genererCanal(JNIEnv *env, jclass, jobject canal)
///
/// Genere le canal
///
/// @param[in] JNIEnv * env : L'environnement Java.
/// @param[in] jclass
/// @param[in] jstring nomCanal : le nom du canal à supprimer.
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationsRadioJNI_genererCanal(JNIEnv *env, jclass, jobject canal)
{
	jclass classeCanal = env->GetObjectClass(canal);
	jmethodID obtenirNomCanal = env->GetMethodID(classeCanal, "obtenirNomDuCanal", "()Ljava/lang/String;");
	jstring jnom = (jstring)env->CallObjectMethod(canal, obtenirNomCanal);

	std::string nom = utilitaire::obtenirChaineISO(env, jnom);

	ConfigScene::obtenirInstance()->supprimerCanal(nom);
	ConfigScene::obtenirInstance()->ajouterCanal(nom);

	jmethodID obtenirChansons = env->GetMethodID(classeCanal, "obtenirListeDesChansons", "()Ljava/util/ArrayList;");
	jobject arrayDeChansons = env->CallObjectMethod(canal, obtenirChansons);
	jclass classeArray = env->GetObjectClass(arrayDeChansons);
	jmethodID getSize = env->GetMethodID(classeArray,"size","()I");
	jint size=env->CallIntMethod(arrayDeChansons,getSize);
	
	jmethodID get = env->GetMethodID(classeArray, "get", "(I)Ljava/lang/Object;");
	for(int i=0;i<size;i++)
	{
		std::string pathChanson = utilitaire::obtenirChaineISO(env, (jstring)env->CallObjectMethod(arrayDeChansons, get, i));
		ConfigScene::obtenirInstance()->ajouterChanson(nom, pathChanson);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_EtatModeEdition_objetEstSelectionne( JNIEnv *, jclass )
///
/// Permet de savoir s'il y a un objet de selectionner dans l'arbre de rendu principal
///
/// @param[in] JNIEnv *
/// @param[in] jclass
///
/// @return JNIEXPORT jboolean JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_EtatModeEdition_objetEstSelectionne (JNIEnv *, jclass)
{
	return FacadeModele::obtenirInstance()->possedeSelection();
}


//////////////////////////////////////////////////////////////////////
//																	//
// Section OperationTournoiJNI										//
//																	//
//////////////////////////////////////////////////////////////////////


/*
 * Class:     ca_polymtl_inf2990_Jeu_OperationTournoiJNI
 * Method:    obtenirTournoi
 * Signature: (Ljava/lang/String;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_ca_polymtl_inf2990_Jeu_OperationTournoiJNI_obtenirTournoi(JNIEnv* env, jclass, jstring nomTournoi)
{
	std::string nomT = utilitaire::obtenirChaineISO(env, nomTournoi);
	FacadeModele::obtenirInstance()->chargerTournoi("tournoi/"+nomT+".xml");
	Tournoi* tournoi = FacadeModele::obtenirInstance()->obtenirTournoi();

	

	jclass jTournoi = env->FindClass("ca/polymtl/inf2990/Jeu/Tournoi");
	jmethodID constructeur = env->GetMethodID(jTournoi, "<init>", "()V");
	jobject tournoiJava = env -> NewObject(jTournoi, constructeur);

	if(!tournoi)
	{
		return 0;
	}

	// placer le nom
	jmethodID setName = env->GetMethodID(jTournoi, "setNom", "(Ljava/lang/String;)V");
	env->CallVoidMethod(tournoiJava, setName, nomTournoi);

	// placer la liste des vainqueurs
	Tournoi::ListeGagnants listeGagnants = tournoi->obtenirListeGagnants();
	jclass arrayListClass = env->FindClass("java/util/ArrayList");
	jmethodID add = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
	jmethodID constructeurArrayList = env -> GetMethodID(arrayListClass, "<init>", "()V");
	//on cree la arraylist java.
	jobject arraylist = env->NewObject(arrayListClass, constructeurArrayList);

	for(int i=0;i<listeGagnants.size(); ++i)
	{
		jstring nomGagnant = env->NewStringUTF(listeGagnants[i].c_str());
		env->CallBooleanMethod(arraylist, add, nomGagnant);
	}
	
	jmethodID modifierListeGagnants = env->GetMethodID(jTournoi, "modifierVainqueurs", "(Ljava/util/ArrayList;)V");
	env->CallVoidMethod(tournoiJava, modifierListeGagnants, arraylist);
	// placer les points de la table
	FacadeModele::obtenirInstance()->chargerTerrain(tournoi->obtenirTerrain());
	NoeudTable* table=FacadeModele::obtenirInstance()->obtenirTerrain()->obtenirTable();

	jclass point = env->FindClass("java/awt/Point");
	jmethodID creerPoint=env->GetMethodID(point,"<init>","()V");
	jmethodID setLocation=env->GetMethodID(point,"setLocation", "(DD)V");

	jobject arrayPoints=env->NewObject(arrayListClass, constructeurArrayList);
	for(int i=0;i<8;++i)
	{
		jobject p=env->NewObject(point,creerPoint);

		NoeudPoint* pp=table->obtenirPoint(i);
		Vecteur3 vv=pp->obtenirPositionAbsolue();
		
		env->CallVoidMethod(p, setLocation, vv[VX], vv[VY]);
		env->CallBooleanMethod(arrayPoints, add, p);
	}

	jmethodID modifierPointsTable=env->GetMethodID(jTournoi, "modifierPointsTable", "(Ljava/util/ArrayList;)V");
	env->CallVoidMethod(tournoiJava, modifierPointsTable, arrayPoints);
	
	/////////////////////////////////////////
	//obtenir la liste des joueurs initiaux.
	/////////////////////////////////////////
	//1) creer une arraylist vide.
	jobject arraylistJoueurs = env->NewObject(arrayListClass, constructeurArrayList);
	//2) populer le arraylist avec le nom des joueurs initiaux.
	JoueursParticipant listeJoueursCpp = tournoi->obtenirListeJoueursInitiaux();


	for(int i=0;i<listeJoueursCpp.size(); ++i)
	{
		jstring nomJoueur = env->NewStringUTF(listeJoueursCpp[i]->obtenirNom().c_str());
		env->CallBooleanMethod(arraylistJoueurs, add, nomJoueur);
	}
	// 3) ajouter le arraylist au tournoi.
	jmethodID modifierListeJoueurs = env->GetMethodID(jTournoi,"chargerJoueurs","(Ljava/util/ArrayList;)V");
	env->CallVoidMethod(tournoiJava, modifierListeJoueurs, arraylistJoueurs);

	return tournoiJava;
}

/*
 * Class:     ca_polymtl_inf2990_Jeu_OperationTournoiJNI
 * Method:    supprimerTournoi
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Jeu_OperationTournoiJNI_supprimerTournoi(JNIEnv* env, jclass, jstring nomT)
{
	string nomfile=utilitaire::obtenirChaineISO(env, nomT);
#ifdef WIN32
	DeleteFileA(("tournoi/"+ nomfile +".xml").c_str());
#else
	// suppression d'un fichier avec un autre API, dunno which yet
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Panneaux_PanneauGestionTournoi_reinitialiserTournoi( JNIEnv *, jobject )
///
/// Permet de reinitialiser le tournoi dans le modele
///
/// @param[in] JNIEnv *
/// @param[in] jobject
///
/// @return JNIEXPORT void JNICALL
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Panneaux_PanneauGestionTournoi_reinitialiserTournoi( JNIEnv *, jobject )
{
	Tournoi* tournoi = FacadeModele::obtenirInstance()->obtenirTournoi();
	if(tournoi)
	{
		tournoi->reinitialiserTournoi();
		FacadeModele::obtenirInstance()->enregistrerTournoi(tournoi);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Panneaux_PanneauGestionTournoi_reinitialiserTournoi( JNIEnv *, jobject )
///
/// Retourne si un point de but est sélectionné
///
/// @param[in] JNIEnv *
/// @param[in] jobject
///
/// @return JNIEXPORT jboolean JNICALL Si un point de but est sélectionné
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_EtatModeEdition_pointButEstSelectionne (JNIEnv *, jclass)
{
	Terrain* terrain = FacadeModele::obtenirInstance()->obtenirTerrain();
	if(!terrain)
		return false;

	NoeudTable* table = terrain->obtenirTable();
	if(!table)
		return false;

	return (table -> obtenirPoint(POSITION_MILIEU_GAUCHE))->estSelectionne() || (table -> obtenirPoint(POSITION_MILIEU_DROITE))->estSelectionne();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JNIEXPORT void JNICALL Java_ca_polymtl_inf2990_Panneaux_PanneauGestionTournoi_reinitialiserTournoi( JNIEnv *, jobject )
///
/// Retourne si un point qui n'est pas un point de but est sélectionné
///
/// @param[in] JNIEnv *
/// @param[in] jobject
///
/// @return JNIEXPORT jboolean JNICALL Si un point qui n'est pas un point de but est sélectionné
///
////////////////////////////////////////////////////////////////////////
JNIEXPORT jboolean JNICALL Java_ca_polymtl_inf2990_Etats_EtatModeEdition_pointPasButEstSelectionne (JNIEnv *, jclass)
{
	Terrain* terrain = FacadeModele::obtenirInstance()->obtenirTerrain();
	if(!terrain)
		return false;

	NoeudTable* table = terrain->obtenirTable();
	if(!table)
		return false;

	return (table -> obtenirPoint(POSITION_HAUT_GAUCHE))->estSelectionne() 
		|| (table -> obtenirPoint(POSITION_HAUT_MILIEU))->estSelectionne()
		|| (table -> obtenirPoint(POSITION_HAUT_DROITE))->estSelectionne()
		|| (table -> obtenirPoint(POSITION_BAS_GAUCHE))->estSelectionne()
		|| (table -> obtenirPoint(POSITION_BAS_MILIEU))->estSelectionne()
		|| (table -> obtenirPoint(POSITION_BAS_DROITE))->estSelectionne();
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////









