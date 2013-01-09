//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenementsTest.h
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_GESTIONNAIREETATABSTRAITTEST_H
#define _TESTS_GESTIONNAIREETATABSTRAITTEST_H

#include <cppunit/extensions/HelperMacros.h>


///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEvenementsTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes de la classe GestionnaireEtatAbstrait
///
/// @author Vincent Lemire
/// @date 2012-02-03
///////////////////////////////////////////////////////////////////////////
class GestionnaireEvenementsTest : public CppUnit::TestFixture
{
	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( GestionnaireEvenementsTest );
	CPPUNIT_TEST( testChangementEtat );
	//CPPUNIT_TEST( testToucheEnfoncee );
	CPPUNIT_TEST( testSourisEnfoncee );
	//CPPUNIT_TEST( testSourisRelachee );
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
	
	/// Test g�n�ral du changement d'�tat
	void testChangementEtat();

	/// Test d'un changement d'�tat par un �v�nement du clavier
	void testToucheEnfoncee();

	/// Test d'un changement d'�tat par un enfoncement du bouton de la souris
	void testSourisEnfoncee();

	/// Test d'un changement d'�tat par un rel�chement du bouton de la souris
	void testSourisRelachee();

};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

