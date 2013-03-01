//////////////////////////////////////////////////////////////////////////////
/// @file PaquetChatMessage.h
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetChatMessage;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetChatMessage
/// @brief Classe qui represente un paquet pour faire les echanges de messages pour le chat
///
/// @author Mathieu Parent
/// @date 2013-01-26
///////////////////////////////////////////////////////////////////////////
class PaquetChatMessage : public Paquet {
private:
    friend class UsinePaquetChatMessage;

    int64_t mTimestamp; // Timestamp (should be the same as __int64)
    std::string mMessage; // Message a envoyer
    bool mIsTargetGroup; // True means the target is a group, false means it's only one player
    std::string mGroupName; // Or player name if mIsTragetGroup == false
    std::string mOrigin; // Mettre le nom du PC local pour eviter les loop infinies sur le network et eviter de recevoir ses propres messages


protected:
	PaquetChatMessage();

public:
	virtual PacketTypes getOperation() const { return CHAT_MESSAGE; }

    inline std::string getMessage() const { return mMessage; }
    inline void setMessage(const std::string& val) { mMessage = val; }

    inline int64_t getTimestamp() const { return mTimestamp; }
    inline void setTimestamp(int64_t val) { mTimestamp = val; }

    inline bool IsTargetGroup() const { return mIsTargetGroup; }
    inline void setIsTargetGroup(bool val) { mIsTargetGroup = val; }

    inline std::string getGroupName() const { return mGroupName; }
    inline void setGroupName(const std::string& val) { mGroupName = val; }

    inline std::string getOrigin() const { return mOrigin; }
	inline void setOrigin(const std::string& val) { mOrigin = val; }

	~PaquetChatMessage();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

