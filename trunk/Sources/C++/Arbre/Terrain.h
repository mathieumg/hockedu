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
#include <string>
#include "ZoneEdition.h"
#include <vector>
#include "RazerGameTypeDef.h"
#include "XMLUtils.h"
#include "RunnableBreaker.h"

class RazerGameTree;
class ArbreNoeudLibre;
class ZoneEdition;
class NoeudTable;
class NoeudMaillet;
class NoeudRondelle;
class NoeudAbstrait;
class TerrainTest;
class Partie;

#if BOX2D_INTEGRATED
#include "Box2D\Dynamics\b2WorldCallbacks.h"
class b2Contact;
struct b2ContactImpulse;
struct b2Manifold;
#endif

///////////////////////////////////////////////////////////////////////////
/// @class Terrain
/// @brief Objet contenant les éléments d'un terrain de jeu permettant de l'afficher,
///			le modifier et le sauvegarder.
///			Tous les noeuds contenus dans ses arbres connaissent l'existence du terrain
///			et ont un pointeur sur celui-ci qui est gardé a jour par l'assignation de son parent
///
/// @author Michael Ferris
/// @date 2012-03-19
///////////////////////////////////////////////////////////////////////////
class Terrain: public RunnableBreaker
#if BOX2D_INTEGRATED 
    , public b2ContactListener 
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

	/// Permet d'initialiser le terrain avec ces éléments de bases pour le terrain
	/// Permet de reintialiser en meme temps
	void initialiser(std::string nom);

	/// Permet d'initialiser le terrain avec ces éléments a partir d'un noeud XML
	bool initialiserXml( XmlElement* element );

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
	void creerTerrainParDefaut(std::string);

	/// Verifie si le terrain respecte les conditions de jeu
	bool verifierValidite( bool afficherErreur = true);

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

    /// Launch a visitor on the field
    void acceptVisitor(class VisiteurNoeud& visitor);

    /// duplicate nodes selected that can be duplicated
    void duplicateSelection();

    /// gets the list of node selected
    void getSelectedNodes(ConteneurNoeuds& pSelectedNodes) const;

    /// indicate if the node can be released at that position safely
    bool IsNodeAtValidEditionPosition(NoeudAbstrait* pNode, bool pDoHightlightNodeInCollision = false);

    /// Tente de repositionner les noeuds de facon a enlever l'overlapping
    bool FixCollidingObjects();
    /// tente de repositionner 1 noeud pour enlver l'overlapping
    bool FixCollindingNode(NoeudAbstrait* node, unsigned int nbIterations);

    float GetTableWidth()const;

#if BOX2D_INTEGRATED
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
    void ecrireArbre(NoeudAbstrait* parentNoeud, const XmlNode* node);

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
	
	/// Contient la zone d'édition du terrain
	ZoneEdition* mEditionZone;
	
    /// Indique si ce terrain est utilisé pour jouer ou pour l'édition
    Partie* mGame;

#if BOX2D_INTEGRATED  
    class b2World* mWorld;
#endif

/// Accesseurs
public:
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
	/// Accesseur de la rondelle sur le terrain.
	NoeudRondelle* getPuck() const ;
    /// Accesseur des buts, l'argument DOIT etre un array de 2 pointeur sur des but
    void getGoals(class NoeudBut** pOutGoals);

    /// Accessors of mGame
    inline const bool IsGameField() const { return !!mGame; }

    NoeudMaillet* getLeftMallet() const;
    NoeudMaillet* getRightMallet() const;
#if BOX2D_INTEGRATED  
    inline class b2World* GetWorld() {return mWorld;}
#endif

public:

};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



