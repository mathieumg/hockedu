
#pragma once

#include "NetworkEnums.h"

class Paquet;

namespace PaquetRunnable
{

    int RunnableEvent(Paquet* pPaquet);
    int RunnableUserStatusClient(Paquet* pPaquet);
    int RunnableChatMessageClient(Paquet* pPaquet);
};



