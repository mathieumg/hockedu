//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeurMaitre.h
/// @author Mathieu Parent
/// @date 2013-02-09
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include "GestionnaireReseau.h"

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireReseauServeurMaitre
/// @brief Classe qui représente le Gestionnaire Réseau du serveur maitre
///
/// @author Mathieu Parent
/// @date 2013-02-09
///////////////////////////////////////////////////////////////////////////
class GestionnaireReseauServeurMaitre :public Singleton<GestionnaireReseauServeurMaitre>
{

    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireReseauServeurMaitre);
public:
	static void SocketStateCallback(const ConnectionStateEvent& pEvent);

private:
    /// Constructeur par defaut
    GestionnaireReseauServeurMaitre();
    /// Destructeur
    ~GestionnaireReseauServeurMaitre();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




