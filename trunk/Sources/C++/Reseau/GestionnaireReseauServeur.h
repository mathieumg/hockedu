//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeur.h
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include "GestionnaireReseau.h"

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireReseauClientLourd
/// @brief Classe qui représente le Gestionnaire Réseau du serveur
///
/// @author Mathieu Parent
/// @date 2013-01-26
///////////////////////////////////////////////////////////////////////////
class GestionnaireReseauServeur :public Singleton<GestionnaireReseauServeur>
{

    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireReseauServeur);
public:
	static void SocketStateCallback(const ConnectionStateEvent& pEvent);

private:
    /// Constructeur par defaut
    GestionnaireReseauServeur();
    /// Destructeur
    ~GestionnaireReseauServeur();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




