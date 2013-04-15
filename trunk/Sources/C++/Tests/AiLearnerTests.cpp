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
#include "GameManager.h"
#include "JoueurHumain.h"
#include "PartieApprentissage.h"
#include "Terrain.h"
#include <iosfwd>

bool AiLearnerTests::mDataSavingFinished = false;

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
    //CPPUNIT_ASSERT(AILearner::obtenirInstance()->init("./testAiLearner.airaw", Vecteur2(-1000.0f, 400.0f), Vecteur2(1000.0f, -400.0f)));
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
    //AILearner::libererInstance();
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
    /*AILearner* wInstance = AILearner::obtenirInstance();

    for(int i=0; i<500; ++i)
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
    */
}



// Callback a appeler une fois que le traitement est termine
int CallbackTestAiLearner(bool pOperationSuccess)
{
    /*std::cout << "Status Operation: " << pOperationSuccess << std::endl;
    FacadePortability::sleep(1000);
    AiLearnerTests::testReloadAiLogic();*/
    return 0;
}


void AiLearnerTests::testConvertData()
{
    /*FacadePortability::sleep(1000);
    std::string wFolderPath = "./";
    std::string wFilename = "unitTest";

    AILearner::convertirDonneesRaw(wFolderPath, wFilename, CallbackTestAiLearner);*/
}


void AiLearnerTests::testReloadAiLogic()
{
    /*
    // Reinit pcq une fois que le test est termine, le liberer instance est appele et la dimension de la map devient nulle
    AILearner::obtenirInstance()->init("./testAiLearner2.airaw", Vecteur2(-1000.0f, 400.0f), Vecteur2(1000.0f, -400.0f));
    JoueurVirtuelRenforcement wJoueur("./unitTest.ailogic");
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
    */
}

void AiLearnerTests::testLearningGame()
{
    /*
    char* wOutputFolder = "LearningTest/";

    FacadePortability::createDirectory(wOutputFolder);
    GameManager* wGameManager = GameManager::obtenirInstance();
    SPJoueurAbstrait wDummyPlayer(new JoueurHumain("Asd"));
    std::ostringstream wRawDataOutputFile;
    wRawDataOutputFile << wOutputFolder << "TestAI" << AI_LEARNER_RAW_DATA_EXTENSION;
    AILearner::obtenirInstance()->changerRepertoireOutput(wRawDataOutputFile.str());
    SPJoueurAbstrait wLearningAI(new JoueurVirtuelRenforcement(wRawDataOutputFile.str()));
    int wGameId = wGameManager->addNewGame(GAME_TYPE_OFFLINE, wDummyPlayer, wLearningAI, false, true);
    PartieApprentissage* wGame = (PartieApprentissage*)wGameManager->getGame(wGameId);
    Terrain* wField = wGame->getField();
    wField->creerTerrainParDefaut("");
    wGame->getReadyToPlay();
    NoeudMaillet* wLearningMallet = wField->getRightMallet();
    NoeudRondelle* wPuck = wField->getPuck();
    float wTimeElapsed = 10.0/1000.0;
    wPuck->setPosition(Vecteur3(-5,0,0));
    wGame->animer(wTimeElapsed);
    wPuck->setPosition(Vecteur3(5,0,0)); 
    wGame->animer(wTimeElapsed); // Should generate a (75,0,0), (0,0,0), (5, 0, 0), (0, 0, 0), (-75, 0, 0), [action] learning.
    wPuck->setPosition(Vecteur3(-5,0,0));
    wGame->animer(wTimeElapsed);
    wPuck->setPosition(Vecteur3(4,0,0));
    wGame->animer(wTimeElapsed); // Gives result AI_OUTPUT_RIEN to previous learning and starts a new learning with the data (75,0,0), (0,0,0), (4, 0, 0), (0, 0, 0), (-75, 0, 0), [action]
    wPuck->setPosition(Vecteur3(-5,0,0));
    wGame->animer(wTimeElapsed);
    wGame->incrementerPointsJoueurDroit(); // Gives result AI_OUTPUT_BUT_COMPTE to previous learning.
    wPuck->setPosition(Vecteur3(6,0,0));
    wGame->animer(wTimeElapsed); // Starts a new learning with the data (75,0,0), (0,0,0), (6, 0, 0), (0, 0, 0), (-75, 0, 0), [action]
    wPuck->setPosition(Vecteur3(-5,0,0));
    wGame->animer(wTimeElapsed);
    wGame->incrementerPointsJoueurGauche(); // Gives result AI_OUTPUT_ADVERSAIRE_BUT_COMPTE to previous learning.
    wPuck->setPosition(Vecteur3(5,0,0));

    AILearner::obtenirInstance()->dump();

    std::ostringstream wConvertedDataOutputFile, wOutputFileName;
    wOutputFileName << "TestAI" << AI_LEARNER_RUNTIME_DATA_EXTENSION;
    wConvertedDataOutputFile << wOutputFolder << wOutputFileName.str();
    CPPUNIT_ASSERT(AILearner::obtenirInstance()->convertirDonneesRaw(wOutputFolder, wOutputFileName.str(),notifyDataSavingFinished));
    
    FacadePortability::sleep(100);
    CPPUNIT_ASSERT(mDataSavingFinished);

    auto wLoadedAi = dynamic_pointer_cast<JoueurVirtuelRenforcement>(SPJoueurAbstrait(new JoueurVirtuelRenforcement(wConvertedDataOutputFile.str())));

    Vecteur3 wAIPosition(75,0,0),
             wAIVelocity(0,0,0),
             wPuckVelocity(0,0,0),
             wOpponentPosition(-75,0,0);
    Vecteur3 wPuckPosition(5,0,0);

    CPPUNIT_ASSERT(wLoadedAi->hasMapEntryFor(wAIPosition,wAIVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition));
    wPuckPosition = Vecteur3(4,0,0);
    CPPUNIT_ASSERT(wLoadedAi->hasMapEntryFor(wAIPosition,wAIVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition));
    wPuckPosition = Vecteur3(6,0,0);
    CPPUNIT_ASSERT(wLoadedAi->hasMapEntryFor(wAIPosition,wAIVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition));
    wPuckPosition = Vecteur3(78,0,0);
    CPPUNIT_ASSERT(!wLoadedAi->hasMapEntryFor(wAIPosition,wAIVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition));
    */
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


