///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeur.cpp
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GestionnaireReseauServeur.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "Reseau\GestionnaireReseau.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseauServeur);

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauServeur::GestionnaireReseauServeur(  )
///
/// Constructeur, appelle le constructeur de GestionnaireReseau
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauServeur::GestionnaireReseauServeur()
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau::setNetworkMode(SERVER);
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->init();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);


}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauServeur::~GestionnaireReseauServeur(  )
///
/// Destructeur 
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauServeur::~GestionnaireReseauServeur()
{
    GestionnaireReseau::libererInstance();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////