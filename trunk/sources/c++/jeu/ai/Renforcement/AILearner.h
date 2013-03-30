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
#include "Singleton.h"
#include <stdint.h>
#include <fstream>

#define AI_LEARNER_OUTPUT_BUFFER_SIZE 5000 // Taille du buffer d'input
#define AI_LEARNER_MAX_VELOCITE_INPUT 100 // Velocite maximale en entree pour la conversion en uint8_t (ajuster en fonction des parametres de velocite passes)
#define AI_LEARNER_RAW_DATA_EXTENSION ".airaw" // Extension des fichiers d'output

struct LearningInfosRaw {
    // Input
    uint8_t positionAi[2]; // [0] = VX, [1] = VY
    int8_t velociteAi[2];
    uint8_t positionRondelle[2];
    int8_t velociteRondelle[2];
    uint8_t positionAdversaire[2];
    uint8_t action;

    // Output
    uint8_t resultat;
};


enum LearningAiAction {AI_ACTION_DEFENDRE, AI_ACTION_ATTAQUER_DIRECTEMENT, AI_ACTION_ATTAQUER_GAUCHE, AI_ACTION_ATTAQUER_DROITE};
enum LearningAiOutput {AI_OUTPUT_BUT_COMPTE, AI_OUTPUT_RIEN, AI_OUTPUT_ADVERSAIRE_BUT_COMPTE};

///////////////////////////////////////////////////////////////////////////
/// @class AILearner
/// @brief Manager pour l'apprentissage par renforcement
///
/// @author Mathieu Parent
/// @date 2013-03-29
///////////////////////////////////////////////////////////////////////////
class AILearner : public Singleton<AILearner>
{
    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(AILearner);
public:
	
	/// Destructeur
	~AILearner();

    // Init pour la classe. Mandatory
    bool init(const std::string& pFilePath, const Vecteur2& pMapTopLeft, const Vecteur2& pMapBottomRight);


    // Methode pour changer le repertoire d'output
    bool changerRepertoireOutput(const std::string& pFilePath);

    // Sauvegarde toutes les informations d'apprentissage qui sont dans le buffer dans le fichier specifie au init
    bool dump();

    // Ajoute la nouvelle information au buffer, mais ne l'ecrit pas necessairement sur le disque
    void sauvegarderNouvelleInfo(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse, LearningAiAction pAction);

    // Termine l'input de la nouvelle info
    void terminerSauvegardeNouvelleInfo(LearningAiOutput pOutput);






private:
	
    /// Constructeur
    AILearner();

    /// Methodes
    void convertirPositionUint8(const Vecteur2& pPositionAConvertir, Vecteur2i& pOut);
    void convertirVelociteUint8(const Vecteur2& pVelociteAConvertir, Vecteur2i& pOut);

    /// Attributs
    bool mInitDone;
    int mBufferCount;
    LearningInfosRaw mOutputBuffer[AI_LEARNER_OUTPUT_BUFFER_SIZE];
    Vecteur2 mMapTopLeft;
    Vecteur2 mMapBottomRight;
    Vecteur2 mMapDimensions;
    std::ofstream mOutputStream;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////