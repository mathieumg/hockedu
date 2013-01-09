//////////////////////////////////////////////////////////////////////////////
/// @file TournoiTest.h
/// @author Michael Ferris
/// @date 2012-02-18
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "Tournoi.h"


///////////////////////////////////////////////////////////////////////////
/// @class TournoiTest
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        m�thodes de la classe Tournoi
///
/// @author Michael Ferris
/// @date 2012-02-18
///////////////////////////////////////////////////////////////////////////
class TournoiTest : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( TournoiTest );
	CPPUNIT_TEST( testEcritureXML );
	CPPUNIT_TEST( testReinitialisationTournoi );
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
	void testEcritureXML();
	/// Test pour s'assurer qu'un tournoi revient a son etat orginial suite a une reinitialisation
	void testReinitialisationTournoi();

private:
	/// Objet utiles pour les tests
	SPJoueurAbstrait joueurs[16];
	Tournoi* tournoi_;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
