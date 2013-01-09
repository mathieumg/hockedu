//////////////////////////////////////////////////////////////////////////////
/// @file FacadeModele.h
/// @author Michael Ferris
/// @date 2012-01-30
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __APPLICATION_FACADEMODELE_H__
#define __APPLICATION_FACADEMODELE_H__
#define GL_GLEXT_PROTOTYPES 1
#include "glew.h"
#include <windows.h>
#include "Vecteur.h"
#include "INF2990TypeDef.h"
#include "jni.h"
#include "GameTime.h"
#include <queue>

class VisiteurNoeud;
class NoeudMaillet;
class NoeudAbstrait;
class NoeudRondelle;
class NoeudAffichage;
class ArbreRenduINF2990;
class ArbreNoeudLibre;
class GestionnaireEvenementsTest;
class Tournoi;
class Partie;
class Terrain;
class HUDElement;

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

#include <functional>
class Runnable
{
public:
    Runnable(std::function<void (Runnable*)> function, bool keepAlive = false):
      mFunction(function), mbKeepAlive(keepAlive)
      {

      }
    inline void Run()
    {
        mFunction(this);
    }

    /// Accessors of mbKeepAlive
    inline bool KeepAlive() const { return mbKeepAlive; }
    inline void setKeepAlive(bool val) { mbKeepAlive = val; }
private:
    std::function<void (Runnable*)> mFunction;
    bool mbKeepAlive;

    

};


///////////////////////////////////////////////////////////////////////////
/// @class FacadeModele
/// @brief Classe qui constitue une interface (une fa�ade) sur l'ensemble
///        du mod�le et des classes qui le composent.
///
/// @author Martin Bisson
/// @date 2007-02-20
///////////////////////////////////////////////////////////////////////////
class FacadeModele
{
public:

	/// Pour les test unitaires
	friend GestionnaireEvenementsTest;

	/// Obtient l'instance unique de la classe.
	static FacadeModele* obtenirInstance();
	/// Lib�re l'instance unique de la classe.
	static void libererInstance();

	/// Cr�e un contexte OpenGL et initialise celui-ci.
	void initialiserOpenGL(HWND hWnd);

    /// Initialise le context OpenGl courant
    void InitOpenGLContext();

    /// Charge un terrain � partir d'un fichier XML.
	void chargerTerrain( const std::string& nomFichier = "", Terrain* terrain  = 0);
	/// Enregistre un terrain dans un fichier XML.
	void enregistrerTerrain(const std::string& nomFichier = "", Terrain* terrain = 0);
	/// Charge les joueurs � partir d'un fichier XML.
	void chargerJoueurs( const std::string& nomFichier = "", ConteneurJoueur* joueurs = 0 );
	/// Enregistre les joueurs dans un fichier XML.
	void enregistrerJoueurs( const std::string& nomFichier = "", ConteneurJoueur* joueurs = 0 );
	/// Chargement d'un tournoi a partir d'un fichier XML
	void chargerTournoi(std::string nomFichier);
	/// Enregistrement d'un tournoi
	void enregistrerTournoi( Tournoi* tournoi = 0 );
	/// Lib�re le contexte OpenGL.
	void libererOpenGL();
	/// Affiche le contenu du mod�le.
	void afficher();
    /// Affiche le rectangle elastique
    /// Le dessin du rectangle elastique doit ce faire apres le swapbuffer sinon on ne le voit pas
    void DrawSelectionRectangle() const;
    /// Indique qu'il est temps de faire un rendu
    void SignalRender();
    /// Affiche la base du contenu du mod�le.
	void afficherBase() const;
	/// Indique que la fen�tre doit �tre r�affich�e sans le faire directement.
	void rafraichirFenetre() const;

	/// Initialisation de la vue par d�faut
	void initialiserVue();
	/// Application de la projection et la camera
	void appliquerVue(int quelViewport = 1);
	/// R�initialise la sc�ne.
	void reinitialiserTerrain();

	/// Anime la sc�ne.
	void animer( const float& temps);

	/// Change le fond de la fen�tre.
	void changerFond(int r, int g, int b, float a);

	/// D�place la fen�tre
	void deplacerSouris(Vecteur2i deplacement);

	/// D�place la fen�tre
	void deplacerFleches(Vecteur2i deplacement);

	/// Effectue un orbite de la camera
	void orbit(Vecteur2i deplacement);

	/// Fait un zoom sur la zone d'�dition
	void zoom(int nbCoches);

	/// Zoom doux pour le zoom proportionnel a la souris
	void zoomDoux(int coches);

	/// Fait le rendu du rectangle de selection pour le zoom elastique
	void dessinerRectangleSelection( const Vecteur2i& coin1, const Vecteur2i& coin2 );
	/// Assignation des variables pour le zoom elastique
	void modifierVariableZoomElastique(bool actif, Vecteur2i coin1 = Vecteur2i(), Vecteur2i coin2 = Vecteur2i());

	/// Changements d'�tats
	bool passageModeEdition();
	bool passageModeJeu();
	bool passageModeTournoi();
	bool passageMenuPrincipal();

	/// Effectue la selection dans l'arbre de rendu
	void visiterArbre(VisiteurNoeud* visiteur);

	/// Permet la convertion de coordonnes cloture en coordonnes virtuelles
	bool convertirClotureAVirtuelle(int x, int y, Vecteur3& point) const;
	/// Permet la conversion d'une distance de cloture a virtuelle
	Vecteur2 convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const;

	/// Permet de selectionner ou de deselectionner l'arbre de rendu en entier
	void selectionArbre(bool selectionner);
	/// Verifie si la position du noeud est valide (dans l'aire de jeu et pas sur un autre noeud)
	bool positionNoeudValide(NoeudAbstrait* noeudAVerifier);

	/// Retourne les informations sur le noeud selectionne
	jobject obtenirAttributsNoeudSelectionne(JNIEnv* env);

	/// Verifie si la position n'entre pas en collision
	bool validerPositionNoeud(NoeudAbstrait* noeudAValider, bool flag = false);
	/// Verifie s'il y a un objet selectionnable sous cette position en pixel
	bool pointOccupe(Vecteur2 positionSouris);
	/// Retourne true si un noeud de l'arbre de rendu est selectionne
	bool possedeSelection();
	/// V�rification pour voir si un noeud est dans les limites de la zone edition
	bool insideLimits(NoeudAbstrait* noeud);

	/// M�thode pour ajouter un �l�ment sur la table
	void ajouterElementSurTable(NoeudAbstrait* n);
	/// M�thode pour supprimer un �l�ment sur la table
	void supprimerElementSurTable(NoeudAbstrait* n);
	/// Algorithme pour r�tablir la table dans un �tat o� les �l�ments ne sont pas en collision
	bool ajusterElementSurTableEnCollision(const unsigned int& nbIteration = 10);
	/// Algorithme pour que les noeud en collisions se repositionne correctement
	bool ajusterElementEnCollision( NoeudAbstrait* noeud, const unsigned int& nbIterations = 10, NoeudAbstrait* racine = 0 );

	/// Ajout d'un joueur
	void ajouterJoueur(SPJoueurAbstrait joueur);
	/// Suppression d'un joueur
	void supprimerJoueur(std::string nom);
	/// Verifier si la map est valide pour jouer, soit la presence de 2 maillet sur des parties oppose de la map et une rondelle
	bool verifierValiditeMap( Terrain*terrain= 0 );
	/// Cr�ation d'un terrain par d�faut
	void creerTerrainParDefaut();

	/// Permet de creer le tournoi avec une liste de joueurs y participants
	void initialiserTournoi(JoueursParticipant joueurs, std::string terrain);

	/// Permet de reinitialiser la partie courante
	void reinitialiserPartie();
	/// Reset des flags de l'arbre de rendu
	void resetHighlightFlags();

	void modifierVue(vue::Vue* nouvelleVue);

	NoeudAffichage* obtenirDecompte();

    // va surement necessit� des mutex
    /// Permet d'ex�cuter du code sur un thread sp�cifique au moment opportun
    inline void RunOnUIThread(Runnable* run){mUIRunnables.push(run);}
    inline void RunOnUpdateThread(Runnable* run){mUpdateRunnables.push(run);}

private:
   /// Constructeur par d�faut.
   FacadeModele();
   /// Destructeur.
   ~FacadeModele();
   /// Constructeur copie.  D�clar� mais non d�fini pour �viter qu'il soit
   /// g�n�r� par le compilateur.
   FacadeModele(const FacadeModele&);
   /// Op�rateur d'assignation.  D�clar� mais non d�fini pour �viter qu'il
   /// soit g�n�r� par le compilateur.
   FacadeModele& operator =(const FacadeModele&);
   /// Liberation de la m�moire
   void libererMemoire();
   /// Nom du fichier XML dans lequel doit se trouver le terrain par defaut
   static const std::string FICHIER_TERRAIN_EN_COURS;
  
   /// Nom du fichier XML dans lequel doit se trouver les joueurs.
   static const std::string FICHIER_JOUEURS;

	/// Pointeur vers l'instance unique de la classe.
	static FacadeModele* instance_;

	/// Poign�e ("handle") vers la fen�tre o� l'affichage se fait.
	HWND  hWnd_;
	/// Poign�e ("handle") vers le contexte OpenGL.
	HGLRC hGLRC_;
	HDC   hDC_;
    HANDLE renderThread_;

    class DebugRenderBox2D* mDebugRenderBox2D;
    class b2World* mWorld;

	/// Vue courante de la sc�ne.
	vue::Vue* vue_;	
	/// Terrain contenant les arbres de rendus de la sc�ne
	Terrain* terrain_;
	/// Variable pour le rendu du rectangle de zoom elastique
	bool zoomElastique_;
	/// Variables pour conserver les coins du zoom �lastique.
	Vecteur2i coinElastique1_;
	Vecteur2i coinElastique2_;

	/// Conteneur des �l�ments modifiable sur la table
	ConteneurNoeuds elementSurTable_;
	/// Joueurs possibles
	ConteneurJoueur profilsVirtuels_;
	/// Tournoi courant
	Tournoi* tournoi_;
	/// Chemin vers le fichier contenant le tournoi courant
	std::string cheminTournoi_;

	/// Pointeur sur la partie courante
	Partie* partieCourante_;


	/// Indique si on applique la physique du jeu 
	bool enJeu_;	

	/// Objet contenant le temps ecouler en temps reel
	GameTime temps_;

	/// Nombre de noeuds selectionne depuis la derniere verification
	unsigned int nbNoeudSelect_;

	static int anglePause_;
	SPJoueurAbstrait adversaire_;
	utilitaire::BoiteEnvironnement* boiteEnvironnement;

	// Lien vers le programme de SHADER
	GLuint progPhong_;

    std::queue<Runnable*> mUIRunnables, mUpdateRunnables;

	/// Accesseurs
public:
    /// Accessors of mWorld
    inline class b2World* getWorld() const { return mWorld; }

    inline bool IsInGame() const { return enJeu_; }
    /// Accessors of hGLRC_
    HGLRC GetHGLRC() const { return hGLRC_; }
	/// Accesseur de hDC_
	HDC obtenirHDC() const { return hDC_; }
	/// Retourne la vue courante.
	inline vue::Vue* obtenirVue();
	/// Retourne l'arbre de rendu.
	inline const ArbreRenduINF2990* obtenirArbreRenduINF2990() const;
	/// Retourne l'arbre de rendu.
	inline ArbreRenduINF2990* obtenirArbreRenduINF2990();

	/// Accesseur de terrain_
	inline Terrain* obtenirTerrain() const { return terrain_; }

	/// Application de la mise a l'echelle sur les noeuds selectionnes de l'arbre
	void modifierDeplacement(Vecteur2 deplacement);
	void modifierRotation( double angle );
	/// retourne le type des noeuds selectionn�
	std::string obtenirTypeNoeudSelectionne();
	/// Obtention d'un joueur
	SPJoueurAbstrait obtenirJoueur(std::string nom);
	/// Obtention de la liste tri�es des joueurs
	ConteneurJoueursTries obtenirListeNomsJoueurs();

	/// Accesseurs des maillets
	NoeudMaillet* obtenirMailletJoueurGauche() const;
	NoeudMaillet* obtenirMailletJoueurDroit() const;

	/// Obtention du tournoi en cours
	Tournoi* obtenirTournoi();

	/// Obtention de la largeur de la zone d'edition
	double obtenirLargeurZoneEdition();

	/// Accesseur de partieCourante_
	Partie* obtenirPartieCourante() const { return partieCourante_; }
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
	unsigned int obtenirNbNoeudSelect() const { return nbNoeudSelect_; }
	/// Modificateur de nbNoeudSelect_
	void modifierNbNoeudSelect(unsigned int val) { nbNoeudSelect_ = val; }

	/// Accesseurs de adversaire_
	SPJoueurAbstrait obtenirAdversaire() { return adversaire_; }

	/// construit le HUD
	void construireHUD();

	/// Modification de l'adversaire
	void modifierAdversaire(SPJoueurAbstrait val);

	/// Fonction dinitialisation des shaders
	void initialiserNuanceurs();
	void afficherProgramInfoLog( GLuint obj, const char* message );
	void afficherShaderInfoLog( GLuint obj, const char* message );
};




////////////////////////////////////////////////////////////////////////
///
/// @fn inline vue::Vue* FacadeModele::obtenirVue()
///
/// Cette fonction retourne la vue qui est pr�sentement utilis�e pour
/// voir la sc�ne.
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
