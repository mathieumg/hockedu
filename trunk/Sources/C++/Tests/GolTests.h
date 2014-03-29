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
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( GolTests );
    CPPUNIT_TEST( testDecodeRLE );
	CPPUNIT_TEST_SUITE_END();

public:

	// =================================================================
	// M�thodes pour initialiser et 'finaliser' la suite de tests
	// =================================================================

	/// Traitement � effectuer pour initialiser cette suite de tests
	void setUp();

	/// Traitement � effectuer pour 'finaliser' cette suite de tests
	void tearDown();

	// =================================================================
	// D�finissez ici les diff�rents cas de tests...
	// =================================================================

    void testDecodeRLE( );
};

#endif // _TESTS_GolTests_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
