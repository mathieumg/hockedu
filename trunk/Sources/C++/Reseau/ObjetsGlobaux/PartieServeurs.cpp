


#include "PartieServeurs.h"

int PartieServeurs::compteurGameId = 0;



PartieServeurs::PartieServeurs( const std::string& pPlayer1Name, const std::string& pPlayer2Name )
{
    mPlayer1Name = pPlayer1Name;
    mPlayer2Name = pPlayer2Name;
    mUniqueGameId = compteurGameId; // Assigne un id unique a la partie
    compteurGameId++;
}




PartieServeurs::~PartieServeurs()
{
	
}


