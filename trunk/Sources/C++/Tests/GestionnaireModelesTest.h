//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaieModeleTest.h
/// @author Vincent Lemire
/// @date 2012-02-10
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_GESTIONNAIREMODELESTEST_H
#define _TESTS_GESTIONNAIREMODELESTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "GestionnaireModeles.h"


///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireModelesTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes de la classe GestionnaireModelesTest
///
/// @author Vincent Lemire
/// @date 2012-02-10
///////////////////////////////////////////////////////////////////////////
class GestionnaireModelesTest: public CppUnit::TestFixture
{
	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( GestionnaireModelesTest );
	CPPUNIT_TEST( testObtenirModele );
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

	/// Test de l'obtention du mod�le
	void testObtenirModele();

private:
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


