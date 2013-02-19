#include "Network_Defines.h"

#ifdef WINDOWS
void FacadePortability::createMutex (HANDLE_MUTEX& pMutex)
{
    pMutex = CreateMutex(NULL, FALSE, NULL);
}

void FacadePortability::takeMutex (HANDLE_MUTEX& pMutex)
{
	WaitForSingleObject(pMutex,INFINITE);
}

void FacadePortability::releaseMutex (HANDLE_MUTEX& pMutex)
{
	ReleaseMutex(pMutex);
}

void FacadePortability::createSemaphore (HANDLE_SEMAPHORE& pSemaphore, int pInitialCount, int pMaxAmount)
{
    pSemaphore = CreateSemaphore(NULL, pInitialCount, pMaxAmount, NULL);
}

void FacadePortability::takeSemaphore (HANDLE_SEMAPHORE& pSemaphore)
{
    WaitForSingleObject(pSemaphore, INFINITE);
}

void FacadePortability::releaseSemaphore (HANDLE_SEMAPHORE& pSemaphore)
{
    ReleaseSemaphore(pSemaphore,1,NULL);
}

void FacadePortability::createThread (HANDLE_THREAD& pThread, void* (*pThreadFunction) (void*), void* pThreadParameters)
{
    pThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pThreadFunction, pThreadParameters, 0, NULL);
}

void FacadePortability::terminateThread (HANDLE_THREAD& pThread)
{
    TerminateThread(pThread, 0);
}

void FacadePortability::exitThread(int result)
{
    ExitThread(result);
}

void FacadePortability::closeSocket(HANDLE_SOCKET& pSocket)
{
    closesocket(pSocket);
}

void FacadePortability::sleep(int pTimeInMs)
{
    Sleep(pTimeInMs);
}
#elif defined(LINUX)
#include <stdlib.h>
#include <unistd.h>

void FacadePortability::createMutex (HANDLE_MUTEX& pMutex)
{
	pthread_mutex_init (&pMutex, NULL);
}

void FacadePortability::takeMutex (HANDLE_MUTEX& pMutex)
{
}

void FacadePortability::releaseMutex (HANDLE_MUTEX& pMutex)
{
}

void FacadePortability::createSemaphore (HANDLE_SEMAPHORE& pSemaphore, int pInitialCount, int pMaxAmount)
{
}

void FacadePortability::takeSemaphore (HANDLE_SEMAPHORE& pSemaphore)
{
}

void FacadePortability::releaseSemaphore (HANDLE_SEMAPHORE& pSemaphore)
{
}

void FacadePortability::createThread (HANDLE_THREAD& pThread, void* (*pThreadFunction) (void*), void* pThreadParameters)
{
}

void FacadePortability::terminateThread (HANDLE_THREAD& pThread)
{
}

void FacadePortability::exitThread(int result)
{
}

void FacadePortability::closeSocket(HANDLE_SOCKET& pSocket)
{
    close(pSocket);
}

void FacadePortability::sleep(int pTimeInMs)
{
}
#endif
