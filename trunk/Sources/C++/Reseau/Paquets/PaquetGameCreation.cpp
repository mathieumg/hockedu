#include "PaquetGameCreation.h"
#include "FacadeModele.h"


PaquetGameCreation::PaquetGameCreation()
    : mUsername(GestionnaireReseau::obtenirInstance()->getPlayerName()), mMapName(FacadeModele::FICHIER_TERRAIN_EN_COURS)
{
}


PaquetGameCreation::~PaquetGameCreation(void)
{
}
