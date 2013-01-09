//////////////////////////////////////////////////////////////////////////////
/// @file TerrainTest.h
/// @author Michael Ferris
/// @date 2012-04-06
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "Terrain.h"


///////////////////////////////////////////////////////////////////////////
/// @class TerrainTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        méthodes de la classe Tournoi
///
/// @author Michael Ferris
/// @date 2012-04-06
///////////////////////////////////////////////////////////////////////////
class TerrainTest : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
	CPPUNIT_TEST_SUITE( TerrainTest );
	CPPUNIT_TEST( testInitialiser );
	CPPUNIT_TEST( testVerifierValiditer );
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
	/// Test de la methode qui verifie la validite d'un terrain
	void testVerifierValiditer();
	/// Test l'initialisation d'un terrain
	void testInitialiser();

private:
	Terrain* terrain_;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
