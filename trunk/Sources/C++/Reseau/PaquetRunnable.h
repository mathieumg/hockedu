
#pragma once

typedef int (*PaquetRunnableFunc) (class Paquet*);
class Paquet;

namespace PaquetRunnable
{
    int RunnableEvent(Paquet* pPaquet);

    int RunnableConnAutomatiqueClient(Paquet* pPaquet);
    int RunnableConnAutomatiqueServer(Paquet* pPaquet);

    int RunnableUserStatusClient(Paquet* pPaquet);
    int RunnableUserStatusServer(Paquet* pPaquet);

    int RunnableChatMessageServer(Paquet* pPaquet);
    int RunnableChatMessageClient(Paquet* pPaquet);

    int RunnableTest(Paquet* pPaquet);
};



