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

    // Test du filepath donne et ouverture du fichier
    if(mOutputStream.is_open())
    {
        mOutputStream.close();
    }


    if(mInitDone)
    {
        // Si init deja fait, on dump avant pour etre certain qu'il ne reste pas d'infos dans le buffer
        dump();
    }

    mOutputStream.open(pFilePath, std::ios_base::binary | std::ios_base::out);

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





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



