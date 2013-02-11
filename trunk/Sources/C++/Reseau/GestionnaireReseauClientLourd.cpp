///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauClientLourd.cpp
/// @author Mathieu Parent
/// @date 2013-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GestionnaireReseauClientLourd.h"
#include "PaquetHandlers\PacketHandlerChatMessage.h"
#include "UsinePaquets\UsinePaquetChatMessage.h"
#include "PaquetHandlers\PacketHandlerUserStatus.h"
#include "UsinePaquets\UsinePaquetUserStatus.h"

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
GestionnaireReseauClientLourd::GestionnaireReseauClientLourd(): mMessageReceivedCallBack(NULL)
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau::setNetworkMode(CLIENT);
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->init();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
	wGestionnaireReseau->ajouterOperationReseau("UserStatus", new PacketHandlerUserStatus, new UsinePaquetUserStatus);


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

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseauClientLourd::messageReceived( char* pUsername, char* pMessage )
///
/// /*Description*/
///
/// @param[in] char * pUsername
/// @param[in] char * pMessage
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseauClientLourd::messageReceived( const char* pUsername, const char* pMessage )
{
    if(mMessageReceivedCallBack)
    {
        mMessageReceivedCallBack((char*)pUsername,(char*)pMessage);
    }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////