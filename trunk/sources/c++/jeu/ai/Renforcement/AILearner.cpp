//////////////////////////////////////////////////////////////////////////////
/// @file AILearner.cpp
/// @author Mathieu Parent
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AILearner.h"
#include "DirectoySearch.h"
#include <map>
#include "PlayerAbstract.h"
#include <sstream>

int AILearner::mStepVelocite = 5;//20
int AILearner::mStepPosition = 8;//100

////////////////////////////////////////////////////////////////////////
///
/// @fn  AILearner::AILearner( )
///
/// Constructeur
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AILearner::AILearner(const std::string& pAiName)
    : mInitDone(false), mBufferCount(0), mHandleThreadConversion(NULL), mSavingData(false)
{
    FacadePortability::createDirectory("AIData");
    setAINAme(pAiName);
    resetPathStringStream();
}

bool AILearner::setupFile()
{
    resetPathStringStream();
    //File name = mapName_AISide.airaw
    
    mFilePath << "/" << mMapName << "_" << (mAISide == PLAYER_SIDE_LEFT ? "left" : "right") << AI_LEARNER_RAW_DATA_EXTENSION;

//#if !SHIPPING
    std::cout << "Full path: " << mFilePath.str() << std::endl;
//#endif

    //Change output directory.
    if(changerRepertoireOutput(mFilePath.str()))
    {
        mInitDone = true;
    }
    return mInitDone;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AILearner::~AILearner()
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AILearner::~AILearner()
{
    if(mOutputStream.is_open())
    {
        mOutputStream.close();
    }

}

bool AILearner::changerRepertoireOutput( const std::string& pFilePath )
{
    if(pFilePath.find(AI_LEARNER_RAW_DATA_EXTENSION) == -1)
    {
        // Mauvaise extension
        return false;
    }

    if(mInitDone)
    {
        // Si init deja fait, on dump avant pour etre certain qu'il ne reste pas d'infos dans le buffer
        dump();
    }

    // Test du filepath donne et ouverture du fichier
    if(mOutputStream.is_open())
    {
        mOutputStream.close();
    }

    mOutputStream.open(pFilePath, std::ios_base::binary | std::ios_base::out | std::ios_base::app);

    if(mOutputStream.fail())
    {
        return false;
    }
    
    // Changement de repertoire termine
    return true;
}



// Si on dump et le terminerSauvegardeNouvelleInfo n'a pas ete appele, on n'aura pas la derniere info ajoutee
bool AILearner::dump()
{
    int wSize = mBufferCount*sizeof(LearningInfosRaw)/sizeof(char);
    mOutputStream.write((char*) mOutputBuffer, wSize);
    mOutputStream.flush();
    mBufferCount = 0;
    return true;
}



void AILearner::sauvegarderNouvelleInfo( const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse, LearningAiAction pAction )
{
    if(mBufferCount == AI_LEARNER_OUTPUT_BUFFER_SIZE)
    {
        // Buffer plein, on doit dump avant de sauvegarder
        dump();
    }

    // Contertir les informations passees en parametre pour les sauvegarder en uint8_t et les mettre dans le buffer
    
    // Calcul des positions
    Vecteur2i wPos8b;
    convertirPositionUint8((Vecteur2) pPositionAi, wPos8b);
    mOutputBuffer[mBufferCount].positionAi[VX] = wPos8b[VX];
    mOutputBuffer[mBufferCount].positionAi[VY] = wPos8b[VY];

    convertirPositionUint8((Vecteur2) pPositionRondelle, wPos8b);
    mOutputBuffer[mBufferCount].positionRondelle[VX] = wPos8b[VX];
    mOutputBuffer[mBufferCount].positionRondelle[VY] = wPos8b[VY];

    convertirPositionUint8((Vecteur2) pPositionJoueurAdverse, wPos8b);
    mOutputBuffer[mBufferCount].positionAdversaire[VX] = wPos8b[VX];
    mOutputBuffer[mBufferCount].positionAdversaire[VY] = wPos8b[VY];

    // Calcul des velocites
    Vecteur2i wVel8b;
    convertirVelociteUint8((Vecteur2) pVelociteRondelle, wVel8b);
    mOutputBuffer[mBufferCount].velociteRondelle[VX] = wVel8b[VX];
    mOutputBuffer[mBufferCount].velociteRondelle[VY] = wVel8b[VY];

    convertirVelociteUint8((Vecteur2) pVelociteAi, wVel8b);
    mOutputBuffer[mBufferCount].velociteAi[VX] = wVel8b[VX];
    mOutputBuffer[mBufferCount].velociteAi[VY] = wVel8b[VY];

    mOutputBuffer[mBufferCount].action = (int) pAction;
    
    mSavingData = true;
}


void AILearner::terminerSauvegardeNouvelleInfo( LearningAiOutput pOutput )
{
    if(mSavingData)
    {
        mOutputBuffer[mBufferCount].resultat = pOutput;

        // Incrementer le compteur pour que le prochaine sauvegarde se fasse dans le prochain objet
        ++mBufferCount;
    }
    mSavingData = false;
}





void AILearner::convertirPositionUint8( const Vecteur2& pPositionAConvertir, Vecteur2i& pOut ) const
{
    // Convertir la position en coordonnee (0-255, 0-255) grid
    // La map est separee en 255 cases en X et 255 en Y. De cette facon, on peut utiliser des uint8_t pour sauvegarder les positions

    // Si la dimension est nulle, c'est louche
    checkf(mMapDimensions[VX] > 1.0f && mMapDimensions[VY] > 1.0f);

    pOut[VX] = (int) (AILearner::mStepPosition*(pPositionAConvertir[VX]-mMapTopLeft[VX])/mMapDimensions[VX]);
    pOut[VY] = (int) (AILearner::mStepPosition*(mMapTopLeft[VY]-pPositionAConvertir[VY])/mMapDimensions[VY]);


}



void AILearner::convertirVelociteUint8( const Vecteur2& pVelociteAConvertir, Vecteur2i& pOut )
{
    // Conversion d'une velocite en velocite relative sur 8bits
    if(abs(pVelociteAConvertir[VX]) >= AI_LEARNER_MAX_VELOCITE_INPUT)
    {
        int wSignX = (int) (pVelociteAConvertir[VX] / abs(pVelociteAConvertir[VX]));
        pOut[VX] = AILearner::mStepVelocite * wSignX; 
    }
    else
    {
        pOut[VX] = (int) (((float)AILearner::mStepVelocite * pVelociteAConvertir[VX]) / AI_LEARNER_MAX_VELOCITE_INPUT);
    }

    if(abs(pVelociteAConvertir[VY]) >= AI_LEARNER_MAX_VELOCITE_INPUT)
    {
        int wSignY = (int) (pVelociteAConvertir[VY] / abs(pVelociteAConvertir[VY]));
        pOut[VY] = AILearner::mStepVelocite * wSignY;
    }
    else
    {
        pOut[VY] = (int) (((float)AILearner::mStepVelocite * pVelociteAConvertir[VY]) / AI_LEARNER_MAX_VELOCITE_INPUT);
    }
}


bool AILearner::convertirDonneesRaw( AiLearnerBuildReadyCallback pCallback)
{
    DWORD returnValue;
    GetExitCodeThread(mHandleThreadConversion,&returnValue);
    if(!pCallback || returnValue == STILL_ACTIVE)
    {
        // Fail si pas de callback ou si thread deja en cours d'execution
        return false;
    }

    AiLearnerConvertionThreadParams* wParams = new AiLearnerConvertionThreadParams;
    std::string wFilePath(mFilePath.str());
    wParams->thisPtr = this;
    //wParams->folderPath = wFilePath.substr(2, wFilePath.find_last_not_of("/"));
    wParams->callback = pCallback;
    wParams->outputFile = getAiLogicFilePath();
    wParams->inputFile = mFilePath.str();
    FacadePortability::createThread(mHandleThreadConversion, convertirDonneesRawThread, wParams);
    GetExitCodeThread(mHandleThreadConversion,&returnValue);
    if(returnValue != STILL_ACTIVE)
    {
        // Fail a la creation du thread
        return false;
    }

    return true;
}



void * AILearner::convertirDonneesRawThread( void *arg )
{
    AiLearnerConvertionThreadParams* wParams = (AiLearnerConvertionThreadParams*) arg;


    // Faire la conversion
    std::string wOutputFile = wParams->outputFile;
    size_t wIndexExtension = wOutputFile.find(AI_LEARNER_RUNTIME_DATA_EXTENSION);
    if(wIndexExtension == std::string::npos)
    {
        // Pas d'extension, on en met une
        size_t wDotIndex = wOutputFile.find(".");
        if(wDotIndex == std::string::npos)
        {
            wOutputFile += AI_LEARNER_RUNTIME_DATA_EXTENSION;
        }
        else
        {
            wOutputFile = wOutputFile.substr(0, wDotIndex) + AI_LEARNER_RUNTIME_DATA_EXTENSION;
        }
    }

    std::ofstream wOutput;
//    wOutput.open(wParams->folderPath + "/" + wOutputFile);
    wOutput.open(wOutputFile);
    if(wOutput.fail())
    {
        wParams->callback(false);
    }
    else
    {
/*
        // Conversion
        std::vector<std::string> wFilesList;
        std::map<std::string, bool> wExtMap;
        std::string wExtLogic = std::string(AI_LEARNER_RAW_DATA_EXTENSION).substr(1);
        wExtMap[wExtLogic] = true;
        if(SearchDirectory(wFilesList, wParams->folderPath, wExtMap, false) !=0)
        {
            // Erreur, abort
            wParams->callback(false);
        }
        else
        {*/
            // Pas d'erreur, on continue

            // On se cree une hashmap pour sauvegarder les points des differentes entrees
            std::map<AiRuntimeInfos, AiLearningComputingValue*> wMapData;

            // Lecture des infos de chaque fichier
//            for(auto it = wFilesList.begin(); it!=wFilesList.end(); ++it)
//            {
                // On ouvre le fichier
                std::ifstream wFichier;
//                wFichier.open((*it), std::ios_base::binary | std::ios_base::in);
                wFichier.open(wParams->inputFile, std::ios_base::binary | std::ios_base::in);
                LearningInfosRaw wBuffer;
                AiRuntimeInfos wBufferRuntime;
                int wValueToAdd = 0;
                if(!wFichier.fail())
                {
                    // On trouve la taille totale du fichier
                    wFichier.seekg(0, std::ios::end);
                    int wFilesize = (int) wFichier.tellg();
                    wFichier.seekg(0, std::ios::beg);
                    int wPourcentage = 0;
                    int wDataCount = 0;

                    // Ouverture reussie, on lit le fichier
                    while(!wFichier.eof())
                    {
                        wFichier.read((char*) &wBuffer, sizeof(LearningInfosRaw));
                        if(wFichier.eof())
                        {
                            break;
                        }

                        memcpy_s(&wBufferRuntime, sizeof(AiRuntimeInfos), &wBuffer, sizeof(AiRuntimeInfos));

                        // On traite cette info
                        auto it = wMapData.find(wBufferRuntime);
                        if(it == wMapData.end())
                        {
                            // n'existe pas, on cree la valeur
                            AiLearningComputingValue* wValue = new AiLearningComputingValue(getPointsForResult((LearningAiOutput) wBuffer.resultat), 1);
                            wMapData[wBufferRuntime] = wValue;
                        }
                        else
                        {
                            // Existe deja, on met a jour les infos
                            it->second->somme += getPointsForResult((LearningAiOutput) wBuffer.resultat);
                            ++(it->second->nbEntrees);
                        }

                        // On affiche le pourcentage
                        wDataCount++;
                        int wNewPourcentage = wDataCount * 100 * sizeof(LearningInfosRaw) / wFilesize;
                        if(wPourcentage != wNewPourcentage)
                        {
                            std::cout << "Conversion " << wParams->inputFile << " passe 1: " << wNewPourcentage << "%" << std::endl;
                        }
                        wPourcentage = wNewPourcentage;

                    }

                    // Lecture terminee, fichier suivant
 //               }
//            }

            std::cout << std::endl << std::endl;

            // Pour chaque action, on trouve la meilleure action a effectuer
            AiRuntimeInfos wLastInfosChecked;
            AiRuntimeInfos wBufferCompare;
            int wPourcentage2 = 0;
            int wCount = 0;
            int wNbObjects = (int) wMapData.size();
            for(auto it = wMapData.begin(); it!=wMapData.end(); ++it)
            {
                // On teste pour voir si on vient de traiter l'entree avec seulement l'action differente
                wBufferCompare = it->first;
                if(memcmp(&wBufferCompare, &wLastInfosChecked, sizeof(AiRuntimeInfos)-1))
                {
                    float wMoyenneMax = -10000000.0f;
                    LearningAiAction wBestAction = AI_ACTION_DEFENDRE;
                    for(int i=0; i<AI_ACTION_NB; ++i)
                    {
                        wBufferCompare.action = (LearningAiAction) i;
                        auto itMap = wMapData.find(wBufferCompare);
                        if(itMap != wMapData.end())
                        {
                            float wMoyenne = (float)itMap->second->somme / (float)itMap->second->nbEntrees;
                            if(wMoyenne > wMoyenneMax)
                            {
                                // Valeur la plus faible
                                wMoyenneMax = wMoyenne;
                                wBestAction = (LearningAiAction) i;
                            }
                        }
                    }


                    // On sauvegarde les informations dans le fichier de destination
                    wBufferCompare.action = wBestAction;
                    wOutput.write((char*) &wBufferCompare, sizeof(AiRuntimeInfos));
                }
                wLastInfosChecked = it->first;

                wCount++;
                int wNewPourcentage = wCount * 100 / wNbObjects;
                if(wPourcentage2 != wNewPourcentage)
                {
                    std::cout << "Conversion " << wParams->inputFile << " passe 2: " << wNewPourcentage << "%" << std::endl;
                }
                wPourcentage2 = wNewPourcentage;
            }

            wOutput.flush();
            wOutput.close();
            

            // Delete les elements de la map
            for(auto it = wMapData.begin(); it!=wMapData.end(); ++it)
            {
                delete it->second;
            }

            // Conversion terminee, on appelle le callback et on signal au AILearner que le traitement est termine
            if(wParams->callback)
            {
                (wParams->callback)(true);
            }
        }
    }


    // Reset the thread handle to signal the thread is done
    //AILearner::conversionThreadDone();
    delete wParams;
    return 0;
}



int AILearner::getPointsForResult(LearningAiOutput pResult)
{
    switch(pResult)
    {
    case AI_OUTPUT_BUT_COMPTE:
        return 10;
        break;
    case AI_OUTPUT_ADVERSAIRE_BUT_COMPTE:
        return -10;
        break;
	case AI_OUTPUT_ADVERSAIRE_PAS_TOUCHE:
		return 1;
		break;
    default:
        return -1;
        break;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AILearner::setAINAme( std::string pName )
///
/// Description
///
/// @param[in] std::string pName
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AILearner::setAINAme( std::string pAiName )
{
    mPlayerName = pAiName;
    // Maintenant dans resetPathStringStream()
//     if(pAiName != "")
//     {
//         mFilePath << pAiName; // Path = AIData/AI Name
// #if !SHIPPING
//         std::cout << "Sub path: " <<mFilePath.str() << std::endl;
// #endif
//         FacadePortability::createDirectory(mFilePath.str().substr(2).c_str()); // Create the AIData/AI name directory
//     }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



