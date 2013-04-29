//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenementsTest.cpp
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEvenementsTest.h"
#include "EventManager.h"
#include "EvenementClavier.h"
#include "EvenementSouris.h"
#include "EvenementRouletteSouris.h"
#include "CodesEvenements.h"
#include "Vecteur.h"
#include "FacadeModele.h"
#include "vue.h"
#include "ProjectionOrtho.h"
#include "NoeudMaillet.h"
#include "SourisEtatSelection.h"
#include "SourisEtatZoomProportionnel.h"
#include "RazerGameTree.h"
#include "GestionnaireEtatMenuPrincipal.h"
#include "GestionnaireEtatModeEdition.h"
#include "GestionnaireEtatModeJeu.h"
#include "GestionnaireEtatModeTournoi.h"
#include "GestionnaireEtatPartieRapideTerminee.h"
#include "GestionnaireEtatPartieTournoiTerminee.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( GestionnaireEvenementsTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::setUp()
///
/// Effectue l'initialisation préalable à l'exécution de l'ensemble des
/// cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets doivent être construits, il est conseillé de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::setUp()
{
	// L'état de base pour les tests est le mode d'édition
	EventManager::modifierEtat( ETAT_MODE_EDITION );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::tearDown()
///
/// Effectue les opérations de finalisation nécessaires suite à l'exécution
/// de l'ensemble des cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets ont été alloués à l'initialisation, ils doivent être
/// désalloués, et il est conseillé de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::tearDown()
{
	if(EventManager::etatCourant_ != 0)
		delete EventManager::etatCourant_;
	EventManager::etatCourant_ = 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenementsTest::testChangementEtat()
///
/// Cas de test: On teste un changement d'état manuel.
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testChangementEtat()
{
	// Modification de l'état
	EventManager::modifierEtat( ETAT_MENU_PRINCIPAL );
    CPPUNIT_ASSERT(!!dynamic_cast<GestionnaireEtatMenuPrincipal*>(EventManager::etatCourant_));
    EventManager::modifierEtat( ETAT_MODE_EDITION           );
    CPPUNIT_ASSERT(!!dynamic_cast<GestionnaireEtatModeEdition*>(EventManager::etatCourant_));
    EventManager::modifierEtat( ETAT_MODE_JEU               );
    CPPUNIT_ASSERT(!!dynamic_cast<GestionnaireEtatModeJeu*>(EventManager::etatCourant_));
    EventManager::modifierEtat( ETAT_MODE_TOURNOI           );
    CPPUNIT_ASSERT(!!dynamic_cast<GestionnaireEtatModeTournoi*>(EventManager::etatCourant_));
    EventManager::modifierEtat( ETAT_PARTIE_RAPIDE_TERMINEE );
    CPPUNIT_ASSERT(!!dynamic_cast<GestionnaireEtatPartieRapideTerminee*>(EventManager::etatCourant_));
    EventManager::modifierEtat( ETAT_PARTIE_TOURNOI_TERMINEE);
    CPPUNIT_ASSERT(!!dynamic_cast<GestionnaireEtatPartieTournoiTerminee*>(EventManager::etatCourant_));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenementsTest::testToucheEnfoncee()
///
/// Cas de test: Simulation d'un changement d'état par un événement du clavier
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testToucheEnfoncee()
{
	FacadeModele::getInstance()->initialiserVue();

	// Déclaration d'un événement du clavier (touche 'escape')
	EvenementClavier evenementTest(VJAK_MINUS);

	// Simulation de l'événement
	EventManager::toucheEnfoncee(evenementTest);

	Vecteur2i vecteurTest = dynamic_cast<vue::ProjectionOrtho&>(FacadeModele::getInstance()->obtenirVue()->obtenirProjection()).obtenirDimensionFenetre();

	// Tests
	CPPUNIT_ASSERT(vecteurTest[VX] > 200);
	CPPUNIT_ASSERT(vecteurTest[VY] > 200);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenementsTest::testSourisEnfoncee()
///
/// Cas de test: Simulation d'un changement d'état par un enfoncement du bouton de la souris
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testSourisEnfoncee()
{
	// Déclaration d'un événement de la souris (bouton gauche)
	Vecteur2i vecteurTest(19,23);
	EvenementSouris evenementTest(vecteurTest, BOUTON_SOURIS_GAUCHE);

	// On effectue le test dans l'état où l'utilisateur effectue un zoom proportionnel
	EventManager::modifierEtatSouris(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
	EventManager::sourisEnfoncee(evenementTest);

	// On véfifie que l'état change l'état du bouton de la souris
	SourisEtatZoomProportionnel* se = dynamic_cast<SourisEtatZoomProportionnel*>(EventManager::etatCourant_->etatSouris_);
	CPPUNIT_ASSERT(se->estEnfoncee_);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenementsTest::testSourisRelachee()
///
/// Cas de test: Simulation d'un changement d'état par un relâchement du bouton de la souris
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testSourisRelachee()
{
	/// test stupide
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


