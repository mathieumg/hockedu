//////////////////////////////////////////////////////////////////////////////
/// @file AIMaillet.cpp
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIMaillet.h"
#include "NoeudMaillet.h"
#include "AIStratDefensive.h"
#include "AIStratOffensive.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIStratOffensiveLigneDroite.h"
#include "AIStratOffensiveDroite.h"
#include "AIStratOffensiveGauche.h"
#include "FacadeModele.h"
#include "JoueurVirtuel.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  AIMaillet::AIMaillet( const JoueurVirtuel& jv )
///
/// Constructeur
///
/// @param[in] const JoueurVirtuel & jv : reference du joueurVirtuel ayant cet Intelligence Artificiel
///										la reference force cette instance a exister et donc non-Null
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AIMaillet::AIMaillet(JoueurVirtuel* jv): strategie_(0),jv_(jv)
{
    mNextStrat = NBSTRAT;
    mNextStratValidator = NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIMaillet::~AIMaillet()
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIMaillet::~AIMaillet()
{
	if(strategie_)
		delete strategie_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIMaillet::evaluerDirection( NoeudMaillet* maillet, NoeudTable* table,NoeudRondelle* rondelle )
///
/// On evalue la nouvelle direction du maillet contrôlé par joueur 
/// virtuel selon la strategie
///
/// @param[in] NoeudMaillet * maillet : pointeur sur le maillet du JV
/// @param[in] NoeudTable * table : pointeur sur la table pour savoir 
/// où sont les murets
///
/// @return Vecteur2
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIMaillet::evaluerDirection( NoeudMaillet* maillet )
{
	// A ce point, la methode evaluerStrategie a deja été appelé, donc on peut l'utiliser directement
	return strategie_->appliquerStrategie(maillet);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AIMaillet::evaluerStrategie( NoeudRondelle* rondelle, NoeudMaillet* maillet )
///
/// Évaluation de la stratégie à utiliser selon la position de la 
/// rondelle et le côté du maillet
///
/// @param[in] NoeudRondelle * rondelle : pointeur sur la rondelle
/// @param[in] NoeudMaillet * maillet : pointeur sur le maillet du jv
///
/// @return
///
////////////////////////////////////////////////////////////////////////
void AIMaillet::evaluerStrategie( NoeudMaillet* maillet )
{
	NoeudRondelle* rondelle;
	if(!maillet->getField() || !(rondelle = maillet->getField()->getPuck()))
		return;

	(rondelle->getPosition()[VX] * maillet->getPosition()[VX] >= 0) ? changerStrat(OFFENSIVE): changerStrat(DEFENSIVE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AIMaillet::changerStrat( typeStrat )
///
/// Changement de strategie
///
/// @param[in] typeStrat
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AIMaillet::changerStrat( typeStrat strat )
{
	AIStrat* nouvelleStrat = 0;
	if(!strategie_ || strategie_->obtenirType() != strat)
	{
		switch(strat)
		{
		case DEFENSIVE: nouvelleStrat = new AIStratDefensive(*this); break;
        case OFFENSIVE: nouvelleStrat = new AIStratOffensive(*this); break;
        case OFFENSIVE_LIGNE_DROITE: nouvelleStrat = new AIStratOffensiveLigneDroite(*this); break;
        case OFFENSIVE_DROITE: nouvelleStrat = new AIStratOffensiveDroite(*this); break;
		case OFFENSIVE_GAUCHE: nouvelleStrat = new AIStratOffensiveGauche(*this); break;
		}
	}
	if(nouvelleStrat)
	{
		if(strategie_)
			delete strategie_;
		strategie_ = nouvelleStrat;
	}
}


typeStrat AIMaillet::getNextStrat()
{
    typeStrat wReturn = mNextStrat;
    if(mNextStratValidator && mNextStratValidator(FacadeModele::getInstance()->obtenirPartieCourante(), jv_->getControlingMallet()))
    {
        mNextStrat = NBSTRAT;
        mNextStratValidator = NULL;
    }
    return wReturn;
}



void AIMaillet::changerStratNext( typeStrat pStrat, StratChangerValidator pStratValidator )
{
    mNextStrat = pStrat;
    mNextStratValidator = pStratValidator;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



