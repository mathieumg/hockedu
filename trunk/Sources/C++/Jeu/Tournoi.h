//////////////////////////////////////////////////////////////////////////////
/// @file Tournoi.h
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Partie.h"
#include "RazerGameTypeDef.h"
#include "GameManager.h"

typedef std::vector<Partie*> ConteneurPartie;
typedef std::pair<unsigned int, PositionJoueur> Vainqueur;
typedef std::vector<Vainqueur> Vainqueurs;
class TournoiTest;
///////////////////////////////////////////////////////////////////////////
/// @class Tournoi
/// @brief Classe représentant un tournoi.
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
	/// Constructeur par défaut
	Tournoi( );

	/// Destructeur
	~Tournoi(void);

	/// Nombre de parties dans le tournoi
	static const unsigned int nbrParties_ = 15;
	static const unsigned int nbrJoueurs_ = 16;

	/// Initialisation d'un tournoi
	bool initialisation(const JoueursParticipant& joueurs, const std::string& pFieldName);
	/// Initialisation d'un tournoi avec un noeud XML
	bool initialisationXML( XmlElement* element, ConteneurJoueur* profilsVirtuelsExistant = 0 );
	/// Liberation de la mémoire
	void libererMemoire();
	/// Enregistrement du tournoi dans le fichier XML
	XmlElement* creerTournoiXML() const;
	/// Méthode pour créer un arbre de noeuds XML pour l'enregistrement
	XmlElement* creerArbreXML( unsigned int index  );
	// Permet de recommencer un tournoi
	void reinitialiserTournoi();

    bool estTermine() const { return indexPartieCourante_==0 && obtenirPartie(0)->partieTerminee();}

	bool miseAJour( bool save = true );

private:
	void majCheminVainqueur();
	static const std::string tounoiNonJoue;

	/// Conteneur de parties
	int parties_[nbrParties_];

	/// Indique si le tournoi est jouer en ce moment bien present
	bool estEnCours_;

	/// Partie courante à jouer
	unsigned int indexPartieCourante_;
	
	/// Nom du terrain
	std::string mFieldName;
	/// Le nom du tournoi (et du fichier où il sera enregistré)
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

	/// Méthodes pour connaitre les position l'arbre binaire des parties adjacente (retourne -1 si n'existe pas)
	unsigned int obtenirPartieRondeSuivante(unsigned int index);
	unsigned int obtenirPartieRondePrecedente(unsigned int index, bool gauche);
	inline const std::string& GetFieldName() const {return mFieldName;}

	/// Obtient un pointeur sur la partie courante. Objet statique donc pas besoin de vérifier la validité du pointeur
	Partie* obtenirPartieCourante(){return GameManager::obtenirInstance()->getGame(parties_[indexPartieCourante_]);}
	/// Accesseurs et modificateurs
	Partie* obtenirPartie(const unsigned int& index) const {return GameManager::obtenirInstance()->getGame(parties_[index]);}
	/// Obtention de la liste des joueurs du tournoi
	JoueursParticipant obtenirListeJoueursInitiaux();
};



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


