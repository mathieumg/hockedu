#include "AIRenforcementTest.h"
#include "NoeudRondelle.h"
#include "Partie.h"
#include "Terrain.h"
#include "NoeudMaillet.h"
#include "JoueurVirtuelRenforcement.h"
#include "JoueurVirtuel.h"
#include "AIMaillet.h"
#include "AIStratOffensiveRenforcement.h"
#include "FacadeModele.h"
#include "GameManager.h"

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
    GameManager::obtenirInstance()->addNewGame(GAME_TYPE_OFFLINE, SPJoueurAbstrait(new JoueurVirtuel("")),SPJoueurAbstrait(new JoueurVirtuelRenforcement("")), true, false, 9000);
	GameManager::obtenirInstance()->getGameReady(9000);
    FacadeModele::getInstance()->setPartieCourante(9000);
    partie = GameManager::obtenirInstance()->getGame(9000);
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
	 wMaillet->setPosition(Vecteur3(55,0,0));
	// Place la rondelle
	NoeudRondelle* wPuck = partie->getField()->getPuck();
	wPuck->setPosition(Vecteur3(1,0,0));
	wPuck->modifierVelocite(Vecteur3(150,0,0));

	// Setup strat
	auto joueurVirtuel = (std::dynamic_pointer_cast<JoueurVirtuelRenforcement>(wMaillet->obtenirJoueur()));
	AIMailletRenforcement* aimaillet = (AIMailletRenforcement*)joueurVirtuel->getAiMaillet();

	aimaillet->changerStrat(OFFENSIVE_LIGNE_DROITE);
	AIStratOffensiveRenforcement* strat = ((AIStratOffensiveRenforcement*)aimaillet->getStrategie());
	//strat->setMalletTargetPos(Vecteur2()); PAS A SETTER
	strat->setPointImpact(Vecteur2(50,0));
	strat->setPointVise(Vecteur2(-150,0));
	strat->setTimeBeforeImpact(0); // pas utilisé pour le moment
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

