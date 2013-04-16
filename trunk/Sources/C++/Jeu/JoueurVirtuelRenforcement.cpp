//////////////////////////////////////////////////////////////////////////////
/// @file JoueurVirtuelRenforcement.cpp
/// @author Mathieu Parent
/// @date 2013-04-1
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "JoueurVirtuelRenforcement.h"
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
JoueurVirtuelRenforcement::JoueurVirtuelRenforcement(const std::string& nom /*= ""*/, unsigned int vitesse /*= 0*/, unsigned int probabiliteEchec /*= 0*/, bool isLearning /*= true*/)
    :JoueurVirtuel(nom, vitesse, probabiliteEchec), mAiLearner(nom), mIsLearning(isLearning)
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
JoueurVirtuelRenforcement::~JoueurVirtuelRenforcement( void )
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
XmlElement* JoueurVirtuelRenforcement::creerNoeudXML() const
{
	XmlElement* elementNoeud = JoueurVirtuel::creerNoeudXML();

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
bool JoueurVirtuelRenforcement::initialiser( const XmlElement* element )
{
	if(!JoueurVirtuel::initialiser(element))
		return false;
	
    std::string path;
    if(XMLUtils::readAttribute(element,"path",path))
    {
        chargerAiLogic(path);
    }

	type_ = JOUEUR_VIRTUEL_RENFORCEMENT;
	
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurVirtuelRenforcement::genererAleatoirement()
///
/// Permet de generer des informations aleatoire pour un joueur,
/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void JoueurVirtuelRenforcement::genererAleatoirement()
{
    checkf(0, "Ne pas generer des joueurs virtuels avec apprentissage par renforcement aleatoirement")
	modifierNom("Joueur Virtuel Aleatoire");
	modifierVitesse(rand()%100);
	modifierProbabiliteEchec(rand()%100);
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
Vecteur2 JoueurVirtuelRenforcement::obtenirDirectionAI( NoeudMaillet* maillet )
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
bool JoueurVirtuelRenforcement::chargerAiLogic( const std::string& pAiLogicFilepath )
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



AiRuntimeInfosOutput JoueurVirtuelRenforcement::getActionFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse) const
{
    auto wInfos = convertInputData(pPositionAi, pPositionJoueurAdverse, pPositionRondelle, pVelociteAi, pVelociteRondelle);

    auto wIt = mActionMap.find(wInfos);
    if(wIt == mActionMap.end())
    {
        // Si l'apprentissage ne permet pas de determiner une action a effectuer, on utilise une valeur aleatoire
        return rand() % AI_ACTION_NB;
    }
    else
    {
        return wIt->second;
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
bool JoueurVirtuelRenforcement::hasMapEntryFor( const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse ) const
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
void JoueurVirtuelRenforcement::setPlayerSide( PlayerSide val )
{
    mAiLearner.setAISide(val);
    JoueurAbstrait::setPlayerSide(val);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuelRenforcement::setControlingMallet( class NoeudMaillet* pMallet )
///
/// Overridden function in order to be able to give the right information to the AILearner object.
///
/// @param[in] class NoeudMaillet * pMallet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void JoueurVirtuelRenforcement::setControlingMallet( NoeudMaillet* pMallet )
{
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
            //wDimensions[0] *= utilitaire::ratioWorldToBox2D;
            //wDimensions[1] *= utilitaire::ratioWorldToBox2D;

            mAiLearner.setMapTopLeft(wMapTopLeft.convertir<2>());
            mAiLearner.setMapBottomRight(wMapBottomRight.convertir<2>());
            mAiLearner.setMapDimensions(Vecteur2(wDimensions[0], wDimensions[1]));
            std::string wFieldName = wField->getNom();
            mAiLearner.setMapName(wFieldName.substr(wFieldName.find_last_of("/")+1));
        }
    }
    JoueurAbstrait::setControlingMallet(pMallet);
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
AiRuntimeInfosInput JoueurVirtuelRenforcement::convertInputData( const Vecteur3 &pPositionAi, const Vecteur3 &pPositionJoueurAdverse, const Vecteur3 &pPositionRondelle, const Vecteur3 &pVelociteAi, const Vecteur3 &pVelociteRondelle ) const
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
void JoueurVirtuelRenforcement::setupFinished()
{
    mAiLearner.setupFile();
    if(!isLearning())
    {
        chargerAiLogic(mAiLearner.getAiLogicFilePath());
    }
}

void JoueurVirtuelRenforcement::modifierNom( const std::string nom )
{
    JoueurAbstrait::modifierNom(nom);
    mAiLearner.setAINAme(nom);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



