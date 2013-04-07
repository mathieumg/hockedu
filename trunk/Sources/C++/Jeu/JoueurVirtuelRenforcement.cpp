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
JoueurVirtuelRenforcement::JoueurVirtuelRenforcement(const std::string& pAiLogicFilepath, const std::string& nom /*= ""*/, unsigned int vitesse /*= 0*/, unsigned int probabiliteEchec /*= 0*/)
    :JoueurVirtuel(nom, vitesse, probabiliteEchec), mAiLogicFilepath(pAiLogicFilepath)
{
	type_ = JOUEUR_VIRTUEL_RENFORCEMENT;
    chargerAiLogic(pAiLogicFilepath);
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
	/*XmlElement* elementNoeud = JoueurAbstrait::creerNoeudXML();

	elementNoeud->SetAttribute(JoueurAbstrait::etiquetteType.c_str(),type_);
	elementNoeud->SetAttribute("vitesse",obtenirVitesse());
	elementNoeud->SetAttribute("probEchec",obtenirProbabiliteEchec());

	return elementNoeud;*/
    return JoueurAbstrait::creerNoeudXML();
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
	/*if(!JoueurAbstrait::initialiser(element))
		return false;
	int elem;
	if( element->QueryIntAttribute("vitesse", &elem) != TIXML_SUCCESS )
		return false;
	modifierVitesse(elem);
	if( element->QueryIntAttribute("probEchec", &elem) != TIXML_SUCCESS )
		return false;
	modifierProbabiliteEchec(elem);
	type_ = JOUEUR_VIRTUEL;
	*/
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
    AiRuntimeInfosInput wInfos;
    Vecteur2i wVect8b;
    AILearner::convertirPositionUint8(pPositionAi.convertir<2>(), wVect8b);
    wInfos.positionAi[VX] = wVect8b[VX];
    wInfos.positionAi[VY] = wVect8b[VY];
    AILearner::convertirPositionUint8(pPositionJoueurAdverse.convertir<2>(), wVect8b);
    wInfos.positionAdversaire[VX] = wVect8b[VX];
    wInfos.positionAdversaire[VY] = wVect8b[VY];
    AILearner::convertirPositionUint8(pPositionRondelle.convertir<2>(), wVect8b);
    wInfos.positionRondelle[VX] = wVect8b[VX];
    wInfos.positionRondelle[VY] = wVect8b[VY];

    AILearner::convertirVelociteUint8(pVelociteAi.convertir<2>(), wVect8b);
    wInfos.velociteAi[VX] = wVect8b[VX];
    wInfos.velociteAi[VY] = wVect8b[VY];
    AILearner::convertirVelociteUint8(pVelociteRondelle.convertir<2>(), wVect8b);
    wInfos.velociteRondelle[VX] = wVect8b[VX];
    wInfos.velociteRondelle[VY] = wVect8b[VY];

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
    AiRuntimeInfosInput wInfos;
    Vecteur2i wVect8b;
    AILearner::convertirPositionUint8(pPositionAi.convertir<2>(), wVect8b);
    wInfos.positionAi[VX] = wVect8b[VX];
    wInfos.positionAi[VY] = wVect8b[VY];
    AILearner::convertirPositionUint8(pPositionJoueurAdverse.convertir<2>(), wVect8b);
    wInfos.positionAdversaire[VX] = wVect8b[VX];
    wInfos.positionAdversaire[VY] = wVect8b[VY];
    AILearner::convertirPositionUint8(pPositionRondelle.convertir<2>(), wVect8b);
    wInfos.positionRondelle[VX] = wVect8b[VX];
    wInfos.positionRondelle[VY] = wVect8b[VY];

    AILearner::convertirVelociteUint8(pVelociteAi.convertir<2>(), wVect8b);
    wInfos.velociteAi[VX] = wVect8b[VX];
    wInfos.velociteAi[VY] = wVect8b[VY];
    AILearner::convertirVelociteUint8(pVelociteRondelle.convertir<2>(), wVect8b);
    wInfos.velociteRondelle[VX] = wVect8b[VX];
    wInfos.velociteRondelle[VY] = wVect8b[VY];

    auto wIt = mActionMap.find(wInfos);
    return wIt != mActionMap.end();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



