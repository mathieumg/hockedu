////////////////////////////////////////////////////////////////////////////////////
/// @file AiLearnerTests.cpp
/// @author Mathieu Parent
/// @date 2013-03-30
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "AiLearnerTests.h"
#include "Renforcement\AILearner.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( AiLearnerTests );



////////////////////////////////////////////////////////////////////////
///
/// @fn void AiLearnerTests::setUp()
///
/// Effectue l'initialisation préalable à l'exécution de l'ensemble des
/// cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets doivent être construits, il est conseillé de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void AiLearnerTests::setUp()
{
    CPPUNIT_ASSERT(AILearner::obtenirInstance()->init("C:/temp/testAiLearner.airaw", Vecteur2(-1000.0f, 400.0f), Vecteur2(1000.0f, -400.0f)), "AiLearner init failed");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AiLearnerTests::tearDown()
///
/// Effectue les opérations de finalisation nécessaires suite à l'exécution
/// de l'ensemble des cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets ont été alloués à l'initialisation, ils doivent être
/// désalloués, et il est conseillé de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void AiLearnerTests::tearDown()
{
    AILearner::libererInstance();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AiLearnerTests::testAddNewDataAndSaveRawBinary()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AiLearnerTests::testAddNewDataAndSaveRawBinary()
{
    // Ajouter des nouveaux entries
    AILearner* wInstance = AILearner::obtenirInstance();

    for(int i=0; i<1000000; ++i)
    {
        wInstance->sauvegarderNouvelleInfo(
            Vecteur3(-500.0f, -200.0f, 0.0f), // Pos AI
            Vecteur3(20.0f, 50.0f, 0.0f),     // Velocite AI
            Vecteur3(0.0f, 300.0f, 0.0f),     // Pos Rondelle
            Vecteur3(-10.0f, 50.0f, 0.0f),    // Velocite Rondelle
            Vecteur3(700.0f, -300.0f),        // Pos joueur adverse
            AI_ACTION_ATTAQUER_DIRECTEMENT    // Action a effectuer
            );

        // Set the end of reading
        wInstance->terminerSauvegardeNouvelleInfo(AI_OUTPUT_BUT_COMPTE);
    }
    

    // Dump to file in raw format
    CPPUNIT_ASSERT(wInstance->dump(), "Dump failed");

    // When implemented, reload the binary and compare data


}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


