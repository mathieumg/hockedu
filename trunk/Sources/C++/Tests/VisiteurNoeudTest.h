//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurNoeudTest.h
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_VISITEURNOEUDTEST_H
#define _TESTS_VISITEURNOEUDTEST_H

#include <cppunit/extensions/HelperMacros.h>


///////////////////////////////////////////////////////////////////////////
/// @class VisiteurNoeudTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes de la classe VisiteurNoeudTest
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-02-03
///////////////////////////////////////////////////////////////////////////
class VisiteurNoeudTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( VisiteurNoeudTest );
	CPPUNIT_TEST( testDeplacerObjet );
	CPPUNIT_TEST( testSelectionObjet );
	CPPUNIT_TEST( testSuppressionObjet );
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

	/// Test pour le d�placement d'�l�ments dans l'�diteur
	void testDeplacerObjet();
	/// Test de selection de noeuds
	void testSelectionObjet();
	/// Test de suppression de noeuds
	void testSuppressionObjet();

};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////