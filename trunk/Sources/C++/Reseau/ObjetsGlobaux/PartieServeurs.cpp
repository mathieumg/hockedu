


#include "PartieServeurs.h"

// Eventuellement il faudrait aller chercher la valeur dans la BD
int PartieServeurs::compteurGameId = 0;



PartieServeurs::PartieServeurs( const std::string& pPlayer1Name, const std::string& pPlayer2Name )
{
    mPlayer1Name    = pPlayer1Name;
    mPlayer2Name    = pPlayer2Name;
    mUniqueGameId   = compteurGameId; // Assigne un id unique a la partie
    mTime           = time(0);
    compteurGameId++;
}

PartieServeurs::PartieServeurs()
{
    mTime           = time(0);
    mUniqueGameId   = compteurGameId; // Assigne un id unique a la partie
    compteurGameId++;
}



PartieServeurs::PartieServeurs( PartieServeurs* pSource )
{
    mUniqueGameId   = pSource->mUniqueGameId;
    mPlayer1Name    = pSource->mPlayer1Name;
    mPlayer2Name    = pSource->mPlayer2Name;
    mPlayer1Score   = pSource->mPlayer1Score;
    mPlayer2Score   = pSource->mPlayer2Score;
    mTime           = pSource->mTime;
}



PartieServeurs::~PartieServeurs()
{

}



// Plus simple de faire des tailles fixes avec 2 joueurs pour l'envoie sur le reseau
void PartieServeurs::setPlayerName1( const std::string& pPlayerName1 )
{
    mPlayer1Name = pPlayerName1;
}

void PartieServeurs::setPlayerName2( const std::string& pPlayerName2 )
{
    mPlayer2Name = pPlayerName2;
}





void PartieServeurs::setTime( int pHours, int pMins, int pSec )
{
    tm* timeInfo = NULL;
#ifdef WINDOWS
    gmtime_s ( timeInfo, &mTime );
#elif defined(LINUX)
    timeInfo = gmtime(&mTime);
#endif
    timeInfo->tm_hour   = pHours;
    timeInfo->tm_min    = pMins;
    timeInfo->tm_sec    = pSec;
}

