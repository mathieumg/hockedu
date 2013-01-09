//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenementsTest.h
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_GESTIONNAIREETATABSTRAITTEST_H
#define _TESTS_GESTIONNAIREETATABSTRAITTEST_H

#include <cppunit/extensions/HelperMacros.h>


///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEvenementsTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        méthodes de la classe GestionnaireEtatAbstrait
///
/// @author Vincent Lemire
/// @date 2012-02-03
///////////////////////////////////////////////////////////////////////////
class GestionnaireEvenementsTest : public CppUnit::TestFixture
{
	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
	CPPUNIT_TEST_SUITE( GestionnaireEvenementsTest );
	CPPUNIT_TEST( testChangementEtat );
	//CPPUNIT_TEST( testToucheEnfoncee );
	CPPUNIT_TEST( testSourisEnfoncee );
	//CPPUNIT_TEST( testSourisRelachee );
	CPPUNIT_TEST_SUITE_END();

public:

	// =================================================================
	// Méthodes pour initialiser et 'finaliser' la suite de tests
	// =================================================================

	/// Traitement à effectuer pour initialiser cette suite de tests
	void setUp();

	/// Traitement à effectuer pour 'finaliser' cette suite de tests
	void tearDown();


	// =================================================================
	// Définissez ici les différents cas de tests...
	// =================================================================
	
	/// Test général du changement d'état
	void testChangementEtat();

	/// Test d'un changement d'état par un événement du clavier
	void testToucheEnfoncee();

	/// Test d'un changement d'état par un enfoncement du bouton de la souris
	void testSourisEnfoncee();

	/// Test d'un changement d'état par un relâchement du bouton de la souris
	void testSourisRelachee();

};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

