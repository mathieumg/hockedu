//////////////////////////////////////////////////////////////////////////////
/// @file UtilitaireTests.h
/// @author Michael Ferris
/// @date 2013-03-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_UTILITAIRE_H
#define _TESTS_UTILITAIRE_H

#include <cppunit/extensions/HelperMacros.h>

///////////////////////////////////////////////////////////////////////////
/// @class UtilitaireTests
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        algorithme dans l'Utilitaire
///
/// @author Michael Ferris
/// @date 2013-03-05
///////////////////////////////////////////////////////////////////////////
class UtilitaireTests : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
  CPPUNIT_TEST_SUITE( UtilitaireTests );
  CPPUNIT_TEST( testRunnableBreaker );
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

	/// Cas de test: breaking Runnables
    void testRunnableBreaker();
};

#endif // _TESTS_XML_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
