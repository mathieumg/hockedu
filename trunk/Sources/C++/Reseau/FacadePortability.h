#ifndef FACADEPORTABILITY_INCLUDED
#define FACADEPORTABILITY_INCLUDED
#ifdef WINDOWS
#include <WinSock2.h>
#include <windows.h>
typedef HANDLE HANDLE_MUTEX;
typedef HANDLE HANDLE_THREAD;
typedef HANDLE HANDLE_SEMAPHORE;
typedef SOCKET HANDLE_SOCKET;

// Defines pour les erreurs
#define RAZORGAME_NO_ERROR           0                       // No error
#define RAZORGAME_EBADF             WSAEBADF                // Bad file number
#define RAZORGAME_EACCES            WSAEACCES               // Permission denied
#define RAZORGAME_EFAULT            WSAEFAULT               // Bad address
#define RAZORGAME_EINVAL            WSAEINVAL               // Invalid argument
#define RAZORGAME_EMFILE            WSAEMFILE               // Too many open sockets
#define RAZORGAME_EWOULDBLOCK       WSAEWOULDBLOCK          // Operation would block
#define RAZORGAME_EINPROGRESS       WSAEINPROGRESS          // Operation now in progress
#define RAZORGAME_EALREADY          WSAEALREADY             // Operation already in progress
#define RAZORGAME_ENOTSOCK          WSAENOTSOCK             // Socket operation on non-socket
#define RAZORGAME_EDESTADDRREQ      WSAEDESTADDRREQ         // Destination address required
#define RAZORGAME_EMSGSIZE          WSAEMSGSIZE             // Message too long
#define RAZORGAME_EPROTOTYPE        WSAEPROTOTYPE           // Protocol wrong type for socket
#define RAZORGAME_ENOPROTOOPT       WSAENOPROTOOPT          // Bad protocol option
#define RAZORGAME_EPROTONOSUPPORT   WSAEPROTONOSUPPORT      // Protocol not supported
#define RAZORGAME_ESOCKTNOSUPPORT   WSAESOCKTNOSUPPORT      // Socket type not supported
#define RAZORGAME_EOPNOTSUPP        WSAEOPNOTSUPP           // Operation not supported on socket
#define RAZORGAME_EPFNOSUPPORT      WSAEPFNOSUPPORT         // Protocol family not supported
#define RAZORGAME_EAFNOSUPPORT      WSAEAFNOSUPPORT         // Address family not supported
#define RAZORGAME_EADDRINUSE        WSAEADDRINUSE           // Address already in use
#define RAZORGAME_EADDRNOTAVAIL     WSAEADDRNOTAVAIL        // Can't assign requested address
#define RAZORGAME_ENETDOWN          WSAENETDOWN             // Network is down
#define RAZORGAME_ENETUNREACH       WSAENETUNREACH          // Network is unreachable
#define RAZORGAME_ENETRESET         WSAENETRESET            // Net connection reset
#define RAZORGAME_ECONNABORTED      WSAECONNABORTED         // Software caused connection abort
#define RAZORGAME_ECONNRESET        WSAECONNRESET           // Connection reset by peer
#define RAZORGAME_ENOBUFS           WSAENOBUFS              // No buffer space available
#define RAZORGAME_EISCONN           WSAEISCONN              // Socket is already connected
#define RAZORGAME_ENOTCONN          WSAENOTCONN             // Socket is not connected
#define RAZORGAME_ESHUTDOWN         WSAESHUTDOWN            // Can't send after socket shutdown
#define RAZORGAME_ETOOMANYREFS      WSAETOOMANYREFS         // Too many references can't splice
#define RAZORGAME_ETIMEDOUT         WSAETIMEDOUT            // Connection timed out
#define RAZORGAME_ECONNREFUSED      WSAECONNREFUSED         // Connection refused
#define RAZORGAME_ELOOP             WSAELOOP                // Too many levels of symbolic links
#define RAZORGAME_ENAMETOOLONG      WSAENAMETOOLONG         // File name too long
#define RAZORGAME_EHOSTDOWN         WSAEHOSTDOWN            // Host is down
#define RAZORGAME_EHOSTUNREACH      WSAEHOSTUNREACH         // No route to host
#define RAZORGAME_ENOTEMPTY         WSAENOTEMPTY            // Directory not empty
#define RAZORGAME_EUSERS            WSAEUSERS               // Too many users
#define RAZORGAME_EDQUOT            WSAEDQUOT               // Disc quota exceeded
#define RAZORGAME_ESTALE            WSAESTALE               // Stale NFS file handle
#define RAZORGAME_EREMOTE           WSAEREMOTE              // Too many levels of remote in path
#define RAZORGAME_EPROCLIM          WSAEPROCLIM             // Too many processes
#define RAZORGAME_SYSNOTREADY       WSASYSNOTREADY          // Network system is unavailable
#define RAZORGAME_VERNOTSUPPORTED   WSAVERNOTSUPPORTED      // Winsock version out of range
#define RAZORGAME_NOTINITIALISED    WSANOTINITIALISED       // WSAStartup not yet called
#define RAZORGAME_EDISCON           WSAEDISCON              // Graceful shutdown in progress
#define RAZORGAME_HOST_NOT_FOUND    WSAHOST_NOT_FOUND       // Host not found
#define RAZORGAME_NO_DATA           WSANO_DATA              // No host data of that type was found
#define RAZORGAME_NOT_ENOUGH_MEMORY WSA_NOT_ENOUGH_MEMORY

#elif defined(LINUX)
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>
typedef pthread_mutex_t HANDLE_MUTEX;
typedef pthread_t       HANDLE_THREAD;
typedef sem_t           HANDLE_SEMAPHORE;
typedef int             HANDLE_SOCKET;

// Setup des defines pour les codes d'erreur
#define RAZORGAME_NO_ERROR          0
#define RAZORGAME_EBADF             EBADF
#define RAZORGAME_EACCES            EACCES
#define RAZORGAME_EFAULT            EFAULT
#define RAZORGAME_EINVAL            EINVAL
#define RAZORGAME_EMFILE            EMFILE
#define RAZORGAME_EWOULDBLOCK       EWOULDBLOCK
#define RAZORGAME_EINPROGRESS       EINPROGRESS
#define RAZORGAME_EALREADY          EALREADY
#define RAZORGAME_ENOTSOCK          ENOTSOCK
#define RAZORGAME_EDESTADDRREQ      EDESTADDRREQ
#define RAZORGAME_EMSGSIZE          EMSGSIZE
#define RAZORGAME_EPROTOTYPE        EPROTOTYPE
#define RAZORGAME_ENOPROTOOPT       ENOPROTOOPT
#define RAZORGAME_EPROTONOSUPPORT   EPROTONOSUPPORT
#define RAZORGAME_ESOCKTNOSUPPORT   ESOCKTNOSUPPORT
#define RAZORGAME_EOPNOTSUPP        EOPNOTSUPP
#define RAZORGAME_EPFNOSUPPORT      EPFNOSUPPORT
#define RAZORGAME_EAFNOSUPPORT      EAFNOSUPPORT
#define RAZORGAME_EADDRINUSE        EADDRINUSE
#define RAZORGAME_EADDRNOTAVAIL     EADDRNOTAVAIL
#define RAZORGAME_ENETDOWN          ENETDOWN
#define RAZORGAME_ENETUNREACH       ENETUNREACH
#define RAZORGAME_ENETRESET         ENETRESET
#define RAZORGAME_ECONNABORTED      ECONNABORTED
#define RAZORGAME_ECONNRESET        ECONNRESET
#define RAZORGAME_ENOBUFS           ENOBUFS
#define RAZORGAME_EISCONN           EISCONN
#define RAZORGAME_ENOTCONN          ENOTCONN
#define RAZORGAME_ESHUTDOWN         ESHUTDOWN
#define RAZORGAME_ETOOMANYREFS      ETOOMANYREFS
#define RAZORGAME_ETIMEDOUT         ETIMEDOUT
#define RAZORGAME_ECONNREFUSED      ECONNREFUSED
#define RAZORGAME_ELOOP             ELOOP
#define RAZORGAME_ENAMETOOLONG      ENAMETOOLONG
#define RAZORGAME_EHOSTDOWN         EHOSTDOWN
#define RAZORGAME_EHOSTUNREACH      EHOSTUNREACH
#define RAZORGAME_ENOTEMPTY         ENOTEMPTY
#define RAZORGAME_EUSERS            EUSERS
#define RAZORGAME_EDQUOT            EDQUOT
#define RAZORGAME_ESTALE            ESTALE
#define RAZORGAME_EREMOTE           EREMOTE
#define RAZORGAME_NOT_ENOUGH_MEMORY ENOMEM
#endif

#include <list>
#include <string>

namespace FacadePortability {
#ifdef WINDOWS
    static const int TAILLE_BUFFER_HOSTNAME = 512;
#endif

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
        pSemaphore = CreateSemaphore(NULL, pInitialValue, pMaxAmount, NULL);
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
        shutdown(pSocket, SD_BOTH);
        closesocket(pSocket);
#elif defined(LINUX)
        shutdown(pSocket, SHUT_RDWR);
        close(pSocket);
#endif
    }

    inline void sleep (int pTimeInMs)
    {
#ifdef WINDOWS
        Sleep(pTimeInMs);
#elif defined(LINUX)
        usleep(pTimeInMs*1000);
#endif
    }

    inline int getLastError()
    {
#ifdef WINDOWS
        return WSAGetLastError();
#else
        return errno;
#endif
    }

    inline void createDirectory(char* dirName)
    {
#ifdef WIN32
        CreateDirectoryA(
            dirName,
            NULL
            );
#else
        struct stat st = {0};
        if (stat(dirName, &st) == -1)
        {
            mkdir(dirName, 0777);
        }
#endif
    }

    void getLocalIPAddresses(std::list<std::string>& pOut);
};
#endif // FACADEPORTABILITY_INCLUDED
