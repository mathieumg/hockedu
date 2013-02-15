//////////////////////////////////////////////////////////////////////////////
/// @file Partie.h
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning( disable : 4290 )

#include "RazerGameTypeDef.h"
#include "NoeudAffichage.h"
#include "GameTime.h"
class NoeudMaillet;
class NoeudRondelle;

enum PositionJoueur{GAGNANT_GAUCHE,GAGNANT_DROITE,GAGNANT_AUCUN};

///////////////////////////////////////////////////////////////////////////
/// @class Partie
/// @brief Classe représentant une partie entre 2 joueurs (virtuels ou humains).
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class Partie
{
public:

	/// Constructeur par paramètres
	Partie(SPJoueurAbstrait joueurGauche = 0, SPJoueurAbstrait joueurDroit = 0);

	/// Destructeur
	~Partie(void);

	/// Modificateur des points des joueurs
	void incrementerPointsJoueurGauche();
	void incrementerPointsJoueurDroit();

	/// Assignation d'un joueur à la partie, S'assurer que le pointeur est unique !!!
	void assignerJoueur( SPJoueurAbstrait joueur );

	/// Creation du noeud XML de la partie
	XmlElement* creerNoeudXML() const;
	/// Initialisation d'une partie a partir d'un noeud XML
	bool initialiserXML(XmlElement* elem, ConteneurJoueur* profilsVirtuelsExistants = 0);

	/// Permet de reinitialiser la partie courante
	void reinitialiserPartie();

	/// Constante indiquant combien il faut de points pour gagner
	static const int POINTAGE_GAGNANT;

	/// Methode pour indiquer au maillet par qui ils sont controlles
	void assignerControlesMaillet(NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit, NoeudRondelle* rondelle) throw(std::logic_error);

	/// Permet de savoir si la partie est terminee
	inline bool partieTerminee() const {return pointsJoueurGauche_>=POINTAGE_GAGNANT || pointsJoueurDroit_>= POINTAGE_GAGNANT ;}

	/// Effectue une mise au jeu
	void miseAuJeu( bool debutDePartie = false );

	/// Affiche le score
	void afficherScore() const;

	/// Cree un delais d'inactivite de (time) ms
	void delais(int time);

	/// Update la minuterie
	void updateMinuterie(int time);
	
	/// Gestion de l'affichage du décompte de mise au jeu
	void afficher();
	void animer( const float& temps){chiffres_->animer(temps);}

	void vider();

	/// Termine la partie si les 2 joueur sont des AI
	bool terminerSi2AI();

	/// Retourne lobjet gameTime
	GameTime* obtenirGameTime(){return &tempsJeu_;}

/// Methode Privee
private:
	/// Modificateur de estPret_
	inline void modifierEstPret(bool val) { estPret_ = val; if(estPret_) tempsJeu_.unPause(); else tempsJeu_.pause(); }

/// Attributs
private:
	/// Les deux joueurs qui s'affrontent
	SPJoueurAbstrait joueurGauche_;
	SPJoueurAbstrait joueurDroit_;

	/// Les points des deux joueurs
	int pointsJoueurGauche_;
	int pointsJoueurDroit_;

	/// Fichier xml où est enregistré le terrain de la partie
	std::string terrain_;
	/// Indique si la partie peut debuter/continuer
	bool estPret_;
	/// Indique si la partie est en pause
	bool enPause_;

	/// Temps restant a etre inactif
	int minuterie_;

	/// Décompte de mise au jeu
	NoeudAffichage* chiffres_;

	static GLuint listePause_;

	/// Temps ecoule depuis le debut de la partie
	GameTime tempsJeu_;

	/// Indique si cette
	bool faitPartieDunTournoi_;

/// Accesseurs
public:
	/// Indique si les 2 joueurs de la partie sont virtuels
	bool partieVirtuelle();
	/// Accesseur de faitPartieDunTournoi_
	bool faitPartieDunTournoi() const { return faitPartieDunTournoi_; }
	/// Modificateur de faitPartieDunTournoi_
	void modifierFaitPartieDunTournoi(bool val) { faitPartieDunTournoi_ = val; }
	/// Modificateur de enPause_
	void modifierEnPause(bool val) { enPause_ = val; if(enPause_) tempsJeu_.pause(); else tempsJeu_.unPause(); }
	/// Accesseur de enPause_
	bool estEnPause() const { return enPause_; }
	/// Accesseurs des points des joueurs
	int obtenirPointsJoueurGauche() const;
	int obtenirPointsJoueurDroit() const;
	int obtenirReferencePointsJoueurGauche() {return pointsJoueurGauche_;}
	int obtenirReferencePointsJoueurDroit() {return pointsJoueurDroit_;}

	/// Accesseurs des noms des joueurs
	std::string obtenirNomJoueurGauche() const;
	std::string obtenirNomJoueurDroit() const;

	NoeudAffichage* obtenirDecompte() {return chiffres_;}

	/// Accesseur du joueur1
	inline SPJoueurAbstrait obtenirJoueurDroit() const { return joueurDroit_; }
	void modifierJoueurDroit(SPJoueurAbstrait val);

	/// Accesseur du joueur1
	inline SPJoueurAbstrait obtenirJoueurGauche() const { return joueurGauche_; }
	void modifierJoueurGauche(SPJoueurAbstrait val);

	/// Accesseur et modificateur pour le terrain
	std::string obtenirCheminTerrain() const;
	void setFieldName(const std::string terrain);

	/// Retourne le gagnant de la partie
	SPJoueurAbstrait obtenirGagnant() const;
	/// Permet de savoir la position du gagnant
	PositionJoueur obtenirPositionGagant();
	/// Indique si la partie est prete
	bool estPret() const {return estPret_;}

	float obtenirTempsJeu(){ return tempsJeu_.Elapsed_Time_sec(); }
    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void SignalGameOver()
    ///
    /// /*Description*/
    ///
    /// @param[in] 
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void SignalGameOver();
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


