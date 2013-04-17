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
#include "JoueurVirtuel.h"
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

	float wTime = mGameTime.Elapsed_Time_sec();
	std::cout << "TimeDef: " << wTime << std::endl;
    if(wTime > 3)
    {
        // Si apres 3 sec on est encore en defensive, on attaque
        const_cast<AIMaillet&>(context_).changerStratNext(OFFENSIVE, AIStrat::TesterPushPuckOnTheOtherSide); // Va changer au prochain tick
        return Vecteur2(); // On doit return car on vient d'etre delete
    }

	// On obtient le but du JV
	NoeudBut* butJV;
	// joueur gauche
	int i = maillet->estAGauche() ? 1 : 2;
	butJV =  maillet->getField()->getTable()->obtenirBut(i);

	// On determine la direction a prendre entre le but et le maillet
	Vecteur2 dir= butJV->getPosition() - maillet->getPosition();

	// On senligne avec la rondelle si on est deja dans la zone de but
	if (abs(dir[VX]) < maillet->getRadius()*2+10)
	{
		dir = (rondelle->getPosition() - maillet->getPosition()).convertir<2>();
		dir.normaliser();
		dir[VX] = 0.0;
	}
	else
		dir.normaliser();

	dir *= (float)context_.obtenirJv()->obtenirVitesse()*0.75f;

	return dir;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


