//////////////////////////////////////////////////////////////////////////////
/// @file Paquet.cpp
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "Paquet.h"
#include <sstream>
#include <iostream>
#include <time.h>
#ifdef LINUX
#include <limits.h>
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn obtenirNumeroHazard() const
///
/// Fonction qui retourne un nombre aleatoire afin d'initialiser le numero de sequence
///
///
/// @return int : nombre aleatoire entre 1 et INT_MAX/2
///
////////////////////////////////////////////////////////////////////////
int obtenirNumeroInitHazard()
{
    int maxValue = INT_MAX/2;

#pragma warning( disable : 4244 )
    srand(time(NULL));


    return rand() % maxValue;

}

// Numero des paquets (unique)
int Paquet::CompteurNumeroPaquet = obtenirNumeroInitHazard();

const std::string Paquet::sequenceIdentification = "LindseyStirling";

////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet::Paquet( )
///
/// Constructeur par defaut (Prive) Seulement les Usines peuvent creer des Paquets
///
/// @return
///
////////////////////////////////////////////////////////////////////////
Paquet::Paquet()
{
    mNbAssociatedQueries = 1; // Par defaut, on envoie le paquet qu'une seule fois
    mNumeroPaquet = ++CompteurNumeroPaquet;
    mLatency = 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet::~Paquet( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
Paquet::~Paquet(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Paquet::removeAssociatedQueries()
///
/// Methode qui decremente le compteur de queries associees a ce Paquet.
/// Si compteur = 0, on appelle le destructeur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Paquet::removeAssociatedQuery()
{
    --mNbAssociatedQueries;
    if(mNbAssociatedQueries == 0)
    {
        delete this;
    }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
