///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauClientLourd.cpp
/// @author Mathieu Parent
/// @date 2013-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GestionnaireReseauClientLourd.h"
#include "PaquetHandlers\PacketHandlerChatMessage.h"
#include "UsinePaquets\UsinePaquetChatMessage.h"
#include "PaquetHandlers\PacketHandlerChatUserStatus.h"
#include "UsinePaquets\UsinePaquetChatUserStatus.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseauClientLourd);

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauClientLourd::GestionnaireReseauClientLourd(  )
///
/// Constructeur, appelle le constructeur de GestionnaireReseau
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauClientLourd::GestionnaireReseauClientLourd()
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau::setNetworkMode(CLIENT);
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->init();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
	wGestionnaireReseau->ajouterOperationReseau("ChatUserStatus", new PacketHandlerChatUserStatus, new UsinePaquetChatUserStatus);


}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauClientLourd::~GestionnaireReseauClientLourd(  )
///
/// Destructeur 
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauClientLourd::~GestionnaireReseauClientLourd()
{
    GestionnaireReseau::libererInstance();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////