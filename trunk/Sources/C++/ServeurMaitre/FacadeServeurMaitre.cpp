///////////////////////////////////////////////////////////////////////////////
/// @file FacadeServeurMaitre.cpp
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "FacadeServeurMaitre.h"
#include "CommunicateurBD.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(FacadeServeurMaitre);

////////////////////////////////////////////////////////////////////////
///
/// @fn  FacadeServeurMaitre::FacadeServeurMaitre(  )
///
/// Constructeur
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
FacadeServeurMaitre::FacadeServeurMaitre()
{
    /*std::list<std::pair<std::string, std::string>> wUsersToTest;
    wUsersToTest.push_back(std::pair<std::string, std::string>("bob", "sdsd"));
    wUsersToTest.push_back(std::pair<std::string, std::string>("testing", "testing"));
    wUsersToTest.push_back(std::pair<std::string, std::string>("testing", "yolo"));

    for(auto it = wUsersToTest.begin(); it != wUsersToTest.end(); ++it)
    {
        std::string& wUsername = (*it).first;
        std::string& wPassword = (*it).second;
        std::ostringstream oss;
        oss << "Testing connection with user " << wUsername << " and password " << wPassword << ":" << std::endl;
        int wUserId;
        if((wUserId = CommunicateurBD::obtenirInstance()->authenticate(wUsername, wPassword)) == -1)
        {
            oss << "Combination of user " << wUsername << " and password " << wPassword << " not found." << std::endl;
        }
        else
        {
            oss << "User " << wUsername << " connected successfully! User ID: " << wUserId << std::endl;
        }
        std::cout << oss.str() << std::endl;
    }*/
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  FacadeServeurMaitre::~FacadeServeurMaitre(  )
///
/// Destructeur
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
FacadeServeurMaitre::~FacadeServeurMaitre()
{
    emptyMemory();
}


// Retourne true si le joueur a bien pu etre sauvegarder, false si un joueur avec le meme nom est deja en train de se connecter
bool FacadeServeurMaitre::savePlayerConnecting( JoueurServeurs* pJoueur )
{
    JoueurServeurs* wJoueurDansTable = mListeJoueursConnecting[pJoueur->getIdentifier()];
    mListeJoueursConnecting[pJoueur->getIdentifier()] = pJoueur;
    return wJoueurDansTable != pJoueur;
}




void FacadeServeurMaitre::updateGameStatus(PartieServeurs* pPartieInfos)
{
    // On trouve la partie recherchee
    PartieServeurs* wPartieDejaEnMap = mListeParties[pPartieInfos->getUniqueGameId()];
    if(wPartieDejaEnMap != pPartieInfos)
    {
        delete wPartieDejaEnMap;
    }
    mListeParties[pPartieInfos->getUniqueGameId()] = pPartieInfos;

}


// Libere les infos contenus dans les maps et listes et qui sont normalement supprimes par la facade
void FacadeServeurMaitre::emptyMemory()
{

    // Vide la liste de parties en cours
    auto it = mListeParties.begin();
    for(; it!=mListeParties.end(); ++it)
    {
        delete (*it).second;
    }
    mListeParties.clear();


    // Vide les joueurs en cours de connexion
    auto itPlayer = mListeJoueursConnecting.begin();
    for(;itPlayer!=mListeJoueursConnecting.end(); ++it)
    {
        delete (*it).second;
    }
    mListeJoueursConnecting.clear();


    // Vide les joueurs connectes
    itPlayer = mListeJoueursConnected.begin();
    for(;itPlayer!=mListeJoueursConnected.end(); ++it)
    {
        delete (*it).second;
    }
    mListeJoueursConnected.clear();

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
