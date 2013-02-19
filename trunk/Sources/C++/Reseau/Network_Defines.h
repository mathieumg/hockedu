#ifndef NETWORK_DEFINES_INCLUDED
#define NETWORK_DEFINES_INCLUDED
#ifdef LINUX
#include <pthread.h>
#include <semaphore.h>
#endif
#ifdef WINDOWS
#include <WinSock2.h>
#include <windows.h>
#endif
#ifdef WINDOWS
typedef HANDLE HANDLE_MUTEX;
typedef HANDLE HANDLE_THREAD;
typedef HANDLE HANDLE_SEMAPHORE;
typedef SOCKET HANDLE_SOCKET;
#elif defined(LINUX)
typedef pthread_mutex_t HANDLE_MUTEX;
typedef pthread_t       HANDLE_THREAD;
typedef sem_t           HANDLE_SEMAPHORE;
typedef int             HANDLE_SOCKET;
#endif
namespace FacadePortability {


    void createMutex  (HANDLE_MUTEX& pMutex);
    void takeMutex    (HANDLE_MUTEX& pMutex);
    void releaseMutex (HANDLE_MUTEX& pMutex);

    void createSemaphore  (HANDLE_SEMAPHORE& pSemaphore, int pInitialValue, int pMaxAmount);
    void takeSemaphore    (HANDLE_SEMAPHORE& pSemaphore);
    void releaseSemaphore (HANDLE_SEMAPHORE& pSemaphore);

    void createThread    (HANDLE_THREAD& pThread, void* (*pThreadFunction) (void*), void* pThreadParameters);
    void terminateThread (HANDLE_THREAD& pThread);
    void exitThread      (int result);


    void closeSocket (HANDLE_SOCKET& pSocket);

void sleep (int pTimeInMs);
};
#endif // NETWORK_DEFINES_INCLUDED
