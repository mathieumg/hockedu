//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeurJeu.h
/// @author Mathieu Parent
/// @date 2013-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include "Reseau\GestionnaireReseau.h"

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireReseauServeurJeu
/// @brief Classe qui représente le Gestionnaire Réseau du serveur jeu
///
/// @author Mathieu Parent
/// @date 2013-02-10
///////////////////////////////////////////////////////////////////////////
class GestionnaireReseauServeurJeu :public Singleton<GestionnaireReseauServeurJeu>
{

    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireReseauServeurJeu);
public:
	static void SocketStateCallback(const ConnectionStateEvent& pEvent);

private:
    /// Constructeur par defaut
    GestionnaireReseauServeurJeu();
    /// Destructeur
    ~GestionnaireReseauServeurJeu();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




