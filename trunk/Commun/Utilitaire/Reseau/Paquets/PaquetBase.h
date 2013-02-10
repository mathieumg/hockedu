//////////////////////////////////////////////////////////////////////////////
/// @file PaquetBase.h
/// @author Mathieu Parent
/// @date 2013-01-09
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetBase;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetBase
/// @brief Classe qui represente les headers de base pour les paquets
///
/// @author Mathieu Parent
/// @date 2013-01-09
///////////////////////////////////////////////////////////////////////////
class PaquetBase : public Paquet {
private:
	friend UsinePaquetBase;
	
	int numeroPaquet;
    
protected:
	PaquetBase( const std::string& pOperation);
public:
	static int CompteurNumeroPaquet;
	int getNumeroPaquet() const;
	~PaquetBase();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

