///////////////////////////////////////////////////////////////////////////////
/// @file Terrain.h
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include "ZoneEdition.h"
#include <vector>

class ArbreRenduINF2990;
class ArbreNoeudLibre;
class ZoneEdition;
class NoeudTable;
class NoeudRondelle;
class TiXmlElement;
class NoeudAbstrait;
class TerrainTest;

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
class Terrain
#if BOX2D_INTEGRATED 
    : public b2ContactListener 
#endif
{
public:
	friend TerrainTest;
	Terrain();
	virtual ~Terrain();

	/// Permet d'effectuer le rendu des arbres du terrain
	void afficherTerrain(bool afficherZoneEdition = false);
	/// Permet d'animer les noeuds des arbres du terrain, la physique reste géré par FacadeModele
	void animerTerrain( const float& temps );
	/// Libère la mémoire du terrain et le retourne à son état de base
	void libererMemoire();
	/// Permet d'initialiser le terrain avec ces éléments de bases pour le terrain
	/// Permet de reintialiser en meme temps
	void initialiser(std::string nom);
	/// Permet d'initialiser le terrain avec ces éléments a partir d'un noeud XML
	bool initialiserXml( TiXmlElement* element );
	/// Remet le terrain a son etat de base
	void reinitialiser();
	/// Creation du noeud XML du Terrain
	TiXmlElement* creerNoeudXML();
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

   

	/// Fields
private:
	/// Le nom du terrain est en fait le chemin pour la sauvegarde en XML
	std::string nom_;
	
	/// Arbre de rendus du terrain
	ArbreRenduINF2990* arbreRendu_;
	ArbreRenduINF2990* arbreAffichage_;
	ArbreNoeudLibre* arbreAjoutNoeud_;

	/// Conservation d'un pointeur vers la table
	NoeudTable* table_;
	

	/// Contient la zone d'édition du terrain
	ZoneEdition zoneEdition_;
	
	/// Indique si le terrain est initialise et donc s'il contient ses elements de base
	bool initialise_;

/// Accesseurs
public:
	/// Accesseur de arbreRendu_
	inline ArbreRenduINF2990* getArbreRendu() const { return arbreRendu_; }
	/// Accesseur de zoneEdition_
	inline ZoneEdition& getZoneEdition() { return zoneEdition_; }
	/// Accesseur de nom_
	std::string getNom() const { return nom_; }
	/// Modificateur de nom_
	void modifierNom(std::string val) { nom_ = val; }
	/// Accesseur de table_
	inline NoeudTable* getTable() const { return table_; }
	/// Accesseur de la rondelle sur le terrain.
	NoeudRondelle* getRondelle() const ;
    /// Accesseur des buts, l<argument DOIT etre un array de 2 pointeur sur des but
    void getGoals(class NoeudBut** pOutGoals);

};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



