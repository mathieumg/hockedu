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
/// Effectue l'initialisation pr�alable � l'ex�cution de l'ensemble des
/// cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets doivent �tre construits, il est conseill� de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::setUp()
{
	// L'�tat de base pour les tests est le mode d'�dition
	EventManager::modifierEtat( ETAT_MODE_EDITION );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::tearDown()
///
/// Effectue les op�rations de finalisation n�cessaires suite � l'ex�cution
/// de l'ensemble des cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets ont �t� allou�s � l'initialisation, ils doivent �tre
/// d�sallou�s, et il est conseill� de le faire ici.
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
/// Cas de test: On teste un changement d'�tat manuel.
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testChangementEtat()
{
	// Modification de l'�tat
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
/// Cas de test: Simulation d'un changement d'�tat par un �v�nement du clavier
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testToucheEnfoncee()
{
	FacadeModele::getInstance()->initialiserVue();

	// D�claration d'un �v�nement du clavier (touche 'escape')
	EvenementClavier evenementTest(VJAK_MINUS);

	// Simulation de l'�v�nement
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
/// Cas de test: Simulation d'un changement d'�tat par un enfoncement du bouton de la souris
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenementsTest::testSourisEnfoncee()
{
	// D�claration d'un �v�nement de la souris (bouton gauche)
	Vecteur2i vecteurTest(19,23);
	EvenementSouris evenementTest(vecteurTest, BOUTON_SOURIS_GAUCHE);

	// On effectue le test dans l'�tat o� l'utilisateur effectue un zoom proportionnel
	EventManager::modifierEtatSouris(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
	EventManager::sourisEnfoncee(evenementTest);

	// On v�fifie que l'�tat change l'�tat du bouton de la souris
	SourisEtatZoomProportionnel* se = dynamic_cast<SourisEtatZoomProportionnel*>(EventManager::etatCourant_->etatSouris_);
	CPPUNIT_ASSERT(se->estEnfoncee_);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenementsTest::testSourisRelachee()
///
/// Cas de test: Simulation d'un changement d'�tat par un rel�chement du bouton de la souris
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


