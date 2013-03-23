//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationEchelle.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "RazerGameTypeDef.h"



////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatTransformationEchelle
/// @brief Classe de gestion du comportement pour la mise a l'echelle des elements de l'arbre de rendu
///
/// @author Mathieu Parent
/// @date 2012-01-28
////////////////////////////////////////////////////////////////////////////
class SourisEtatTransformationEchelle :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatTransformationEchelle();
	/// Destructeur virtuel
	virtual ~SourisEtatTransformationEchelle(void);

	/// Comportement lorsqu'un bouton de la souris est enfoncé.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relaché.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant
	virtual NomEtatSouris obtenirNomEtatSouris();
    virtual void toucheRelachee( EvenementClavier& evenementClavier );

private:
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


