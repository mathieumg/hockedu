//////////////////////////////////////////////////////////////////////////////
/// @file GolTests.h
/// @author Michael Ferris
/// @date 2013-03-25
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_GolTests_H
#define _TESTS_GolTests_H

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>

using namespace std;


///////////////////////////////////////////////////////////////////////////
/// @class GolTests
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        classes de d'achievements
///
/// @author Michael Ferris
/// @date 2013-03-25
///////////////////////////////////////////////////////////////////////////
class GolTests : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
	CPPUNIT_TEST_SUITE( GolTests );
    CPPUNIT_TEST( testDecodeRLE );
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

    void testDecodeRLE( );
};

#endif // _TESTS_GolTests_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
