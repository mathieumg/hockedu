//////////////////////////////////////////////////////////////////////////////
/// @file ProjectionOrthoTest.h
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_PROJECTIONORTHOTEST_H
#define _TESTS_PROJECTIONORTHOTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "ProjectionOrtho.h"


///////////////////////////////////////////////////////////////////////////
/// @class ProjectionOrthoTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes de la classe ProjectionOrthoTest
///
/// @author Vincent Lemire
/// @date 2012-02-03
///////////////////////////////////////////////////////////////////////////
class ProjectionOrthoTest : public CppUnit::TestFixture
{
	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( ProjectionOrthoTest );
	CPPUNIT_TEST( testTranslation );
	CPPUNIT_TEST( testZoomIn );
	CPPUNIT_TEST( testZoomOut );
	CPPUNIT_TEST( testRedimensionnement );
	CPPUNIT_TEST( testAjusterRapportAspect );
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

	/// Test de la translation de la projection
	void testTranslation();

	/// Test du zoom de la projection
	void testZoomIn();

	/// Test du zoom de la projection
	void testZoomOut();

	/// Test du redimensionnement de la fen�tre
	void testRedimensionnement();

	/// Test d'ajustement du rapport d'aspect
	void testAjusterRapportAspect();

	/// Test de la conversion des distances
	void testConversionClotureVirtuelle();


private:
	/// Projection Ortho servant pour les tests
	vue::ProjectionOrtho* p_;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////