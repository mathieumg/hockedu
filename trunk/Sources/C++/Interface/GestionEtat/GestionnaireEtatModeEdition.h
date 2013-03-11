//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeEdition.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREETATMODEEDITION_H
#define GESTIONNAIREETATMODEEDITION_H

#include "GestionnaireEtatAbstrait.h"

class GestionnaireEvenementsTest;
class Terrain;
////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatAbstrait
/// @brief Classe qui gère les événement lors du mode édition
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatModeEdition : public GestionnaireEtatAbstrait
{
public:
    typedef GestionnaireEtatAbstrait Super;
	/// utile pour les tests.
	friend GestionnaireEvenementsTest;

	/// Constructeur par paramètre.
	GestionnaireEtatModeEdition(Terrain* pField);

    virtual void modifierEtatSouris(NomEtatSouris etatSouris);

	/// Comportement pour une touche enfoncée
	virtual void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Comportement pour une touche relachée
	virtual void toucheRelachee(EvenementClavier& evenementClavier);
	/// Comportement pour un bouton de souris enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris);
	/// Comportement pour un bouton de souris relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris);
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris);
	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);

	virtual void miseAJourEvenementsRepetitifs( float deltaTemps );
	/// Animation en mode édition
	virtual void animer( const float& temps );
	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();
private:
	bool shiftEnfonce_;
	bool enfonce_;
	int boutonEnfonce_;
	Vecteur2i positionSouris_;
    /// pointer on the field currently being modified
    Terrain* mField;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


