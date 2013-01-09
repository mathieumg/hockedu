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
///        m�thodes de la classe ConfigScene
///
/// @author Julien Gascon-Samson
/// @date 2011-07-16
///////////////////////////////////////////////////////////////////////////
class ConfigSceneTest : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
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
	// M�thodes pour initialiser et 'finaliser' la suite de tests
	// =================================================================

	/// Traitement � effectuer pour initialiser cette suite de tests
	void setUp();

	/// Traitement � effectuer pour 'finaliser' cette suite de tests
	void tearDown();

	// =================================================================
	// D�finissez ici les diff�rents cas de tests...
	// =================================================================

	/// Cas de test: �criture dans un fichier XML
	void testEcritureArbreRenduXML();

	/// Cas de test: lecture d'un fichier XML
	void testLectureArbreRenduXML();

	/// Test de lecture sur un fichier n'ayant pas le format XML
	void testLectureArbreRenduXMLCorrompu();

	/// Test de lecture sur un fichier XML ayant des erreurs de syntaxe
	void testLectureArbreRenduXMLInvalide();

	/// Test de lecture et d'�criture d'un fichier XML
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
