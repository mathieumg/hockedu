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

HANDLE_THREAD AILearner::mHandleThreadConversion = NULL;

bool operator < (const AiRuntimeInfos &pLeft, const AiRuntimeInfos &pRight)
{ 
    return memcmp(&pLeft,&pRight,sizeof(AiRuntimeInfos)) < 0;
}

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(AILearner);

////////////////////////////////////////////////////////////////////////
///
/// @fn  AILearner::AILearner( )
///
/// Constructeur
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AILearner::AILearner()
    : mInitDone(false), mBufferCount(0)
{

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






bool AILearner::init( const std::string& pFilePath, const Vecteur2& pMapTopLeft, const Vecteur2& pMapBottomRight)
{
    if(changerRepertoireOutput(pFilePath))
    {
        // largeur et hauteur de la map
        mMapDimensions = Vecteur2(pMapBottomRight[VX]-pMapTopLeft[VX], pMapTopLeft[VY]-pMapBottomRight[VY]);
        mMapTopLeft = pMapTopLeft;
        mMapBottomRight = pMapBottomRight;
        mInitDone = true;
        return true;
    }
    return false;
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
    
}


void AILearner::terminerSauvegardeNouvelleInfo( LearningAiOutput pOutput )
{
    mOutputBuffer[mBufferCount].resultat = pOutput;

    // Incrementer le compteur pour que le prochaine sauvegarde se fasse dans le prochain objet
    ++mBufferCount;
}





void AILearner::convertirPositionUint8( const Vecteur2& pPositionAConvertir, Vecteur2i& pOut )
{
    // Convertir la position en coordonnee (0-255, 0-255) grid
    // La map est separee en 255 cases en X et 255 en Y. De cette facon, on peut utiliser des uint8_t pour sauvegarder les positions

    pOut[VX] = (int) (255.0f*(pPositionAConvertir[VX]-mMapTopLeft[VX])/mMapDimensions[VX]);
    pOut[VY] = (int) (255.0f*(mMapTopLeft[VY]-pPositionAConvertir[VY])/mMapDimensions[VY]);


}



void AILearner::convertirVelociteUint8( const Vecteur2& pVelociteAConvertir, Vecteur2i& pOut )
{
    
    

    // Conversion d'une velocite en velocite relative sur 8bits
    if(abs(pVelociteAConvertir[VX]) >= AI_LEARNER_MAX_VELOCITE_INPUT)
    {
        int wSignX = (int) (pVelociteAConvertir[VX] / abs(pVelociteAConvertir[VX]));
        pOut[VX] = 127 * wSignX; 
    }
    else
    {
        pOut[VX] = (int) ((127.0f * pVelociteAConvertir[VX]) / AI_LEARNER_MAX_VELOCITE_INPUT);
    }

    if(abs(pVelociteAConvertir[VY]) >= AI_LEARNER_MAX_VELOCITE_INPUT)
    {
        int wSignY = (int) (pVelociteAConvertir[VY] / abs(pVelociteAConvertir[VY]));
        pOut[VY] = 127 * wSignY;
    }
    else
    {
        pOut[VY] = (int) ((127.0f * pVelociteAConvertir[VY]) / AI_LEARNER_MAX_VELOCITE_INPUT);
    }
    


}



bool AILearner::convertirDonneesRaw( const std::string& pFolderPath, const std::string& pOutputFilename, AiLearnerBuildReadyCallback pCallback)
{
    if(!pCallback || mHandleThreadConversion)
    {
        // Fail si pas de callback ou si thread deja en cours d'execution
        return false;
    }

    AiLearnerConvertionThreadParams* wParams = new AiLearnerConvertionThreadParams;
    wParams->thisPtr = AILearner::obtenirInstance();
    wParams->folderPath = pFolderPath;
    wParams->callback = pCallback;
    wParams->outputFilename = pOutputFilename;

    FacadePortability::createThread(mHandleThreadConversion, convertirDonneesRawThread, wParams);
    if(mHandleThreadConversion==NULL)
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
    std::string wOutputFilename = wParams->outputFilename;
    size_t wIndexExtension = wOutputFilename.find(AI_LEARNER_RUNTIME_DATA_EXTENSION);
    if(wIndexExtension == std::string::npos)
    {
        // Pas d'extension, on en met une
        size_t wDotIndex = wOutputFilename.find(".");
        if(wDotIndex == std::string::npos)
        {
            wOutputFilename += AI_LEARNER_RUNTIME_DATA_EXTENSION;
        }
        else
        {
            wOutputFilename = wOutputFilename.substr(0, wDotIndex) + AI_LEARNER_RUNTIME_DATA_EXTENSION;
        }
    }

    std::ofstream wOutput;
    wOutput.open(wParams->folderPath + "/" + wOutputFilename);

    if(wOutput.fail())
    {
        wParams->callback(false);
    }
    else
    {
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
        {
            // Pas d'erreur, on continue

            // On se cree une hashmap pour sauvegarder les points des differentes entrees
            std::map<AiRuntimeInfos, AiLearningComputingValue*> wMapData;

            // Lecture des infos de chaque fichier
            for(auto it = wFilesList.begin(); it!=wFilesList.end(); ++it)
            {
                // On ouvre le fichier
                std::ifstream wFichier;
                wFichier.open((*it), std::ios_base::binary | std::ios_base::in);
                LearningInfosRaw wBuffer;
                AiRuntimeInfos wBufferRuntime;
                int wValueToAdd = 0;
                if(!wFichier.fail())
                {
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
                    }

                    // Lecture terminee, fichier suivant
                }
            }



            // Pour chaque action, on trouve la meilleure action a effectuer
            AiRuntimeInfos wLastInfosChecked;
            AiRuntimeInfos wBufferCompare;
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
            }

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
    AILearner::conversionThreadDone();
    delete wParams;
    return 0;
}



int AILearner::getPointsForResult(LearningAiOutput pResult)
{
    switch(pResult)
    {
    case AI_OUTPUT_BUT_COMPTE:
        return 1;
        break;
    case AI_OUTPUT_ADVERSAIRE_BUT_COMPTE:
        return -1;
        break;
    default:
        return 0;
        break;
    }
}







///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



