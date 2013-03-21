#include "PaquetGameConnection.h"


PaquetGameConnection::PaquetGameConnection()
    :mUsername(GestionnaireReseau::obtenirInstance()->getPlayerName())
{
    mConnectionState = GAME_CONNECTION_PENDING;
}


PaquetGameConnection::~PaquetGameConnection(void)
{
}
