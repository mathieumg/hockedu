#ifndef NETWORK_DEFINES_INCLUDED
#define NETWORK_DEFINES_INCLUDED
#ifdef LINUX
#include <pthread.h>
#include <semaphore.h>
#endif

namespace FacadePortability {
#ifdef WINDOWS
#include <windows.h>
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

inline void createMutex  (HANDLE_MUTEX& pMutex);
inline void takeMutex    (HANDLE_MUTEX& pMutex);
inline void releaseMutex (HANDLE_MUTEX& pMutex);

inline void createSemaphore  (HANDLE_SEMAPHORE& pSemaphore, int pInitialValue, int pMaxAmount);
inline void takeSemaphore    (HANDLE_SEMAPHORE& pSemaphore);
inline void releaseSemaphore (HANDLE_SEMAPHORE& pSemaphore);

inline void createThread    (HANDLE_THREAD& pThread, void* (*pThreadFunction) (void*), void* pThreadParameters);
inline void terminateThread (HANDLE_THREAD& pThread);
inline void exitThread      (int result);


inline void closeSocket (HANDLE_SOCKET& pSocket);

inline void sleep (int pTimeInMs);
};
#endif // NETWORK_DEFINES_INCLUDED
