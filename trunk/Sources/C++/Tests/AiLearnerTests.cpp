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
#include "JoueurVirtuelRenforcement.h"

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
    CPPUNIT_ASSERT(AILearner::obtenirInstance()->init("C:/temp/testAiLearner.airaw", Vecteur2(-1000.0f, 400.0f), Vecteur2(1000.0f, -400.0f)));
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

    for(int i=0; i<500000; ++i)
    {
        LearningAiAction wRandomAction = (LearningAiAction) (rand() % 5);
        wInstance->sauvegarderNouvelleInfo(
            Vecteur3((float)((rand() % 2000)-1000), (float)((rand() % 800) - 400), 0.0f),  // Pos AI
            Vecteur3((float)((rand() % 255)-127)  , (float)((rand() % 255)-127)  , 0.0f),  // Velocite AI
            Vecteur3((float)((rand() % 2000)-1000), (float)((rand() % 800) - 400), 0.0f),  // Pos Rondelle
            Vecteur3((float)((rand() % 255)-127)  , (float)((rand() % 255)-127)  , 0.0f),  // Velocite Rondelle
            Vecteur3((float)((rand() % 2000)-1000), (float)((rand() % 800) - 400), 0.0f),  // Pos joueur adverse
            wRandomAction    // Action a effectuer
            );
        
        // Test pour voir si la conversion se fait bien en mettant le meme elemtn plusieurs fois

//         wInstance->sauvegarderNouvelleInfo(
//             Vecteur3(0.0f, 0.0f, 0.0f),       // Pos AI
//             Vecteur3(-3.0f , 10.0f, 0.0f),      // Velocite AI
//             Vecteur3(50.0f, 20.0f, 0.0f),     // Pos Rondelle
//             Vecteur3(-5.0f , -10.0f, 0.0f),  // Velocite Rondelle
//             Vecteur3(10.0f, 20.0f, 0.0f),     // Pos joueur adverse
//             wRandomAction                     // Action a effectuer
//             );

        // Set the end of reading
        LearningAiOutput wRandomOutput = (LearningAiOutput) (rand() % 3);
        wInstance->terminerSauvegardeNouvelleInfo(wRandomOutput);
    }
    

    // Dump to file in raw format
    CPPUNIT_ASSERT(wInstance->dump());

    // When implemented, reload the binary and compare data








}



// Callback a appeler une fois que le traitement est termine
int CallbackTestAiLearner(bool pOperationSuccess)
{
    std::cout << "Status Operation: " << pOperationSuccess << std::endl;
    FacadePortability::sleep(1000);
    AiLearnerTests::testReloadAiLogic();
    return 0;
}


void AiLearnerTests::testConvertData()
{
    FacadePortability::sleep(1000);
    std::string wFolderPath = "C:/temp";
    std::string wFilename = "unitTest";

    AILearner::convertirDonneesRaw(wFolderPath, wFilename, CallbackTestAiLearner);


}



void AiLearnerTests::testReloadAiLogic()
{
    // Reinit pcq une fois que le test est termine, le liberer instance est appele et la dimension de la map devient nulle
    AILearner::obtenirInstance()->init("C:/temp/testAiLearner2.airaw", Vecteur2(-1000.0f, 400.0f), Vecteur2(1000.0f, -400.0f));
    JoueurVirtuelRenforcement wJoueur("C:/temp/unitTest.ailogic");
    AiRuntimeInfosInput wInfos;
    wInfos.positionAi[VX] = 0;
    wInfos.positionAi[VY] = 0;

    wInfos.velociteAi[VX] = -3;
    wInfos.velociteAi[VY] = 10;

    wInfos.positionAdversaire[VX] = 10;
    wInfos.positionAdversaire[VY] = 20;

    wInfos.velociteRondelle[VX] = -5;
    wInfos.velociteRondelle[VY] = -10;

    wInfos.positionRondelle[VX] = 50;
    wInfos.positionRondelle[VY] = 20;


    AiRuntimeInfosOutput wAction = wJoueur.getActionFor(
        Vecteur3(0.0f, 0.0f, 0.0f),         // Pos AI
        Vecteur3(-3.0f , 10.0f, 0.0f),      // Velocite AI
        Vecteur3(50.0f, 20.0f, 0.0f),       // Pos Rondelle
        Vecteur3(-5.0f , -10.0f, 0.0f),     // Velocite Rondelle
        Vecteur3(10.0f, 20.0f, 0.0f)       // Pos joueur adverse
    );
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


