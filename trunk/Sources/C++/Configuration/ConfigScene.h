//////////////////////////////////////////////////////////////////////////////
/// @file ConfigScene.h
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __CONFIGURATION_CONFIGSCENE_H__
#define __CONFIGURATION_CONFIGSCENE_H__

#include "Singleton.h"
#include "RazerGameTypeDef.h"
#include <vector>
#include "XMLUtils.h"

class Tournoi;
class Terrain;
class RazerGameTree;
class LumiereAbstraite;

typedef std::vector<std::string> NomsPlaylists;
///////////////////////////////////////////////////////////////////////////
/// @class ConfigScene
/// @brief Les variables de configuration de la classe CScene.
///        C'est une classe singleton.
///
/// @author Michael Ferris
/// @date 2012-02-02
///////////////////////////////////////////////////////////////////////////
class ConfigScene : public Singleton<ConfigScene>
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(ConfigScene);
	ConfigScene();
	~ConfigScene();
public:
	/// Créer le DOM avec les valeurs.
	void enregistrerConfiguration() const;

	/// Lire les valeurs du DOM.
	void chargerConfiguration();

	/// Lire les valeurs du DOM d'un arbre de rendu.
	void lireDOM( const XmlNode& node, RazerGameTree* arbre );
	
	/// Créer le DOM avec les valeurs d'une liste de joueur.
	void creerDOM( XmlNode& node, const ConteneurJoueur& Joueurs ) const;

	/// Lire les valeurs du DOM d'une liste de joueur.
	void lireDOM( const XmlNode& node, ConteneurJoueur& Joueurs );

	/// Creer le DOM pour un tournoi
	void creerDOM( XmlNode& node, const Tournoi& tournoi ) const;

	/// Accesseurs des touches de déplacement
	int obtenirToucheHaut() const {return toucheHaut_;}
	int obtenirToucheBas() const {return toucheBas_;}
	int obtenirToucheGauche() const {return toucheGauche_;}
	int obtenirToucheDroite() const {return toucheDroite_;}

	/// Modificateur des touches de déplacement
	void modifierToucheHaut(const int touche){toucheHaut_ = touche;enregistrerConfiguration();}
	void modifierToucheBas(const int touche){toucheBas_ = touche;enregistrerConfiguration();}
	void modifierToucheGauche(const int touche){toucheGauche_ = touche;enregistrerConfiguration();}
	void modifierToucheDroite(const int touche){toucheDroite_ = touche;enregistrerConfiguration();}

	/// Modification du fichier XML concernant les musiques
	void creerFichierMusique();
	void ajouterCanal(std::string nomCanal);
	void supprimerCanal(std::string nomCanal);
	void ajouterChanson(std::string nomCanal, std::string nomChanson);
	void supprimerChanson(std::string nomCanal, std::string nomChanson);

	/// Lecture du fichier XML de musiques
	std::vector<std::string> obtenirListeCanaux();
	NomsPlaylists obtenirCanal(std::string nom);

	/// Enregistrement des Playlists dans FMOD
	void chargerCanaux();
	//  Accesseur de la lumiere voulu
	LumiereAbstraite* obtenirLumiere(int indice) const;
	/// Rafraichi la lumière
	void rafraichirLumiere();

private:
	/// Indique les touches de déplacement pour le clavier
	int toucheHaut_;
	int toucheBas_;
	int toucheGauche_;
	int toucheDroite_;
	
	// Lumieres 0-7
	std::vector<LumiereAbstraite*> conteneurLumiere_;

public:

	/// Nom du fichier XML dans lequel doit se trouver la configuration.
	static const std::string FICHIER_CONFIGURATION;
	static const std::string FICHIER_MUSIQUE;
	static const char ETIQUETTE_ARBRE[];
};


#endif // __CONFIGURATION_CONFIGSCENE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
