//////////////////////////////////////////////////////////////////////////////
/// @file FacadeModele.h
/// @author Michael Ferris
/// @date 2012-01-30
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __APPLICATION_FACADEMODELE_H__
#define __APPLICATION_FACADEMODELE_H__
#define GL_GLEXT_PROTOTYPES 1
#include "glew.h"
#define _WINSOCKAPI_
#include <windows.h>
#include "Vecteur.h"
#include "RazerGameTypeDef.h"
#include "RazerGameUtilities.h"

#include "GameTime.h"
#include <queue>
#include <hash_map>
#include "GameManager.h"
#include "Enum_Declarations.h"

#if WITH_JAVA
#include "jni.h"
#endif

class VisiteurNoeud;
class NoeudMaillet;
class NoeudAbstrait;
class NoeudRondelle;
class NoeudAffichage;
class RazerGameTree;
class ArbreNoeudLibre;
class GestionnaireEvenementsTest;
class Tournoi;
class Partie;
class Terrain;
class HUDElement;

typedef int PartieId;

namespace utilitaire
{
    class BoiteEnvironnement;
}

namespace vue {
	class Vue;
    class Camera;
}

struct RenderWorkerData
{
    Terrain** terrain;
    bool* enjeu;
    HDC dc;
    HGLRC glrc;
};

DWORD WINAPI RenderSceneWorker(LPVOID arg);

///////////////////////////////////////////////////////////////////////////
/// @class FacadeModele
/// @brief Classe qui constitue une interface (une façade) sur l'ensemble
///        du modèle et des classes qui le composent.
///
/// @author Martin Bisson
/// @date 2007-02-20
///////////////////////////////////////////////////////////////////////////
class FacadeModele
{
public:

	/// Pour les test unitaires
	friend GestionnaireEvenementsTest;
    friend AIRenforcementTest;
    //friend __declspec(dllexport) void startLearningAI(char* pReinforcementProfileName, int pSpeed, int pFailProb);

	/// Obtient l'instance unique de la classe.
	static FacadeModele* getInstance();
	/// Libère l'instance unique de la classe.
	static void libererInstance();
    /// Checks if the instance exists
    static bool Exists(){return !!instance_;}

	/// Crée un contexte OpenGL et initialise celui-ci.
	void initialiserOpenGL(HWND hWnd);

    /// Initialise le context OpenGl courant
    void InitOpenGLContext();

	/// Charge les joueurs à partir d'un fichier XML.
	void chargerJoueurs( const std::string& nomFichier = "", ConteneurJoueur* joueurs = 0 );
	/// Enregistre les joueurs dans un fichier XML.
	void enregistrerJoueurs( const std::string& nomFichier = "", ConteneurJoueur* joueurs = 0 );
	/// Chargement d'un tournoi a partir d'un fichier XML
	void chargerTournoi(std::string nomFichier);
	/// Enregistrement d'un tournoi
	void enregistrerTournoi( Tournoi* tournoi = 0 );
	/// Libère le contexte OpenGL.
	void libererOpenGL();
    /// Utilities for shaders activation
    void ActivateShaders();
    void DeActivateShaders();

	/// Affiche le contenu du modèle.
	void afficher();
    /// Affiche le rectangle elastique
    /// Le dessin du rectangle elastique doit ce faire apres le swapbuffer sinon on ne le voit pas
    void DrawSelectionRectangle() const;
    /// Indique qu'il est temps de faire un rendu
    void SignalRender();
	/// Indique que la fenêtre doit être réaffichée sans le faire directement.
	void rafraichirFenetre() const;

	/// Initialisation de la vue par défaut
	void initialiserVue();
	/// Application de la projection et la camera
	void appliquerVue(int quelViewport = 1);
	/// Réinitialise la scène.
	void reinitialiserTerrain();

	/// Anime la scène.
	void animer( const float& temps);

	/// Change le fond de la fenêtre.
	void changerFond(int r, int g, int b, float a);

	/// Déplace la fenêtre
	void deplacerSouris(Vecteur2i deplacement);

	/// Déplace la fenêtre
	void deplacerFleches(Vecteur2i deplacement);

	/// Effectue un orbite de la camera
	void orbit(Vecteur2i deplacement);

	/// Fait un zoom sur la zone d'édition
	void zoom(int nbCoches);

	/// Zoom doux pour le zoom proportionnel a la souris
	void zoomDoux(int coches);

	/// Fait le rendu du rectangle de selection pour le zoom elastique
	void dessinerRectangleSelection( const Vecteur2i& coin1, const Vecteur2i& coin2 );
	/// Assignation des variables pour le zoom elastique
	void modifierVariableZoomElastique(bool actif, Vecteur2i coin1 = Vecteur2i(), Vecteur2i coin2 = Vecteur2i());

	/// Changements d'états
	bool passageModeEdition(bool pLoadDefaultXML = true);
	bool passageModeJeu(); 
	bool passageModeTournoi();
	bool passageMenuPrincipal();
    bool passageModeSimulation();
    /// Ends tournament and clear current game's memory
    void ClearCurrentGame();

	/// Launch a visitor on the field
    void acceptVisitor(VisiteurNoeud& visiteur);
    /// duplicate nodes selected that can be duplicated
    void duplicateSelection();
    /// retrieves node selected on the field
    void getSelectedNodes(ConteneurNoeuds& pSelectedNodes) const;

	/// Permet la convertion de coordonnes cloture en coordonnes virtuelles
	bool convertirClotureAVirtuelle(int x, int y, Vecteur3& point) const;
	/// Permet la conversion d'une distance de cloture a virtuelle
	Vecteur2 convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const;

	/// Permet de selectionner ou de deselectionner l'arbre de rendu en entier
	void selectionArbre(bool selectionner);
	/// Verifie si la position du noeud est valide (dans l'aire de jeu et pas sur un autre noeud)
	bool positionNoeudValide(NoeudAbstrait* noeudAVerifier);

#if WITH_JAVA  
	/// Retourne les informations sur le noeud selectionne
	jobject obtenirAttributsNoeudSelectionne(JNIEnv* env);
#endif

	/// Verifie si la position n'entre pas en collision
	bool validerPositionNoeud(NoeudAbstrait* noeudAValider, bool flag = false);
	/// Verifie s'il y a un objet selectionnable sous cette position en pixel
	bool pointOccupe(Vecteur2 positionSouris);
	/// Retourne true si un noeud de l'arbre de rendu est selectionne
	bool possedeSelection();
	/// Vérification pour voir si un noeud est dans les limites de la zone edition
	bool insideLimits(NoeudAbstrait* noeud);

	/// Ajout d'un joueur
	void ajouterJoueur(SPJoueurAbstrait joueur);
	/// Suppression d'un joueur
	void supprimerJoueur(std::string nom);
	/// Verifier si la map est valide pour jouer, soit la presence de 2 maillet sur des parties oppose de la map et une rondelle
	bool verifierValiditeMap( Terrain*terrain= 0 );
	/// Création d'un terrain par défaut
    void creerTerrainParDefaut();

	/// Permet de creer le tournoi avec une liste de joueurs y participants
	void initialiserTournoi(JoueursParticipant joueurs, std::string terrain);

	/// Permet de reinitialiser la partie courante
	void reinitialiserPartie();
    /// Reinitialise la rondelle a son etat original
    void reinitialiserRondelle();
	/// Reset des flags de l'arbre de rendu
	void resetHighlightFlags();

	void modifierVue(vue::Vue* nouvelleVue);

	NoeudAffichage* obtenirDecompte();

    /// construit le HUD
    void construireHUD();

    /// Modification de l'adversaire
    void modifierAdversaire(SPJoueurAbstrait val);

    // Password pour la prochaine game en network
    inline const std::string& getGameCreationPassword() const { return mGameCreationPassword; }
    inline void setGameCreationPassword(const std::string& val) { mGameCreationPassword = val; }

    /// Fonction dinitialisation des shaders
    void initialiserNuanceurs();
    void afficherProgramInfoLog( GLuint obj, const char* message );
    void afficherShaderInfoLog( GLuint obj, const char* message );

    /// Nom du fichier XML dans lequel doit se trouver le terrain par defaut
    static const std::string FICHIER_TERRAIN_EN_COURS;

    /// Nom du fichier XML dans lequel doit se trouver les joueurs.
    static const std::string FICHIER_JOUEURS;

    inline void resetCurrentZoom(){mCurrentZoom = 0;}

private:
   /// Constructeur par défaut.
   FacadeModele();
   /// Destructeur.
   ~FacadeModele();
   /// Constructeur copie.  Déclaré mais non défini pour éviter qu'il soit
   /// généré par le compilateur.
   FacadeModele(const FacadeModele&);
   /// Opérateur d'assignation.  Déclaré mais non défini pour éviter qu'il
   /// soit généré par le compilateur.
   FacadeModele& operator =(const FacadeModele&);
   /// Liberation de la mémoire
   void libererMemoire();


	/// Pointeur vers l'instance unique de la classe.
	static FacadeModele* instance_;

	/// Poignée ("handle") vers la fenêtre où l'affichage se fait.
	HWND  hWnd_;
	/// Poignée ("handle") vers le contexte OpenGL.
	HGLRC hGLRC_;
	HDC   hDC_;
    HANDLE renderThread_;


	/// Vue courante de la scène.
	vue::Vue* vue_;	
	/// Terrain utilisé pour le mode édition, un seul nécessaire dans FacadeModèle, car il
    /// ne peut avoir plusieurs terrain en édition en même temps.
	Terrain* mEditionField;

	/// Variable pour le rendu du rectangle de zoom elastique
	bool zoomElastique_;
	/// Variables pour conserver les coins du zoom élastique.
	Vecteur2i coinElastique1_;
	Vecteur2i coinElastique2_;

    // Zoom limits
    int mCurrentZoom;
    int mMinZoom;
    int mMaxZoom;

	/// Joueurs possibles
	ConteneurJoueur profilsVirtuels_;
	/// Tournoi courant
	Tournoi* tournoi_;
	/// Chemin vers le fichier contenant le tournoi courant
	std::string cheminTournoi_;

	/// Pointeur sur la partie courante
	int partieCourante_;
    void setPartieCourante(int pPartieCouranteId) {partieCourante_ = pPartieCouranteId;}

    /// Pointeur sur la prochaine partie
    int prochainePartie_;

    /// Password temporaire pour la creation d'une partie en reseau
    std::string mGameCreationPassword;
    

    //std::hash_map<PartieId,Partie*> mGames;

	/// Objet contenant le temps ecouler en temps reel
	//GameTime temps_;
    
	static int anglePause_;
	//SPJoueurAbstrait adversaire_; (maintenant dans le GameManager)
	utilitaire::BoiteEnvironnement* boiteEnvironnement;

    // Map to play with
    std::string mCurrentMapFile;

	// Lien vers le programme de SHADER
	GLuint progPhong_;

	/// Accesseurs
public:
    /// Accessors of boiteEnvironnement
    inline utilitaire::BoiteEnvironnement* getBoiteEnvironnement() const { return boiteEnvironnement; }

    /// Accessors of hGLRC_
    HGLRC GetHGLRC() const { return hGLRC_; }
	/// Accesseur de hDC_
	HDC obtenirHDC() const { return hDC_; }
	/// Retourne la vue courante.
	inline vue::Vue* obtenirVue();

	/// Accesseur de mFieldName
    inline Terrain* getEditionField() const { return mEditionField; }

	/// Application de la mise a l'echelle sur les noeuds selectionnes de l'arbre
	void modifierDeplacement(Vecteur2 deplacement);
	void modifierRotation( float angle );
	/// retourne le type des noeuds selectionné
	std::string obtenirTypeNoeudSelectionne();
	/// Obtention d'un joueur
	SPJoueurAbstrait obtenirJoueur(std::string nom);
	/// Obtention de la liste triées des joueurs
	ConteneurJoueursTries obtenirListeNomsJoueurs();

	/// Accesseurs des maillets
	NoeudMaillet* obtenirMailletJoueurGauche() const;
	NoeudMaillet* obtenirMailletJoueurDroit() const;

	/// Obtention du tournoi en cours
	Tournoi* obtenirTournoi();

	/// Obtention de la largeur de la zone d'edition
	float getTableWidth();

	/// Accesseur pour la partie courante
	Partie* obtenirPartieCourante() const { return GameManager::obtenirInstance()->getGame(partieCourante_); }
    /// Accesseur de partieCourante_ (id de la partie)
    int obtenirPartieCouranteId() const { return partieCourante_; }
	/// Fonction changer l'etat de pause
	void togglePause();
	/// Modificateur de pause de la partie
	void modifierEnPause(bool val);
	/// Indique si la partie est en pause
	bool estEnPause() const;

	//racine de laquelle debuter le tracage du HUD.
	HUDElement *racineHUD_;
	/// Accesseur de rondelle_
	NoeudRondelle* obtenirRondelle() const;
	/// Accesseur de nbNoeudSelect_
	unsigned int obtenirNbNoeudSelect();

	/// Accesseurs de adversaire_
	// SPJoueurAbstrait obtenirAdversaire() { return adversaire_; }
    // (maintenant dans GameManager)

    inline std::string getCurrentMap() 
    {
        if(mCurrentMapFile.size())return mCurrentMapFile;
        return FICHIER_TERRAIN_EN_COURS;
    }
    inline void setCurrentMap(std::string pCurrentMap) {mCurrentMapFile = pCurrentMap;}

    inline void setProchainePartie(int val) { prochainePartie_ = val; }

};




////////////////////////////////////////////////////////////////////////
///
/// @fn inline vue::Vue* FacadeModele::obtenirVue()
///
/// Cette fonction retourne la vue qui est présentement utilisée pour
/// voir la scène.
///
/// @return La vue courante.
///
////////////////////////////////////////////////////////////////////////
inline vue::Vue* FacadeModele::obtenirVue()
{
	if(vue_ == 0)
		initialiserVue();
	return vue_;
}





#endif // __APPLICATION_FACADEMODELE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
