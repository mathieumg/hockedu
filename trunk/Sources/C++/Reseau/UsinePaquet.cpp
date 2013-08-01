///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierFactory.cpp
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "UsinePaquet.h"
#include "PaquetLoginInfo.h"
#include "PaquetUserStatus.h"
#include "PaquetGameStatus.h"
#include "PaquetGameCreation.h"
#include "PaquetGameConnection.h"
#include "PaquetMaillet.h"
#include "PaquetRondelle.h"
#include "PaquetGameEvent.h"
#include "PaquetPortal.h"
#include "PaquetBonus.h"
                          
PAQUET_FACTORY_IMPLEMENTATION(LoginInfo);
PAQUET_FACTORY_IMPLEMENTATION(UserStatus);
PAQUET_FACTORY_IMPLEMENTATION(GameStatus);
PAQUET_FACTORY_IMPLEMENTATION(GameCreation);
PAQUET_FACTORY_IMPLEMENTATION(GameConnection);
PAQUET_FACTORY_IMPLEMENTATION(Maillet);
PAQUET_FACTORY_IMPLEMENTATION(Rondelle);
PAQUET_FACTORY_IMPLEMENTATION(GameEvent);
PAQUET_FACTORY_IMPLEMENTATION(Portal);
PAQUET_FACTORY_IMPLEMENTATION(Bonus);


const std::string PaquetNamesArray[NB_PACKET_TYPES] = 
{
   "LOGIN_INFO"           ,//LOGIN_INFO
   "USER_STATUS"          ,//USER_STATUS                 
   "GAME_STATUS"          ,//GAME_STATUS                 
   "GAME_CREATION_REQUEST",//GAME_CREATION_REQUEST       
   "GAME_CONNECTION"      ,//GAME_CONNECTION             
   "MAILLET"              ,//MAILLET                     
   "RONDELLE"             ,//RONDELLE                    
   "GAME_EVENT"           ,//GAME_EVENT                  
   "PORTAL"               ,//PORTAL                      
   "BONUS"                ,//BONUS                 
};

UsinePaquet* PaquetFactories[NB_PACKET_TYPES] =
{
    new UsinePaquetLoginInfo                 ,//LOGIN_INFO
    new UsinePaquetUserStatus                ,//USER_STATUS                 
    new UsinePaquetGameStatus                ,//GAME_STATUS                 
    new UsinePaquetGameCreation              ,//GAME_CREATION_REQUEST       
    new UsinePaquetGameConnection            ,//GAME_CONNECTION             
    new UsinePaquetMaillet                   ,//MAILLET                     
    new UsinePaquetRondelle                  ,//RONDELLE                    
    new UsinePaquetGameEvent                 ,//GAME_EVENT                  
    new UsinePaquetPortal                    ,//PORTAL                      
    new UsinePaquetBonus                     ,//BONUS                       
};


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquet::creerPaquet( PacketTypes t )
///
/// /*Description*/
///
/// @param[in] PacketTypes t
///
/// @return Paquet*
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquet::creerPaquet( PacketTypes t )
{
    return PaquetFactories[t]->creerPaquet();
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
