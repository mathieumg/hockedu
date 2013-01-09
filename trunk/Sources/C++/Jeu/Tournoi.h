//////////////////////////////////////////////////////////////////////////////
/// @file Tournoi.h
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Partie.h"
#include "INF2990TypeDef.h"

typedef std::vector<Partie*> ConteneurPartie;
typedef std::pair<unsigned int, PositionJoueur> Vainqueur;
typedef std::vector<Vainqueur> Vainqueurs;
class TournoiTest;
///////////////////////////////////////////////////////////////////////////
/// @class Tournoi
/// @brief Classe repr�sentant un tournoi.
///			La premiere partie est a l'index 14 et la finale a l'index 0
///			On parcourt les parties en decrementant l'index
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class Tournoi
{

public:
	typedef std::vector<std::string> ListeGagnants;

	friend TournoiTest;
	/// Constructeur par d�faut
	Tournoi( );

	/// Destructeur
	~Tournoi(void);

	/// Nombre de parties dans le tournoi
	static const unsigned int nbrParties_ = 15;
	static const unsigned int nbrJoueurs_ = 16;

	/// Initialisation d'un tournoi
	bool initialisation(const JoueursParticipant& joueurs, const std::string terrain);
	/// Initialisation d'un tournoi avec un noeud XML
	bool initialisationXML( TiXmlElement* element, ConteneurJoueur* profilsVirtuelsExistant = 0 );
	/// Liberation de la m�moire
	void libererMemoire();
	/// Enregistrement du tournoi dans le fichier XML
	TiXmlElement* creerTournoiXML() const;
	/// M�thode pour cr�er un arbre de noeuds XML pour l'enregistrement
	TiXmlElement* creerArbreXML( unsigned int index  );
	// Permet de recommencer un tournoi
	void reinitialiserTournoi();

	bool estTermine() const { return indexPartieCourante_==0 && parties_[0].partieTerminee();}

	bool miseAJour( bool save = true );

private:
	void modifierTerrain(const std::string terrain);
	void majCheminVainqueur();
	static const std::string tounoiNonJoue;

	/// Conteneur de parties
	Partie parties_[nbrParties_];

	/// Indique si le tournoi est jouer en ce moment bien present
	bool estEnCours_;

	/// Partie courante � jouer
	unsigned int indexPartieCourante_;
	
	/// Nom du terrain
	std::string terrain_;
	/// Le nom du tournoi (et du fichier o� il sera enregistr�)
	std::string nom_;
	ListeGagnants listeGagnants_;
	Vainqueurs cheminDernierVainqueur_;

/// Accesseurs
public:
	/// Accesseur de cheminDernierVainqueur_
	Vainqueurs obtenirCheminDernierVainqueur() const { return cheminDernierVainqueur_; }
	/// Accesseur de indexPartieCourante_
	unsigned int obtenirIndexPartieCourante() const { return indexPartieCourante_; }
	/// Modificateur de indexPartieCourante_
	inline void passerProchainePartie() { majCheminVainqueur(); if(indexPartieCourante_>0) --indexPartieCourante_;}
	/// Accesseur de tournoiEnCours_
	inline bool estEnCours() const { return estEnCours_; }
	/// Modificateur de tournoiEnCours_
	inline void modifierEstEnCours(bool val) { estEnCours_ = val; }

	/// Permet d'obtenir la liste des gagnants de ce tournoi dans le passe
	ListeGagnants obtenirListeGagnants(){return listeGagnants_;	}

	/// Accesseur de nom
	std::string obtenirNom() const;
	/// Modificateur de nom
	void modifierNom(const std::string nom);

	/// M�thodes pour connaitre les position l'arbre binaire des parties adjacente (retourne -1 si n'existe pas)
	unsigned int obtenirPartieRondeSuivante(unsigned int index);
	unsigned int obtenirPartieRondePrecedente(unsigned int index, bool gauche);
	std::string obtenirTerrain() const;

	/// Obtient un pointeur sur la partie courante. Objet statique donc pas besoin de v�rifier la validit� du pointeur
	Partie* obtenirPartieCourante(){return &parties_[indexPartieCourante_];}
	/// Accesseurs et modificateurs
	Partie* obtenirPartie(const unsigned int& index) {return &parties_[index];}
	/// Obtention de la liste des joueurs du tournoi
	JoueursParticipant obtenirListeJoueursInitiaux();
};



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


