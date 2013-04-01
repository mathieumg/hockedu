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
#include <stdlib.h>

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( AiLearnerTests );



////////////////////////////////////////////////////////////////////////
///
/// @fn void AiLearnerTests::setUp()
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
void AiLearnerTests::setUp()
{
    CPPUNIT_ASSERT(AILearner::obtenirInstance()->init("C:/temp/testAiLearner.airaw", Vecteur2(-1000.0f, 400.0f), Vecteur2(1000.0f, -400.0f)));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AiLearnerTests::tearDown()
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

    for(int i=0; i<2000; ++i)
    {
        LearningAiAction wRandomAction = (LearningAiAction) (rand() % 5);
        wInstance->sauvegarderNouvelleInfo(
            Vecteur3((rand() % 2000)-1000, (rand() % 800) - 400, 0.0f), // Pos AI
            Vecteur3((rand() % 255)-127, (rand() % 255)-127, 0.0f),     // Velocite AI
            Vecteur3((rand() % 2000)-1000, (rand() % 800) - 400, 0.0f),     // Pos Rondelle
            Vecteur3((rand() % 255)-127, (rand() % 255)-127, 0.0f),    // Velocite Rondelle
            Vecteur3((rand() % 2000)-1000, (rand() % 800) - 400, 0.0f),        // Pos joueur adverse
            wRandomAction    // Action a effectuer
            );

        // Set the end of reading
        wInstance->terminerSauvegardeNouvelleInfo(AI_OUTPUT_BUT_COMPTE);
    }
    

    // Dump to file in raw format
    CPPUNIT_ASSERT(wInstance->dump());

    // When implemented, reload the binary and compare data


}



// Callback a appeler une fois que le traitement est termine
int CallbackTestAiLearner(bool pOperationSuccess)
{
    std::cout << "Status Operation: " << pOperationSuccess << std::endl;
    return 0;
}


void AiLearnerTests::testConvertData()
{
    FacadePortability::sleep(2000);
    std::string wFolderPath = "C:/temp";
    std::string wFilename = "unitTest";

    AILearner::convertirDonneesRaw(wFolderPath, wFilename, CallbackTestAiLearner);


}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


