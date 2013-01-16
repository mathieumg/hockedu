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
#include "ArbreRenduINF2990.h"

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
	GestionnaireEvenements::obtenirInstance()->modifierEtat( ETAT_MODE_EDITION );
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
	if(GestionnaireEvenements::obtenirInstance()->etatCourant_ != 0)
		delete GestionnaireEvenements::obtenirInstance()->etatCourant_;
	GestionnaireEvenements::obtenirInstance()->etatCourant_ = 0;
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
	GestionnaireEvenements::obtenirInstance()->modifierEtat( ETAT_MODE_EDITION );
	
	// Test
	GestionnaireEvenements* g = GestionnaireEvenements::obtenirInstance()->obtenirEtat()->obtenirContexte();
	CPPUNIT_ASSERT(g == GestionnaireEvenements::obtenirInstance());
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
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
	GestionnaireEvenements::obtenirInstance()->sourisEnfoncee(evenementTest);

	// On v�fifie que l'�tat change l'�tat du bouton de la souris
	SourisEtatZoomProportionnel* se = dynamic_cast<SourisEtatZoomProportionnel*>(GestionnaireEvenements::obtenirInstance()->obtenirEtat()->etatSouris_);
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
	// Initialisation d'un arbre de rendu et calcul de son nombre d'enfants
	FacadeModele::getInstance()->reinitialiserTerrain();
	int nbEnfantsAvant = FacadeModele::getInstance()->obtenirArbreRenduINF2990()->obtenirNombreEnfants();
	
	// D�claration d'un �v�nement de la souris (bouton gauche)
	Vecteur2i vecteurTest(19,23);
	EvenementSouris evenementTest(vecteurTest, BOUTON_SOURIS_GAUCHE);

	// On effectue le test dans l'�tat o� l'utilisateur place un portail dans le mode �diteur
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_AJOUTER_PORTAIL);
	GestionnaireEvenements::obtenirInstance()->sourisRelachee(evenementTest);

	// En rel�chant le bouton de la souris, l'arbre doit gagner un enfant, on v�rifie le bon fonctionnement
	int nbEnfantsApres = FacadeModele::getInstance()->obtenirArbreRenduINF2990()->obtenirNombreEnfants();
	CPPUNIT_ASSERT(nbEnfantsApres = nbEnfantsAvant+1);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


