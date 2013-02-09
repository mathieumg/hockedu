//////////////////////////////////////////////////////////////////////////////
/// @file NoeudCompositeTest.h
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_NOEUDCOMPOSITETEST_H
#define _TESTS_NOEUDCOMPOSITETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "NoeudComposite.h"
#include "RazerGameTree.h"


///////////////////////////////////////////////////////////////////////////
/// @class NoeudCompositeTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        méthodes de la classe NoeudCompositeTest
///
/// @author Vincent Lemire
/// @date 2012-02-03
///////////////////////////////////////////////////////////////////////////
class NoeudCompositeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( NoeudCompositeTest );
	CPPUNIT_TEST (ajoutEnfantTest);
	CPPUNIT_TEST (viderArbreTest);
	CPPUNIT_TEST (ajoutRecursifTest);
	CPPUNIT_TEST (effacerEnfantsTest);
	CPPUNIT_TEST (chercherEnfantTest);
	CPPUNIT_TEST (selectionTousTest);
	CPPUNIT_TEST (deselectionTousTest);
	CPPUNIT_TEST (positionAbsolueTest);
	CPPUNIT_TEST (modificationTerrain);
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

	/// test d'un noeud composite pour l'ajout d'enfant
	void ajoutEnfantTest();
	/// S'assure que l'arbre est vider et que les enfants sont invalidés
	void viderArbreTest();
	/// S'assure que si on ajoute un enfant ayant déjà un parent, le parent perd son pointeur sur cet enfant
	void ajoutRecursifTest();
	/// Test pour la suprresion d'enfant de l'arbe de rendu
	void effacerEnfantsTest();
	/// Test pour la recherche d'enfant dans l'arbre de rendu
	void chercherEnfantTest();
	/// Test pour la selection d'un arbre de rendu
	void selectionTousTest();
	/// Test pour la déselection d'un arbre de rendu
	void deselectionTousTest();
	/// Test pour les position absolue des enfants
	void positionAbsolueTest();
	/// Test pour la modification de terrain dans tout l'arbre
	void modificationTerrain();

private:
	NoeudComposite* enfants[5];
	RazerGameTree *arbre;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////