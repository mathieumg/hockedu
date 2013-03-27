#include "PaquetGameCreation.h"


PaquetGameCreation::PaquetGameCreation()
    : mUsername(GestionnaireReseau::obtenirInstance()->getPlayerName()), mMapName("MapEnCours.xml"), mServerId(0), mGameId(-1), mServerIP(""), mPassword("")
{
}


PaquetGameCreation::~PaquetGameCreation(void)
{
}
