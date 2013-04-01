//////////////////////////////////////////////////////////////////////////////
/// @file AiLearnerTests.h
/// @author Mathieu Parent
/// @date 2013-03-30
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_AI_LEARNER_H
#define _TESTS_AI_LEARNER_H

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "AchievementsEnums.h"
#include <vector>

using namespace std;



///////////////////////////////////////////////////////////////////////////
/// @class AiLearnerTests
/// @brief Classe de test cppunit pour tester le bon fonctionnement du ai learner
///
/// @author Mathieu Parent
/// @date 2013-03-30
///////////////////////////////////////////////////////////////////////////
class AiLearnerTests : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( AiLearnerTests );
    CPPUNIT_TEST( testAddNewDataAndSaveRawBinary );
    CPPUNIT_TEST( testConvertData );
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
    
    void testAddNewDataAndSaveRawBinary();
    void testConvertData();
	
private:

};

#endif // _TESTS_AI_LEARNER_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
