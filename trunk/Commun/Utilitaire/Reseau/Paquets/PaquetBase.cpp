
#include "PaquetBase.h"
#include <time.h>

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
int PaquetBase::CompteurNumeroPaquet = obtenirNumeroInitHazard();



////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet::Paquet( )
///
/// Constructeur par defaut (Prive) Seulement les Usines peuvent creer des Paquets
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PaquetBase::PaquetBase( const std::string& pOperation)
	:Paquet(pOperation)
{
	numeroPaquet = ++CompteurNumeroPaquet;
    // Size de Paquet
    mTaillePaquet = Paquet::sequenceIdentification.length()+1 + GestionnaireReseau::longueurMaxOperationReseau;
    // Size de PaquetBase
    mTaillePaquet += sizeof(PaquetBase::numeroPaquet) + sizeof(mTaillePaquet);
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
PaquetBase::~PaquetBase()
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PaquetBase::obtenirNumeroPaquet() const
///
/// Accesseur du numero de paquet
/// 
///
/// @return int : numero de paquet
///
////////////////////////////////////////////////////////////////////////
int PaquetBase::getNumeroPaquet() const
{
	return numeroPaquet;
}

int PaquetBase::getTaillePaquet() const
{
    return mTaillePaquet;
}
