///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireModeles.h
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once


#include "Modele3D.h"
#include "Singleton.h"
#include "Enum_Declarations.h"

typedef RazerKey Modele3DKey;
class NoeudAbstrait;
///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireModeles
/// @brief Classe qui initialise la banque de modele et permet l'accès à 
///	       ces modèles à l'aide de la clé qui est le nom du modèle.
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-01-27
///////////////////////////////////////////////////////////////////////////
class GestionnaireModeles:public Singleton<GestionnaireModeles>
{
public:
	static GestionnaireModeles* obtenirInstance();
	/// Accesseur sur la banque de modeles
    Modele3D* obtenirModele (Modele3DKey key);
	/// Obtention des listes d'affichages
    void obtenirListe(Modele3DKey key, GLuint& liste);
	/// Rechargement d'un modèle
    void recharger(Modele3DKey type);
	/// Ajout d'un modele
    void ajoutModele(Modele3DKey key, Modele3D* modele3d);
	/// Création d'une liste
    static GLuint CreerListe( Modele3D* pModel, bool avecTexture = true);
    /// Ajoute une liste dans la map pour la retrouver
    void AjouterListe( Modele3DKey key, GLuint liste );
    void ModifierListe( Modele3DKey key, GLuint liste );

	void initialiser();

    // creates a new render component 
    class IRenderComponent* createRenderComponent( NoeudAbstrait *) const;

    bool isStillLoadingModel() const;

    /// Debug usage, do not call unless certain.
    void ReloadModels();

    unsigned int mHouseList;

private:
	typedef std::map<std::string,GLuint> NameToTypeId;
	typedef std::map<GLuint,std::string> TypeIdToName;

	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireModeles);

	/// Création du Gestionnaire de Modeles
	GestionnaireModeles();

	/// Destruction du Gestionnaire de Modeles
	~GestionnaireModeles();

	/// Map dans laquelle sont stockés les modèles
	typedef std::map<Modele3DKey,Modele3D*> BanqueModeles;
	BanqueModeles banqueModeles;

	/// Map dans laquelle sont stockés les listes
	typedef std::map<Modele3DKey,GLuint> BanqueListes;
	BanqueListes banqueListes;

    HANDLE mLoadingThread;
};

////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////