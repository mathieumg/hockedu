#pragma once

enum ByteOrder {NATIVE, BO_LITTLE_ENDIAN, BO_BIG_ENDIAN, UNKNOWN};

enum NetworkMode {CLIENT, SERVER, NOT_DEFINED};
enum ExceptionTypes {GLOBALE, PARAMETRE_INVALIDE, SOCKET_DECONNECTE, TIMEOUT, TYPE_NOT_DEFINED, AUCUNE_ERREUR};

enum ConnectionType {UDP, TCP};
enum ConnectionState {CONNECTED, CONNECTING, NOT_CONNECTED};

typedef int (*PaquetRunnableFunc) (class Paquet*);


//Contains the operating system's byte order.
extern ByteOrder NATIVE_BYTE_ORDER;
