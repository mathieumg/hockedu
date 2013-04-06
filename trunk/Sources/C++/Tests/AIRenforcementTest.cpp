#include "AIRenforcementTest.h"
#include "NoeudRondelle.h"
#include "Partie.h"
#include "Terrain.h"
#include "NoeudMaillet.h"
#include "JoueurVirtuelRenforcement.h"
#include "JoueurVirtuel.h"
#include "AIMaillet.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( AIRenforcementTest );
////////////////////////////////////////////////////////////////////////
///
/// @fn void AIRenforcementTest::setUp()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AIRenforcementTest::setUp()
{
	partie = new Partie(GAME_TYPE_OFFLINE,SPJoueurAbstrait(new JoueurVirtuel("")),SPJoueurAbstrait(new JoueurVirtuelRenforcement("")));
	//joueur = partie->obtenirJoueurDroit();
	// Creation du terrain
	partie->getField()->creerTerrainParDefaut("");
	partie->getReadyToPlay();
	//partie->modifierJoueurDroit(SPJoueurAbstrait(new JoueurVirtuelRenforcement("")));
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void AIRenforcementTest::tearDown()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AIRenforcementTest::tearDown()
{
	delete partie;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void AIRenforcementTest::testDirectionPhaseDeplacement()
///
/// Test de la direction lorsque la rondelle traverse la ligne du milieu, s'il commence à se déplacer
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AIRenforcementTest::testDirectionPhaseDeplacement()
{
	
	// Place le maillet
	 NoeudMaillet* wMaillet = partie->getField()->getRightMallet();
	 wMaillet->setPosition(Vecteur3(150,0,0));
	// Place la rondelle
	NoeudRondelle* wPuck = partie->getField()->getPuck();
	wPuck->setPosition(Vecteur3(1,0,0));
	wPuck->modifierVelocite(Vecteur3(150,0,0));

	// Setup strat
	auto joueurVirtuel = (std::dynamic_pointer_cast<JoueurVirtuelRenforcement>(wMaillet->obtenirJoueur()));
	AIMailletRenforcement* aimaillet = (AIMailletRenforcement*)joueurVirtuel->getAiMaillet();

	aimaillet->changerStrat(OFFENSIVE_LIGNE_DROITE);
	//aimaillet-
	joueurVirtuel->obtenirDirectionAI(wMaillet);
	// Set la strategie
	CPPUNIT_ASSERT(wMaillet->obtenirVelocite()!=Vecteur3());
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void AIRenforcementTest::testDirectionPhaseAttente()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AIRenforcementTest::testDirectionPhaseAttente()
{

}
////////////////////////////////////////////////////////////////////////
///
/// @fn void AIRenforcementTest::testDirectionPhaseAttaque()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AIRenforcementTest::testDirectionPhaseAttaque()
{

}

