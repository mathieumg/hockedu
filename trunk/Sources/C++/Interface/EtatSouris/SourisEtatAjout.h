#pragma once
#include "SourisEtatAbstrait.h"
#include <string>

class NoeudAbstrait;
class HUDTexte;

class SourisEtatAjout :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par paramètre
	SourisEtatAjout(const std::string& nomNoeudAjout);
	/// Destructeur virtuel
	virtual ~SourisEtatAjout(void);

	/// Méthode qui cree des noeuds sur le parent en parametres
	virtual void genererNoeud();

	/// Comportement lorsqu'un bouton de la souris est enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();

protected : 
	/// Pointeur vers le noeud en train d'être ajouté
	NoeudAbstrait* noeud_;
	HUDTexte* hudTextPosInvalide;

    void showInvalidText(bool show)const;

private:
	std::string nom_;
    /// Pointeur sur le terrain présentement modifié
    class Terrain* mTerrain;

};