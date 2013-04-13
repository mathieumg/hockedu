#include "PaquetGameConnection.h"


PaquetGameConnection::PaquetGameConnection()
    :mUsername("")
{
    mConnectionState = GAME_CONNECTION_PENDING;
}


PaquetGameConnection::~PaquetGameConnection(void)
{
}
