#pragma once
#include "Paquet.h"

class UsinePaquetGameEvent;



class PaquetGameEvent : public Paquet
{
public:
    ~PaquetGameEvent(void);

    virtual PacketTypes getOperation() const    { return GAME_EVENT;   }

    
    inline int getGameId() const                { return mGameId;      }
    inline void setGameId(int val)              { mGameId = val;       }

    inline std::string getPlayer1Name() const   { return mPlayer1Name; }
    inline void setPlayer1Name(std::string val) { mPlayer1Name = val;  }

    inline std::string getPlayer2Name() const   { return mPlayer2Name; }
    inline void setPlayer2Name(std::string val) { mPlayer2Name = val;  }

    inline GameEventCode getEvent() const           { return mEvent;       }
    inline void setEvent(GameEventCode val)         { mEvent = val;        }

    inline bool getEventOnPlayerLeft() const   { return mEventOnPlayerLeft; }
    inline void setEventOnPlayerLeft(bool val) { mEventOnPlayerLeft = val;  }

protected:
    PaquetGameEvent(void);

private:
    friend class UsinePaquetGameEvent;

    int             mGameId;              // GameId of the game you want to connect to
    std::string     mPlayer1Name;         // PlayerName1
    std::string     mPlayer2Name;         // PlayerName2
    GameEventCode   mEvent;               // GameEvent
    bool            mEventOnPlayerLeft;   // If the event is specified on the left player
    



};

