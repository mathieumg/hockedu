//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeur.h
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include "Reseau\GestionnaireReseau.h"

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireReseauClientLourd
/// @brief Classe qui repr�sente le Gestionnaire R�seau du serveur
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




