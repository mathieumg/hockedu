//////////////////////////////////////////////////////////////////////////////
/// @file FacadeServeurMaitre.h
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include <map>
#include "..\Reseau\ObjetsGlobaux\JoueurServeurs.h"
#include "..\Reseau\ObjetsGlobaux\PartieServeurs.h"


///////////////////////////////////////////////////////////////////////////
/// @class FacadeServeurMaitre
/// @brief Classe qui représente la facade du serveur maitre
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class FacadeServeurMaitre :public Singleton<FacadeServeurMaitre>
{

    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(FacadeServeurMaitre);
public:
    bool saveJoueurConnecting(JoueurServeurs* pJoueur);

    void updateGameStatus(const PartieServeurs& pPartieInfos);

private:
    /// Constructeur par defaut
    FacadeServeurMaitre();
    /// Destructeur
    ~FacadeServeurMaitre();


    // Liste des parties en cours
    std::map<std::string, PartieServeurs*> mListeParties;

    // Liste des joueurs en attente de connection (attente de la reponse de la BD)
    std::map<std::string, JoueurServeurs*> mListeJoueursConnecting;

    // Liste des joueurs qui ont ete authentifies avec la BD
    std::map<std::string, JoueurServeurs*> mListeJoueursConnected;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

