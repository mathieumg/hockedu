//////////////////////////////////////////////////////////////////////////////
/// @file XMLTests.h
/// @author Michael Ferris
/// @date 2013-03-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_XML_H
#define _TESTS_XML_H

#include <cppunit/extensions/HelperMacros.h>

///////////////////////////////////////////////////////////////////////////
/// @class XMLTests
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        méthodes pour l'écriture et lecture XML
///
/// @author Michael Ferris
/// @date 2013-03-01
///////////////////////////////////////////////////////////////////////////
class XMLTests : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
  CPPUNIT_TEST_SUITE( XMLTests );
  //CPPUNIT_TEST( testGetTreeRoot );
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

	/// Cas de test: écriture/lecture de la position relative

};

#endif // _TESTS_XML_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
