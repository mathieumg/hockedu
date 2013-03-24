#include "Network_Defines.h"
#ifdef LINUX
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "GestionnaireReseau.h"

void FacadePortability::getLocalIPAddresses(std::list<std::string>& pOut)
{
#ifdef WINDOWS
    char addrName[TAILLE_BUFFER_HOSTNAME];
    if(gethostname(addrName, TAILLE_BUFFER_HOSTNAME))
    {
        GestionnaireReseau::obtenirInstance()->throwExceptionReseau("Erreur lors de la lecture du nom de l'hôte");
        return;
    }

    hostent *hostCourant;
    hostCourant=gethostbyname(addrName);
    if(hostCourant == NULL)
    {
        // Probleme au get des valeurs reseau locales
        GestionnaireReseau::obtenirInstance()->throwExceptionReseau("Erreur lors de la lecture des adresses reseau locales");
        return;
    }

    pOut.push_back("127.0.0.1");

    int wNbCount = 0;
    while(hostCourant->h_addr_list[wNbCount])
    {
        std::string wIP = inet_ntoa(*(struct in_addr *)hostCourant->h_addr_list[wNbCount]);
        pOut.push_back(wIP);
        ++wNbCount;
    }
#elif defined(LINUX)
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            pOut.push_back(std::string(addressBuffer));
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
#endif
}
