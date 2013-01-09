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
///        m�thodes de la classe Tournoi
///
/// @author Michael Ferris
/// @date 2012-04-06
///////////////////////////////////////////////////////////////////////////
class TerrainTest : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( TerrainTest );
	CPPUNIT_TEST( testInitialiser );
	CPPUNIT_TEST( testVerifierValiditer );
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
