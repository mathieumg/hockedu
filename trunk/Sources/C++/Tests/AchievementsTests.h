//////////////////////////////////////////////////////////////////////////////
/// @file AchievementsTests.h
/// @author Michael Ferris
/// @date 2013-03-25
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef _TESTS_ACHIEVEMENTSTESTS_H
#define _TESTS_ACHIEVEMENTSTESTS_H

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "AchievementsEnums.h"
#include <vector>

using namespace std;


///////////////////////////////////////////////////////////////////////////
/// @class AchievementsTests
/// @brief Classe de test cppunit pour tester le bon fonctionnement des
///        classes de d'achievements
///
/// @author Michael Ferris
/// @date 2013-03-25
///////////////////////////////////////////////////////////////////////////
class AchievementsTests : public CppUnit::TestFixture
{

	// =================================================================
	// D�claration de la suite de tests et des m�thodes de tests
	//
	// Important, vous devez d�finir chacun de vos cas de tests � l'aide
	// de la macro CPPUNIT_TEST sinon ce dernier ne sera pas ex�cut� !
	// =================================================================
	CPPUNIT_TEST_SUITE( AchievementsTests );
    CPPUNIT_TEST( testAchievementStartApp );
    CPPUNIT_TEST( testAchievementGameWon );
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

    void testAchievementStartApp();
    void testAchievementGameWon();
	
    static int AchievementUnlocked(AchievementsType pType, char* pMessage);
private:
    static std::vector<AchievementsType> mAchievementUnlocked;

};

#endif // _TESTS_ACHIEVEMENTSTESTS_H


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
