//////////////////////////////////////////////////////////////////////////////
/// @file AILearner.h
/// @author Mathieu Parent
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"
#include <stdint.h>
#include <fstream>
#include <sstream>
#include "..\Reseau\FacadePortability.h"
#include <functional>
#include <iostream>

#define AI_LEARNER_OUTPUT_BUFFER_SIZE 5000 // Taille du buffer d'input
#define AI_LEARNER_MAX_VELOCITE_INPUT 1 // 100 Velocite maximale en entree pour la conversion en uint8_t (ajuster en fonction des parametres de velocite passes)
#define AI_LEARNER_RAW_DATA_EXTENSION ".airaw" // Extension des fichiers d'output
#define AI_LEARNER_RUNTIME_DATA_EXTENSION ".ailogic" // Extension des fichiers d'output runtime

enum PlayerSide;

struct LearningInfosRaw {
    // Input
    uint8_t positionAi[2]; // [0] = VX, [1] = VY
    int8_t  velociteAi[2];
    uint8_t positionRondelle[2];
    int8_t  velociteRondelle[2];
    uint8_t positionAdversaire[2];
    uint8_t action;

    // Output
    uint8_t resultat;
};

typedef std::function<int (bool)> AiLearnerBuildReadyCallback;
class AILearner;
struct AiLearnerConvertionThreadParams
{
    AiLearnerBuildReadyCallback callback;
    std::string folderPath;
    std::string outputFile;
    std::string inputFile;
    AILearner* thisPtr;
};


struct AiRuntimeInfos 
{
    // Input
    uint8_t positionAi[2]; // [0] = VX, [1] = VY
    int8_t  velociteAi[2];
    uint8_t positionRondelle[2];
    int8_t  velociteRondelle[2];
    uint8_t positionAdversaire[2];

    // Output
    uint8_t action;

    bool operator < ( const AiRuntimeInfos &pRight) const
    { 
        return memcmp(this,&pRight,sizeof(AiRuntimeInfos)) < 0;
    }
};


struct AiRuntimeInfosInput
{
    uint8_t positionAi[2]; // [0] = VX, [1] = VY
    int8_t  velociteAi[2];
    uint8_t positionRondelle[2];
    int8_t  velociteRondelle[2];
    uint8_t positionAdversaire[2];

    bool operator < ( const AiRuntimeInfosInput &pRight) const 
    { 
        return memcmp(this,&pRight,sizeof(AiRuntimeInfosInput)) < 0;
    }
};

typedef uint8_t AiRuntimeInfosOutput;

struct AiLearningComputingValue
{
    AiLearningComputingValue(int pSomme, int pNbEntrees)
    {
        somme = pSomme;
        nbEntrees = pNbEntrees;
    }
    int somme;
    int nbEntrees;
};

enum LearningAiAction {AI_ACTION_DEFENDRE, AI_ACTION_ATTAQUER_DIRECTEMENT, AI_ACTION_ATTAQUER_GAUCHE, AI_ACTION_ATTAQUER_DROITE, AI_ACTION_NB};
enum LearningAiOutput {AI_OUTPUT_BUT_COMPTE, AI_OUTPUT_RIEN, AI_OUTPUT_ADVERSAIRE_BUT_COMPTE, AI_OUTPUT_ADVERSAIRE_PAS_TOUCHE};



///////////////////////////////////////////////////////////////////////////
/// @class AILearner
/// @brief Manager pour l'apprentissage par renforcement
///
/// @author Mathieu Parent
/// @date 2013-03-29
///////////////////////////////////////////////////////////////////////////
class AILearner
{
public:
    AILearner(const std::string& pAiName);

	/// Destructeur
	~AILearner();

    bool setupFile();

    // Methode pour changer le repertoire d'output
    bool changerRepertoireOutput(const std::string& pFilePath);

    // Sauvegarde toutes les informations d'apprentissage qui sont dans le buffer dans le fichier specifie au init
    bool dump();

    // Ajoute la nouvelle information au buffer, mais ne l'ecrit pas necessairement sur le disque
    void sauvegarderNouvelleInfo(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse, LearningAiAction pAction);

    // Termine l'input de la nouvelle info
    void terminerSauvegardeNouvelleInfo(LearningAiOutput pOutput);


    // Utilise les fichiers dans le dossier specifie pour construire les donnees utilisables pour jouer avec l'AI
    bool convertirDonneesRaw(AiLearnerBuildReadyCallback pCallback);

    static int getPointsForResult(LearningAiOutput pResult);
    
    void convertirPositionUint8(const Vecteur2& pPositionAConvertir, Vecteur2i& pOut) const;
    static void convertirVelociteUint8(const Vecteur2& pVelociteAConvertir, Vecteur2i& pOut);

    inline const Vecteur2& getMapTopLeft() const { return mMapTopLeft; }
    inline void setMapTopLeft(const Vecteur2& val) { mMapTopLeft = val; }

    inline const Vecteur2& getMapBottomRight() const { return mMapBottomRight; }
    inline void setMapBottomRight(const Vecteur2& val) { mMapBottomRight = val; }

    inline const Vecteur2& getMapDimensions() const { return mMapDimensions; }
    inline void setMapDimensions(const Vecteur2& val) { mMapDimensions = val; }

    inline void setAISide(const PlayerSide& pAISide) { mAISide = pAISide; }
    inline void setMapName(const std::string& pMapName) { mMapName = pMapName; }

    std::string getAiLogicFilePath() { 
        std::string wFilePath = mFilePath.str(); 
        return wFilePath.replace(wFilePath.find(AI_LEARNER_RAW_DATA_EXTENSION), strlen(AI_LEARNER_RAW_DATA_EXTENSION), AI_LEARNER_RUNTIME_DATA_EXTENSION);
    }

    inline void resetPathStringStream() {
        mFilePath = std::ostringstream();
        mFilePath << "./AIData/" << mPlayerName;
        FacadePortability::createDirectory(mFilePath.str().substr(2).c_str()); // Create the AIData/AI name directory
    }

    inline void setAINAme(std::string pAiName);
private:
	
    // Steps pour conversion
    static int mStepPosition;
    static int mStepVelocite;

    PlayerSide mAISide;
    std::string mMapName;
    std::ostringstream mFilePath;
    std::string mPlayerName;

    /// Methodes
    //static void conversionThreadDone() {mHandleThreadConversion = NULL;}

    /// Threads
    // Code d'execution du thread de conversion des donnes raw en donnees utilisables
   static void* convertirDonneesRawThread(void *arg);

    /// Attributs
    bool mInitDone;
    int mBufferCount;
    LearningInfosRaw mOutputBuffer[AI_LEARNER_OUTPUT_BUFFER_SIZE];

    Vecteur2 mMapTopLeft;
    Vecteur2 mMapBottomRight;
    Vecteur2 mMapDimensions;
    bool mSavingData;

    std::ofstream mOutputStream;
    HANDLE_THREAD mHandleThreadConversion;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////