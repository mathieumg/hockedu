//////////////////////////////////////////////////////////////////////////////
/// @file ConfigSceneTest.h
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_CONFIGSCENETEST_H
#define _TESTS_CONFIGSCENETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "ConfigScene.h"

///////////////////////////////////////////////////////////////////////////
/// @class ConfigSceneTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        méthodes de la classe ConfigScene
///
/// @author Julien Gascon-Samson
/// @date 2011-07-16
///////////////////////////////////////////////////////////////////////////
class ConfigSceneTest : public CppUnit::TestFixture
{

	// =================================================================
	// Déclaration de la suite de tests et des méthodes de tests
	//
	// Important, vous devez définir chacun de vos cas de tests à l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas exécuté !
	// =================================================================
   CPPUNIT_TEST_SUITE( ConfigSceneTest );
   CPPUNIT_TEST( testEcritureArbreRenduXML );
   CPPUNIT_TEST( testLectureArbreRenduXML );
   CPPUNIT_TEST( testLectureArbreRenduXMLCorrompu );
   CPPUNIT_TEST( testLectureArbreRenduXMLInvalide );
   CPPUNIT_TEST( testLectureEcritureJoueurXML );
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

	/// Cas de test: écriture dans un fichier XML
	void testEcritureArbreRenduXML();

	/// Cas de test: lecture d'un fichier XML
	void testLectureArbreRenduXML();

	/// Test de lecture sur un fichier n'ayant pas le format XML
	void testLectureArbreRenduXMLCorrompu();

	/// Test de lecture sur un fichier XML ayant des erreurs de syntaxe
	void testLectureArbreRenduXMLInvalide();

	/// Test de lecture et d'écriture d'un fichier XML
	void testLectureEcritureJoueurXML();

private:
	/// Un arbre (racine) et des noeuds pour les tests
	ArbreRenduINF2990* arbre;
	NoeudAbstrait* noeuds_[6];
};

#endif // _TESTS_CONFIGSCENETEST_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
