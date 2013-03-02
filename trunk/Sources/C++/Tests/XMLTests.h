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
///        m�thodes pour l'�criture et lecture XML
///
/// @author Michael Ferris
/// @date 2013-03-01
///////////////////////////////////////////////////////////////////////////
class XMLTests : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
  CPPUNIT_TEST_SUITE( XMLTests );
  //CPPUNIT_TEST( testGetTreeRoot );
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

	/// Cas de test: �criture/lecture de la position relative

};

#endif // _TESTS_XML_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
