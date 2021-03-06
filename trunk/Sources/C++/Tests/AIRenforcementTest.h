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
#include "PlayerComputer.h"
///////////////////////////////////////////////////////////////////////////
/// @class AIRenforcementTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes du AI renforceent
///
/// @author Samuel Ledoux
/// @date 2011-07-16
///////////////////////////////////////////////////////////////////////////
class AIRenforcementTest : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( AIRenforcementTest );
	CPPUNIT_TEST( testDirectionPhaseDeplacement );
	CPPUNIT_TEST( testDirectionPhaseAttente );
	CPPUNIT_TEST( testDirectionPhaseAttaque );
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
	void testDirectionPhaseDeplacement();

	/// Cas de test: type de noeud
	void testDirectionPhaseAttente();

	/// Cas de test: d�finition/obtention de l'�tat (bool�ens) du noeud
	void testDirectionPhaseAttaque();

private:
	Partie* partie;
};

#endif // _TESTS_AIRENFORCEMENTTEST_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
