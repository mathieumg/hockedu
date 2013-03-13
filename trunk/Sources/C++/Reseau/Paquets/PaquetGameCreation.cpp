#include "PaquetGameCreation.h"


PaquetGameCreation::PaquetGameCreation()
    : mUsername(GestionnaireReseau::obtenirInstance()->getPlayerName())
{
}


PaquetGameCreation::~PaquetGameCreation(void)
{
}
