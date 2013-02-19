#ifndef NETWORK_DEFINES_INCLUDED
#define NETWORK_DEFINES_INCLUDED
#ifdef LINUX
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
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
    inline void createMutex  (HANDLE_MUTEX& pMutex)
    {
#ifdef WINDOWS
        pMutex = CreateMutex(NULL, FALSE, NULL);
#elif defined(LINUX)
        pthread_mutex_init (&pMutex, NULL);
#endif
    }

    inline void takeMutex    (HANDLE_MUTEX& pMutex)
    {
#ifdef WINDOWS
        WaitForSingleObject(pMutex,INFINITE);
#elif defined(LINUX)
        pthread_mutex_lock (&pMutex);
#endif
    }

    inline void releaseMutex (HANDLE_MUTEX& pMutex)
    {
#ifdef WINDOWS
        ReleaseMutex(pMutex);
#elif defined(LINUX)
        pthread_mutex_unlock(&pMutex);
#endif
    }

    inline void createSemaphore  (HANDLE_SEMAPHORE& pSemaphore, int pInitialValue, int pMaxAmount)
    {
#ifdef WINDOWS
        pSemaphore = CreateSemaphore(NULL, pInitialCount, pMaxAmount, NULL);
#elif defined(LINUX)
        sem_init(&pSemaphore, 0, pMaxAmount);
#endif
    }

    inline void takeSemaphore    (HANDLE_SEMAPHORE& pSemaphore)
    {
#ifdef WINDOWS
         WaitForSingleObject(pSemaphore, INFINITE);
#elif defined(LINUX)
        sem_wait(&pSemaphore);
#endif
    }

    inline void releaseSemaphore (HANDLE_SEMAPHORE& pSemaphore)
    {
#ifdef WINDOWS
        ReleaseSemaphore(pSemaphore,1,NULL);
#elif defined(LINUX)
        sem_post(&pSemaphore);
#endif
    }

    inline void createThread    (HANDLE_THREAD& pThread, void* (*pThreadFunction) (void*), void* pThreadParameters)
    {
#ifdef WINDOWS
        pThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pThreadFunction, pThreadParameters, 0, NULL);
#elif defined(LINUX)
        pthread_create(&pThread, NULL, pThreadFunction, pThreadParameters);
#endif
    }

    inline void terminateThread (HANDLE_THREAD& pThread)
    {
#ifdef WINDOWS
        TerminateThread(pThread, 0);
#elif defined(LINUX)
        pthread_cancel(pThread);
#endif
    }

    inline void exitThread      (int result)
    {
#ifdef WINDOWS
        ExitThread(result);
#elif defined(LINUX)
        pthread_exit(NULL);
#endif
    }

    inline void closeSocket (HANDLE_SOCKET& pSocket)
    {
#ifdef WINDOWS
        closesocket(pSocket);
#elif defined(LINUX)
        close(pSocket);
#endif
    }

    inline void sleep (int pTimeInMs)
    {
#ifdef WINDOWS
        Sleep(pTimeInMs);
#elif defined(LINUX)
        ::sleep(pTimeInMs);
#endif
    }
};
#endif // NETWORK_DEFINES_INCLUDED
