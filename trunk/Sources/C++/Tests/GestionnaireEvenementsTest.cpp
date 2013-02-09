//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenementsTest.cpp
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEvenementsTest.h"
#include "GestionnaireEvenements.h"
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
	GestionnaireEvenements::obtenirInstance()->modifierEtat( ETAT_MODE_EDITION );
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
	if(GestionnaireEvenements::obtenirInstance()->etatCourant_ != 0)
		delete GestionnaireEvenements::obtenirInstance()->etatCourant_;
	GestionnaireEvenements::obtenirInstance()->etatCourant_ = 0;
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
	GestionnaireEvenements::obtenirInstance()->modifierEtat( ETAT_MODE_EDITION );
	
	// Test
	GestionnaireEvenements* g = GestionnaireEvenements::obtenirInstance()->obtenirEtat()->obtenirContexte();
	CPPUNIT_ASSERT(g == GestionnaireEvenements::obtenirInstance());
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
	GestionnaireEvenements::obtenirInstance()->toucheEnfoncee(evenementTest);

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
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
	GestionnaireEvenements::obtenirInstance()->sourisEnfoncee(evenementTest);

	// On véfifie que l'état change l'état du bouton de la souris
	SourisEtatZoomProportionnel* se = dynamic_cast<SourisEtatZoomProportionnel*>(GestionnaireEvenements::obtenirInstance()->obtenirEtat()->etatSouris_);
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


