//////////////////////////////////////////////////////////////////////////////
/// @file JoueurVirtuelRenforcement.cpp
/// @author Mathieu Parent
/// @date 2013-04-1
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PlayerReinforcementAI.h"
#include "RazerGameUtilities.h"
#include "AIMaillet.h"
#include "Renforcement\AIMailletRenforcement.h"
#include <fstream>
#include "Renforcement\AILearner.h"
#include "NoeudMaillet.h"
#include "NoeudPoint.h"
#include "NoeudTable.h"
#include "Terrain.h"
#include "CompteurAffichage.h"
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::JoueurVirtuelRenforcement(const std::string& pAiLogicFilepath, const std::string& nom = "", unsigned int vitesse = 0, unsigned int probabiliteEchec = 0)
///
/// Constructeur qui initialise le joueur virtuel.
///
/// @param[in] pAiLogicFilepath : Path vers un fichier de type ailogic qui decrit le comportement du joueur
/// @param[in] nom : le nom du joueur.
/// @param[in] vitesse : la vitesse de déplacement du joueur.
/// @param[in] probabiliteEchec : la probabilité que le joueur manque son coup.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerReinforcementAI::PlayerReinforcementAI(const std::string& nom /*= ""*/, unsigned int vitesse /*= 0*/, unsigned int probabiliteEchec /*= 0*/, bool isLearning /*= true*/)
    :PlayerComputer(nom, vitesse, probabiliteEchec), mAiLearner(nom), mIsLearning(isLearning), mLearningCounter(0), mIsConversionDone(false)
{
	type_ = JOUEUR_VIRTUEL_RENFORCEMENT;
    setAiMaillet(new AIMailletRenforcement(this));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::~JoueurVirtuelRenforcement( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerReinforcementAI::~PlayerReinforcementAI( void )
{
	//delete aiMaillet_;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* JoueurVirtuelRenforcement::creerNoeudXML()
///
/// Creation du noeud XML du joueur
/// (***PAS UTILISE)
///
///
/// @return XmlElement* le noeud XML du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* PlayerReinforcementAI::creerNoeudXML() const
{
	XmlElement* elementNoeud = PlayerComputer::creerNoeudXML();

    XMLUtils::writeString(elementNoeud,"path",mAiLogicFilepath.c_str());

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurVirtuelRenforcement::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
/// (***PAS UTILISE)
///
/// @param[in] const XmlElement * element : élément contenant les informations pour créer le joueur
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool PlayerReinforcementAI::initialiser( const XmlElement* element )
{
	if(!PlayerComputer::initialiser(element))
		return false;
	
	type_ = JOUEUR_VIRTUEL_RENFORCEMENT;
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 JoueurVirtuelRenforcement::obtenirDirectionAI( NoeudMaillet* maillet )
///
/// Permet de savoir la nouvelle direction du maillet du JV selon AIMaillet
///
/// @param[in] NoeudRondelle * rondelle : un pointeur sur la rondelle 
/// pour savoir de quel côté de la ligne elle est
///
/// @return Vecteur2 :  la nouvelle direction à prendre pour le maillet
///
////////////////////////////////////////////////////////////////////////
Vecteur2 PlayerReinforcementAI::obtenirDirectionAI( NoeudMaillet* maillet )
{
	//envoie le pointeur sur la rondelle et sur le maillet
    auto ai = getAiMaillet();
    if(ai)
    {
        ai->evaluerStrategie(maillet);
        return ai->evaluerDirection(maillet);
    }
    return 0;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurVirtuelRenforcement::chargerAiLogic( const std::string& pAiLogicFilepath )
///
/// Permet de charger les informations de logique du Ai afin de les utiliser dans une partie
///
/// @param[in] pAiLogicFilepath : Path vers le fichier AiLogic a charger
///
/// @return bool    : Success ou echec
///
////////////////////////////////////////////////////////////////////////
bool PlayerReinforcementAI::chargerAiLogic( const std::string& pAiLogicFilepath )
{
    if(pAiLogicFilepath.find(AI_LEARNER_RUNTIME_DATA_EXTENSION) == std::string::npos)
    {
        return false;
    }

    std::ifstream wFile;
    wFile.open(pAiLogicFilepath, std::ios_base::binary | std::ios_base::in);

    if(wFile.fail())
    {
        wFile.close();
        return false;
    }

    // Fichier ouvert et valide
    AiRuntimeInfos wInfosBuffer;
    AiRuntimeInfosInput wInfosInputBuffer;
    while(true)
    {
        // Read data
        wFile.read((char*) &wInfosBuffer, sizeof(AiRuntimeInfos));

        
        // If read done, exit
        if(wFile.eof())
        {
            break;
        }
        // If fail, the file was corrupted and we could not read sizeof(AiRuntimeInfos) bytes
        else if(wFile.fail())
        {
            wFile.close();
            return false;
        }

        // Save the data inside a map
        memcpy_s(&wInfosInputBuffer, sizeof(AiRuntimeInfosInput), &wInfosBuffer, sizeof(AiRuntimeInfosInput));
        mActionMap[wInfosInputBuffer] = wInfosBuffer.action;
    }
    wFile.close();
    return true;
}



AiRuntimeInfosOutput PlayerReinforcementAI::getActionFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse)
{
    if(isLearning())
    {
        return (++mLearningCounter) % AI_ACTION_NB;
    }
    else
    {
        auto wInfos = convertInputData(pPositionAi, pPositionJoueurAdverse, pPositionRondelle, pVelociteAi, pVelociteRondelle);

        auto wIt = mActionMap.find(wInfos);
        if(wIt == mActionMap.end())
        {
#if !SHIPPING
            std::cout << "**** Pas d'apprentissage." << std::endl;
#endif
            // Si l'apprentissage ne permet pas de determiner une action a effectuer, on utilise une valeur aleatoire
            return rand() % AI_ACTION_NB;
        }
        else
        {
#if !SHIPPING
            std::cout << "**** Utilisation de l'apprentissage." << std::endl;
#endif
            return wIt->second;
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::hasMapEntryFor( const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse ) const
///
/// Returns if the AI has an entry for the specified data. Required for unit tests.
///
/// @param[in] const Vecteur3 & pPositionAi
/// @param[in] const Vecteur3 & pVelociteAi
/// @param[in] const Vecteur3 & pPositionRondelle
/// @param[in] const Vecteur3 & pVelociteRondelle
/// @param[in] const Vecteur3 & pPositionJoueurAdverse
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool PlayerReinforcementAI::hasMapEntryFor( const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse ) const
{
    auto wInfos = convertInputData(pPositionAi, pPositionJoueurAdverse, pPositionRondelle, pVelociteAi, pVelociteRondelle);

    auto wIt = mActionMap.find(wInfos);
    return wIt != mActionMap.end();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::setPlayerSide( PlayerSide val )
///
/// Overridden function in order to be able to give the right information to the AILearner object.
///
/// @param[in] PlayerSide val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PlayerReinforcementAI::setPlayerSide( PlayerSide val )
{
    mAiLearner.setAISide(val);
    PlayerAbstract::setPlayerSide(val);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::convertInputData( const Vecteur3 &pPositionAi, const Vecteur3 &pPositionJoueurAdverse, const Vecteur3 &pPositionRondelle, const Vecteur3 &pVelociteAi, const Vecteur3 &pVelociteRondelle ) const
///
/// Description
///
/// @param[in] const Vecteur3 & pPositionAi
/// @param[in] const Vecteur3 & pPositionJoueurAdverse
/// @param[in] const Vecteur3 & pPositionRondelle
/// @param[in] const Vecteur3 & pVelociteAi
/// @param[in] const Vecteur3 & pVelociteRondelle
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
AiRuntimeInfosInput PlayerReinforcementAI::convertInputData( const Vecteur3 &pPositionAi, const Vecteur3 &pPositionJoueurAdverse, const Vecteur3 &pPositionRondelle, const Vecteur3 &pVelociteAi, const Vecteur3 &pVelociteRondelle ) const
{
    AiRuntimeInfosInput wInfos;
    Vecteur2i wVect8b;
    mAiLearner.convertirPositionUint8(pPositionAi.convertir<2>(), wVect8b);
    wInfos.positionAi[VX] = wVect8b[VX];
    wInfos.positionAi[VY] = wVect8b[VY];
    mAiLearner.convertirPositionUint8(pPositionJoueurAdverse.convertir<2>(), wVect8b);
    wInfos.positionAdversaire[VX] = wVect8b[VX];
    wInfos.positionAdversaire[VY] = wVect8b[VY];
    mAiLearner.convertirPositionUint8(pPositionRondelle.convertir<2>(), wVect8b);
    wInfos.positionRondelle[VX] = wVect8b[VX];
    wInfos.positionRondelle[VY] = wVect8b[VY];

    mAiLearner.convertirVelociteUint8(pVelociteAi.convertir<2>(), wVect8b);
    wInfos.velociteAi[VX] = wVect8b[VX];
    wInfos.velociteAi[VY] = wVect8b[VY];
    mAiLearner.convertirVelociteUint8(pVelociteRondelle.convertir<2>(), wVect8b);
    wInfos.velociteRondelle[VX] = wVect8b[VX];
    wInfos.velociteRondelle[VY] = wVect8b[VY];

    return wInfos;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::setupFinished()
///
/// Description
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PlayerReinforcementAI::setupFinished()
{
    mAiLearner.setupFile();
    if(!isLearning())
    {
        chargerAiLogic(mAiLearner.getAiLogicFilePath());
    }
    else
    {
        mIsConversionDone = false;
    }
}

void PlayerReinforcementAI::modifierNom( const std::string nom )
{
    PlayerAbstract::modifierNom(nom);
    mAiLearner.setAINAme(nom);
}



void PlayerReinforcementAI::gameInit()
{
    auto pMallet = getControlingMallet();
    if(pMallet)
    {
        Terrain* wField = pMallet->getField();
        if(wField)
        {
            NoeudTable* wTable = wField->getTable();
            auto wMapTopLeft = wTable->obtenirPoint(POSITION_HAUT_GAUCHE)->getPosition();
            auto wMapBottomRight = wTable->obtenirPoint(POSITION_BAS_DROITE)->getPosition();
            float wDimensions[2];
            wTable->calculerHautLongMax(wDimensions);
            mAiLearner.setMapTopLeft(wMapTopLeft.convertir<2>());
            mAiLearner.setMapBottomRight(wMapBottomRight.convertir<2>());
            mAiLearner.setMapDimensions(Vecteur2(wDimensions[1], wDimensions[0])*2.0f);
            std::string wFieldName = wField->getNom();
            mAiLearner.setMapName(wFieldName.substr(wFieldName.find_last_of("/")+1));
        }
    }

    setIsLearning(false);
    setupFinished();
}

void PlayerReinforcementAI::PlayTick( float time )
{
	auto maillet = getControlingMallet();
	if(maillet)
	{
		auto newPos = obtenirDirectionAI(maillet);
		if(!newPos.estNul())
		{
			newPos += maillet->getPosition();
			maillet->setTargetDestination(newPos);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



