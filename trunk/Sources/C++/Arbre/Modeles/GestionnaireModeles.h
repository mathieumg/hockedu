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
	/// permet de connaître le nom du type a partir de son ID
	std::string obtenirNameFromTypeId(const GLuint& typeId) const;
	/// permet de connaître le ID d'un partir de son nom
    GLuint obtenirTypeIdFromName(const std::string& name) const;
	/// Ajout d'un modele
    void ajoutModele(Modele3DKey key, Modele3D* modele3d);
	/// Création d'une liste
    static GLuint CreerListe( Modele3D* pModel, bool avecTexture = true);
    /// Ajoute une liste dans la map pour la retrouver
    void AjouterListe( Modele3DKey key, GLuint liste );

	void initialiser();

    bool isStillLoadingModel() const;

    /// Debug usage, do not call unless certain.
    void ReloadModels();

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

	/// Tables de référence entre les types concrets des noeuds et un numéro
	NameToTypeId nameToTypeId_;
	TypeIdToName typeIdToName_; 

    HANDLE mLoadingThread;
};

////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////