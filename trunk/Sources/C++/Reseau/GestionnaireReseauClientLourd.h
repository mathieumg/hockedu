//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauClientLourd.h
/// @author Mathieu Parent
/// @date 2013-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "GestionnaireReseau.h"

struct ChatMessageReceived
{
    std::string mUsername;
    std::string mMessage;
};
typedef BOOL (__stdcall *MessageReceivedCallBack)(char* pUsername, char* pMessage, char* pGroupName);

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireReseauClientLourd
/// @brief Classe qui représente le Gestionnaire Réseau du client lourd
///
/// @author Mathieu Parent
/// @date 2013-01-25
///////////////////////////////////////////////////////////////////////////
class GestionnaireReseauClientLourd :public Singleton<GestionnaireReseauClientLourd>
{

    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireReseauClientLourd);
public:
    /// Accessors of mMessageReceivedCallBack
    inline void setMessageReceivedCallBack(MessageReceivedCallBack pVal) { mMessageReceivedCallBack = pVal; }
    void messageReceived(const char* pUsername, const char* pMessage, const char* pGroupName);

private:
    /// Constructeur par defaut
    GestionnaireReseauClientLourd();
    /// Destructeur
    ~GestionnaireReseauClientLourd();

    MessageReceivedCallBack mMessageReceivedCallBack;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




