////////////////////////////////////////////////////////////////////////////////////
/// @file GolTests.cpp
/// @author Michael Ferris
/// @date 2013-03-25
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "GolTests.h"
#include "AchievementsManager.h"
#include "LaunchAchievementLite.h"
#include <algorithm>
#include "GoL\GolUtils.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( GolTests );


////////////////////////////////////////////////////////////////////////
///
/// @fn void GolTests::setUp()
///
/// Effectue l'initialisation pr�alable � l'ex�cution de l'ensemble des
/// cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets doivent �tre construits, il est conseill� de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GolTests::setUp()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GolTests::tearDown()
///
/// Effectue les op�rations de finalisation n�cessaires suite � l'ex�cution
/// de l'ensemble des cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets ont �t� allou�s � l'initialisation, ils doivent �tre
/// d�sallou�s, et il est conseill� de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GolTests::tearDown()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GolTests::testDecodeRLE()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GolTests::testDecodeRLE( )
{
    delete GolUtils::decodeRLEFile( "..\\TestFiles\\GoL\\simple.lif" );
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


