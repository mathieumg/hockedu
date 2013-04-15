///////////////////////////////////////////////////////////////////////////////
/// @file Terrain.h
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once

#if BOX2D_PLAY
#include "Box2D\Dynamics\b2WorldCallbacks.h"
class b2Contact;
struct b2ContactImpulse;
struct b2Manifold;
#endif

#include <string>
#include "ZoneEdition.h"
#include <vector>
#include "RazerGameTypeDef.h"
#include "XMLUtils.h"
#include "Enum_Declarations.h"
#include <deque>
#include "FieldModificationStrategyAbstract.h"
#include "GameTime.h"


class RazerGameTree;
class ArbreNoeudLibre;
class ZoneEdition;
class NoeudTable;
class NoeudMaillet;
class NoeudRondelle;
class NoeudAbstrait;
class TerrainTest;
class Partie;
class VisiteurNoeud;

typedef std::set<NoeudAbstrait*> NodeSet;

enum PuckZone
{
    PUCK_ZONE_UNKNOWN,
    PUCK_ZONE_LEFT,
    PUCK_ZONE_RIGHT,
};


///////////////////////////////////////////////////////////////////////////
/// @class Terrain
/// @brief Objet contenant les éléments d'un terrain de jeu permettant de l'afficher,
///			le modifier et le sauvegarder.
///			Tous les noeuds contenus dans ses arbres connaissent l'existence du terrain
///			et ont un pointeur sur celui-ci qui est gardé a jour par l'assignation de son parent
///
///         Points d'entrés possibles pour avoir un terrain éditable ou jouable sont
///         initialiserXml()  createRandomField()   creerTerrainParDefaut()
///
/// @author Michael Ferris
/// @date 2012-03-19
///////////////////////////////////////////////////////////////////////////
class Terrain
#if BOX2D_PLAY
: public b2ContactListener
#endif
{
public:
	friend TerrainTest;
	Terrain(Partie* pGame);
	virtual ~Terrain();

	/// Permet d'effectuer le rendu des arbres du terrain
    void renderField();

	/// Permet d'animer les noeuds des arbres du terrain, la physique reste géré par FacadeModele
	void animerTerrain( const float& temps );

	/// Libère la mémoire du terrain et le retourne à son état de base
	void libererMemoire();

	

	/// Permet d'initialiser le terrain avec ces éléments a partir d'un noeud XML
    bool initialiserXml( const XmlElement* element, bool fromDocument = true, bool correctErrors = true );


	/// Remet le terrain a son etat de base
	void reinitialiser();

	/// Creation du noeud XML du Terrain
	XmlElement* creerNoeudXML();

	/// Ajout d'un noeud dans l'arbre de noeud Temporaire
	void ajouterNoeudTemp( NoeudAbstrait* noeud);

	/// Transfert d'un noeud de l'arbre de noeud temporaire à l'arbre de rendu
	void transfererNoeud( NoeudAbstrait* noeud);

	/// Permet de detacher un noeud temporaire de l'arbre de noeud temporaire
	void retirerNoeudTemp( NoeudAbstrait* noeud );

	/// Vérification pour voir si un noeud est dans les limites de la zone edition
	bool insideLimits( NoeudAbstrait* noeud );

	/// Création d'un terrain par défaut.
	/// Celui-ci a les dimemsions de base, 2 maillets et une rondelle
	void creerTerrainParDefaut(const std::string&);
    void createRandomField(const std::string&);

	/// Verifie si le terrain respecte les conditions de jeu
    bool verifierValidite( bool afficherErreur = true, bool deleteExternNodes = true);

	/// Applique la physique sur l'arbre de rendu contenant les noeuds de la table
	void appliquerPhysique( float temps );

    /// Updates the content of the game to be ready to play
    void fullRebuild();

    /// Show or hide the control points of the table ( they are hidden during play )
    void setTableControlPointVisible(bool pVisible);

    /// removes user selection on all nodes
    void setTableItemsSelection(bool pSelect);

    /// Indicates if any node in the logic tree is selected
    bool IsAnyNodeSelected() const;

    /// Launch a visitor on the field's logicTree
    void acceptVisitor(VisiteurNoeud& visitor);

    /// duplicate nodes selected that can be duplicated
    void duplicateSelection();

    /// gets the list of node selected
    inline const NodeSet& getSelectedNodes() const
    {
        return mSelectedNodes;
    }

    /// indicate if the node can be released at that position safely
    bool IsNodeAtValidEditionPosition(NoeudAbstrait* pNode, bool pDoHightlightNodeInCollision = false);

    /// Tente de repositionner les noeuds de facon a enlever l'overlapping
    bool FixCollidingObjects();
    /// tente de repositionner 1 noeud pour enlver l'overlapping
    bool FixCollindingNode(NoeudAbstrait* node, unsigned int nbIterations);

    /// checks if both field are the same
    bool equals(Terrain* terrain);


    float GetTableWidth()const;
    void NodeSelectionNotification( NoeudAbstrait* node, bool selectionne );

    // tells if there are selected node on the field that can be deleted
    bool CanSelectedNodeBeDeleted() const;
    // deletes node selected
    void deleteSelectedNodes();


    bool selectNodes(Vecteur2 positionMin, Vecteur2 positionMax, bool toggleSelection);
    /// checks if selected nodes are the same type and returns that type
    /// if not, return NODE_KEY_NONE
    RazerKey getSelectedNodeUniqueKey() const;
    int gatherSelectedNodeProperties(class FullProperties* properties);
    int applySelectedNodeProperties(class FullProperties* properties);

    void visitSelectedNodes(VisiteurNoeud& visitor);

    int BeginModification(FieldModificationStrategyType type, const FieldModificationStrategyEvent& beginEvent);
    int ReceiveModificationEvent(const FieldModificationStrategyEvent& pEvent);
    void cancelModification();
    int EndModification();

    /// Adds an undo state onto the stack and empties the redo stack
    void pushUndoState();
    int undoModification();
    int redoModification();
    /// cancels current modifications and reset the field to the last known correct state
    void reApplyCurrentState();

#if BOX2D_INTEGRATED
    /// Detecte si le b2Body est a un endroit ou il peut etre freely,
    /// dans le cas de collision avec des noeuds, ceux ci sont retourné dans le conteneur
    bool DetectWorldOverlapping(b2Body* pBody, std::set<NoeudAbstrait*>* pCollidingNodes = NULL);
#endif

#if BOX2D_PLAY
    /// Callback before the contact between 2 fixtures
    virtual void BeginContact( b2Contact* contact );

    /// Callback after the contact between 2 fixtures
    virtual void EndContact( b2Contact* contact );

    /// Callback before the solving the contact between 2 fixtures
    virtual void PreSolve( b2Contact* contact, const b2Manifold* oldManifold );

    /// Callback after the solving the contact between 2 fixtures
    virtual void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse );
#endif

/// Private Methods
private:
	/// Methode pour initialiser l'arbre de rendu
	void initialiserArbreRendu();

    /// Assigne les pointeurs sur les maillets et la rondelle pour la partie
    /// Lance une exception si quelque chose manque
    /// ie : mode édition => check table seulement  
    /// mode Jeu => check table, rondelle, maillets
    void initNecessaryPointersForGame();

    /// Permet d'initialiser le terrain avec ces éléments de bases pour le terrain
    /// Permet de reintialiser en meme temps
    void initialiser(std::string nom);

	/// Fields
private:
	/// Le nom du terrain est en fait le chemin pour la sauvegarde en XML
	std::string mFieldName;
	
	/// Tree containing game items
	RazerGameTree* mLogicTree;
    /// tree containing items used only to display
	class ArbreRendu* mRenderTree;
    /// tree containing temporary node for display
	ArbreNoeudLibre* mNewNodeTree;

	/// Conservation d'un pointeur vers la table
	NoeudTable* mTable;
	
    //////////////////////////////////////////////////////////////////////////
    /// Cache de pointeurs sur les maillets et rondelle durant une partie.
    /// pointeur NULL dans le mode édition
    NoeudMaillet* mLeftMallet,*mRightMallet;
    NoeudRondelle* mPuck;
    //////////////////////////////////////////////////////////////////////////

	/// Contient la zone d'édition du terrain
	ZoneEdition* mEditionZone;
	
    /// Indique si ce terrain est utilisé pour jouer ou pour l'édition
    Partie* mGame;
    bool mIsSimulation;
    bool mResizeTableModel;



    /// pointeur sur la zamboni
    class NodeModelRender* mZamboni;

    FieldModificationStrategyAbstract* mModifStrategy;

    static const int UNDO_BUFFERSIZE = 1000;
    typedef XmlElement FieldState;
    std::deque<FieldState*> mUndoBuffer;
    std::vector<FieldState*> mRedoBuffer;
    FieldState* mCurrentState;
    bool mDoingUndoRedo;


    float mBonusesMinTimeSpawn;
    float mBonusesMaxTimeSpawn;


    std::deque<class FieldRunnable*> mRunnableQueue;

    PuckZone mPuckZone;
    GameTime mZoneTimer;

    std::string mAbsolutePath;
    
#if BOX2D_INTEGRATED  
    class b2World* mWorld;
#endif
#if BOX2D_PLAY
    std::set<class ForceField*> mForceFieldActive;
    class ForceField* mLeftForceField,*mRightForceField;
#endif


#if __APPLE__
    typedef bool (*RenderObjCCallback)(RazerKey);
    /// pointer to the manager of model 3D in objc
    RenderObjCCallback mRenderObjC;
public:
    bool renderAppleNode(RazerKey key)const;
    inline void setModelManagerObjc(RenderObjCCallback pVal) { mRenderObjC = pVal; }
private:
   
#endif

    
    std::set<NoeudAbstrait*> mSelectedNodes;

    // Terrain initialized
    bool mIsInit;

    // Flag pour dire de ne pas compter de buts pour eviter que plusieurs buts soient attribuer au joueur
    bool mBesoinMiseAuJeu;
    
/// Accesseurs
public:
    inline bool getIsSimulation() const { return mIsSimulation; }
    inline void setIsSimulation(bool val) { mIsSimulation = val; }
    /// Accessors of mZamboni
    inline class NodeModelRender* getZamboni() const { return mZamboni; }
	/// Accesseur de arbreRendu_
	inline RazerGameTree* getLogicTree() const { return mLogicTree; }
	/// Accesseur de zoneEdition_
	inline ZoneEdition* getZoneEdition() { return mEditionZone; }
	/// Accesseur de nom_
    const std::string& getNom() const { return mFieldName; }
	/// Modificateur de nom_
    void modifierNom(const std::string& val) { mFieldName = val; }
	/// Accesseur de table_
	inline NoeudTable* getTable() const { return mTable; }
    inline void setTable(NoeudTable* table){mTable=table;}
	/// Accesseur de la rondelle sur le terrain.
	NoeudRondelle* getPuck() const ;
    /// Accesseur des buts, l'argument DOIT etre un array de 2 pointeur sur des but
    void getGoals(class NoeudBut** pOutGoals);

    /// Accessors of mGame
    inline const bool IsGameField() const { return !!mGame; }
    inline Partie* getGame() const {return mGame;}

    NoeudMaillet* getLeftMallet() const;
    NoeudMaillet* getRightMallet() const;

    /// Accessors of mBonusesMinTimeSpawn
    inline float getBonusesMinTimeSpawn() const { return mBonusesMinTimeSpawn; }
    void setBonusesMinTimeSpawn(const float pVal);

    /// Accessors of mBonusesMaxTimeSpawn
    inline float getBonusesMaxTimeSpawn() const { return mBonusesMaxTimeSpawn; }
    void setBonusesMaxTimeSpawn(const float pVal);

    /// Accessors of mPuckZone
    inline PuckZone getPuckZone() const { return mPuckZone; }
    void setPuckZone(const PuckZone pVal);

    /// Accessors of mResizeTableModel
    inline bool getResizeTableModel() const { return mResizeTableModel; }
    inline void setResizeTableModel( bool pVal) { mResizeTableModel = pVal; }

    inline const std::string& getAbsolutePath() const { return mAbsolutePath; }
    inline void setAbsolutePath(const std::string& val) { mAbsolutePath = val; }

#if BOX2D_INTEGRATED  
    inline class b2World* GetWorld() {return mWorld;}
#endif
    inline bool isInit() const { return mIsInit; }
public:

};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



