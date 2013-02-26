//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutControlPointMutable.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include "SourisEtatAjout.h"
#include <vector>

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAjoutControlPointMutable
/// @brief Classe de gestion du comportement dans le mode ajout de murets.
///
/// @author Michael Ferris
/// @date 2013-02-16
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutControlPointMutable :
	public SourisEtatAjout
{
public:
    typedef SourisEtatAjout Super;
	/// Constructeur par param�tre
	SourisEtatAjoutControlPointMutable(Terrain* pField, const std::string& typeNoeud);
    /// asks the node to give the next control point to modify
    bool createNextControlPoint();
    /// Destructeur virtuel
	virtual ~SourisEtatAjoutControlPointMutable(void);

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );
    /// M�thode qui cree des noeuds sur le parent en parametres
    virtual void genererNoeud();

private : 
    class NodeControlPoint* mCurrentPoint;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


