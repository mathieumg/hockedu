//////////////////////////////////////////////////////////////////////////////
/// @file ReseauTest.h
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_RESEAUTESTS_H
#define _TESTS_RESEAUTESTS_H

#include <string>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;


///////////////////////////////////////////////////////////////////////////
/// @class ReseauTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        classes de reseautique
///
/// @author Mathieu Parent
/// @date 2011-12-01
///////////////////////////////////////////////////////////////////////////
class ReseauTests : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
	CPPUNIT_TEST_SUITE( ReseauTests );
	CPPUNIT_TEST( testCreationSocketTCP );
	CPPUNIT_TEST( testCreationSocketUDP );
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

	/// Cas de test: Creation d'un socket TCP
	void testCreationSocketTCP();


	/// Cas de test: Creation d'un socket UDP
	void testCreationSocketUDP();
	

private:
	string localIP_;
	string destinationIP_;
};

#endif // _TESTS_RESEAUTESTS_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
