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
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
  CPPUNIT_TEST_SUITE( UtilitaireTests );
  CPPUNIT_TEST( testRunnableBreaker );
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

	/// Cas de test: breaking Runnables
    void testRunnableBreaker();
};

#endif // _TESTS_XML_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
