//////////////////////////////////////////////////////////////////////////////
/// @file PaquetAuthentificationServeurJeu.h
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetAuthentificationServeurJeu;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetAuthentificationServeurJeu
/// @brief Classe qui represente un paquet pour l'authentification du serveur Jeu au serveur Maitre
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class PaquetAuthentificationServeurJeu : public Paquet {
private:
    friend UsinePaquetAuthentificationServeurJeu;

protected:
	PaquetAuthentificationServeurJeu();

public:
    virtual PacketTypes getOperation() const { return AUTHENTIFICATION_SERVEUR_JEU; }


	~PaquetAuthentificationServeurJeu();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

