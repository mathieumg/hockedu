#pragma once
#include "SourisEtatAbstrait.h"
#include <string>

class NoeudAbstrait;
class HUDTexte;

class SourisEtatAjout :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par param�tre
	SourisEtatAjout(const std::string& nomNoeudAjout);
	/// Destructeur virtuel
	virtual ~SourisEtatAjout(void);

	/// M�thode qui cree des noeuds sur le parent en parametres
	virtual void genererNoeud();

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'�tat courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();

protected : 
	/// Pointeur vers le noeud en train d'�tre ajout�
	NoeudAbstrait* noeud_;
	HUDTexte* hudTextPosInvalide;

    void showInvalidText(bool show)const;

private:
	std::string nom_;
    /// Pointeur sur le terrain pr�sentement modifi�
    class Terrain* mTerrain;

};