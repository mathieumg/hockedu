//////////////////////////////////////////////////////////////////////////////
/// @file AIStratDefensive.cpp
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratDefensive.h"
#include "Terrain.h"
#include "NoeudTable.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "PlayerComputer.h"
#include "Partie.h"
#include <iostream>




////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratDefensive::AIStratDefensive()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratDefensive::AIStratDefensive(const AIMaillet& context):AIStrat(context)
{
	// On calcule la ligne de defense sur laquelle on veut defendre

	NoeudMaillet* wMaillet = context.obtenirJv()->getControlingMallet();
	if(wMaillet)
	{
		// On obtient le but du JV
		NoeudBut* butJV;
		// joueur gauche
		int i = wMaillet->estAGauche() ? 1 : 2;
		butJV =  wMaillet->getField()->getTable()->obtenirBut(i);
		int wRand = (rand() % 30)-15;
		float wRayonMaillet = wMaillet->getRadius();
		mPosToDefend = (float) ((butJV->getPosition() + (i==1 ? (1.0f) : (-1.0f)) * Vecteur3(wRayonMaillet + (wRand), 0.0f, 0.0f))[VX]);

		mErreurY = (float) (rand() % (int)(5.0f * wRayonMaillet)) - 2.5f * wRayonMaillet;
	}
	else
	{
		mPosToDefend = 0.0f;
		mErreurY = 0.0f;
	}


}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratDefensive::~AIStratDefensive()
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratDefensive::~AIStratDefensive()
{
}
////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIStratDefensive::appliquerStrategie( NoeudMaillet* maillet, NoeudRondelle* rondelle )
///
/// On applique la strategie defensive, on retourne la direction a prendre du maillet
///
/// @param[in] NoeudMaillet * maillet : le maillet du JV
/// @param[in] NoeudRondelle * rondelle :  la rondelle
///
/// @return Vecteur2 :  la direction
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIStratDefensive::appliquerStrategie( NoeudMaillet* maillet)
{
	NoeudRondelle* rondelle;
	if(!maillet->getField() || !( rondelle = maillet->getField()->getPuck() ) )
		return Vecteur2();

	Vecteur3 wDirectionRondelleMaillet = (rondelle->getPosition() - maillet->getPosition());
	float wDistance = wDirectionRondelleMaillet.norme();
	float wThreshold = (maillet->getRadius() * 3.0f);
	if(wDistance < wThreshold)
	{
		Vecteur3 wNorm = wDirectionRondelleMaillet;
		wNorm.normaliser();
		return wNorm * (wThreshold - abs(wDistance));
	}


	float wTime = mGameTime.Elapsed_Time_sec();
    if(wTime > 3)
    {
        // Si apres 3 sec on est encore en defensive, on attaque
        const_cast<AIMaillet&>(context_).changerStratNext(OFFENSIVE, AIStrat::TesterPushPuckOnTheOtherSide); // Va changer au prochain tick
        return Vecteur2(); // On doit return car on vient d'etre delete
    }

	
	// On determine la direction a prendre entre le but et le maillet
	Vecteur2 dir= Vecteur3(mPosToDefend, mErreurY, 0) - maillet->getPosition();

	// On senligne avec la rondelle si on est deja dans la zone de but
	if (abs(dir[VX]) < maillet->getRadius()*2+10)
	{
		dir = (rondelle->getPosition() - maillet->getPosition()).convertir<2>();
		dir.normaliser();
		dir[VX] = 0.0;
	}
	else
	{
		dir.normaliser();
	}

	dir *= (float)context_.obtenirJv()->obtenirVitesse();

	return dir;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


