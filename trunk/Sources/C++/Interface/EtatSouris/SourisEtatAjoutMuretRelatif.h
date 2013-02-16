//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutMuretRelatif.h
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
/// @class SourisEtatAjoutMuretRelatif
/// @brief Classe de gestion du comportement dans le mode ajout de murets.
///
/// @author Michael Ferris
/// @date 2013-02-16
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutMuretRelatif :
	public SourisEtatAjout
{
public:
    typedef SourisEtatAjout Super;
	/// Constructeur par param�tre
	SourisEtatAjoutMuretRelatif();
    /// asks the node to give the next control point to modify
    void getNextControlPoint();
    /// Destructeur virtuel
	virtual ~SourisEtatAjoutMuretRelatif(void);

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );
	
    /// M�thode qui cree des noeuds sur le parent en parametres
    virtual void genererNoeud();

private : 
    std::vector<class NodeControlPoint*> mControlPoints;
    class NodeControlPoint* mCurrentPoint;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


