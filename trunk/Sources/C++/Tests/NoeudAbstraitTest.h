//////////////////////////////////////////////////////////////////////////////
/// @file NoeudAbstraitTest.h
/// @author Julien Gascon-Samson
/// @date 2011-07-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_NOEUDABSTRAITTEST_H
#define _TESTS_NOEUDABSTRAITTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "RazerGameTree.h"
///////////////////////////////////////////////////////////////////////////
/// @class NoeudAbstraitTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes de la classe NoeudAbstrait
///
/// @author Julien Gascon-Samson
/// @date 2011-07-16
///////////////////////////////////////////////////////////////////////////
class NoeudAbstraitTest : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
  CPPUNIT_TEST_SUITE( NoeudAbstraitTest );
  CPPUNIT_TEST( testPositionRelative );
  CPPUNIT_TEST( testType );
  CPPUNIT_TEST( testEtat );
  CPPUNIT_TEST( testSelection );
  CPPUNIT_TEST( testEnfants );
  CPPUNIT_TEST( testAjout );
  CPPUNIT_TEST( testGetTreeRoot );
  CPPUNIT_TEST( testInsideTable );
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
	void testPositionRelative();

	/// Cas de test: type de noeud
	void testType();

	/// Cas de test: d�finition/obtention de l'�tat (bool�ens) du noeud
	void testEtat();

	/// Cas de test: d�finition/obtention des �tats de s�lection du noeud
	void testSelection();

	/// Cas de test: s'assurer que le noeud abstrait n'a pas d'enfant
	void testEnfants();

	/// Cas de test: s'assurer qu'il est impossible d'ajouter un enfant
	void testAjout();

    /// Cas de test: s'assurer de bien retrouver la racine du noeud si elle existe
    void testGetTreeRoot();

    /// Cas de test: s'assurer de bien detecter si un noeud est a lexterieur de la table
    void testInsideTable();

private:
	/// Instance d'un noeud abstrait
	NoeudAbstrait* noeud;
};

#endif // _TESTS_NOEUDABSTRAITTEST_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
