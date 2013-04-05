//////////////////////////////////////////////////////////////////////////////
/// @file AIRenforcementTest.h
/// @author Samuel Ledoux
/// @date 2011-07-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_AIRENFORCEMENTTEST_H
#define _TESTS_AIRENFORCEMENTTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "RazerGameTree.h"
#include "Renforcement/AIMailletRenforcement.h"
#include "Partie.h"
#include "JoueurVirtuel.h"
///////////////////////////////////////////////////////////////////////////
/// @class AIRenforcementTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        méthodes du AI renforceent
///
/// @author Samuel Ledoux
/// @date 2011-07-16
///////////////////////////////////////////////////////////////////////////
class AIRenforcementTest : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
	CPPUNIT_TEST_SUITE( AIRenforcementTest );
	CPPUNIT_TEST( testDirectionPhaseDeplacement );
	CPPUNIT_TEST( testDirectionPhaseAttente );
	CPPUNIT_TEST( testDirectionPhaseAttaque );
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
	void testDirectionPhaseDeplacement();

	/// Cas de test: type de noeud
	void testDirectionPhaseAttente();

	/// Cas de test: définition/obtention de l'état (booléens) du noeud
	void testDirectionPhaseAttaque();

private:
	Partie* partie;
};

#endif // _TESTS_AIRENFORCEMENTTEST_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
