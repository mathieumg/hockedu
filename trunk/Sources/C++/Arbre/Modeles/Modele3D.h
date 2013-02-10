///////////////////////////////////////////////////////////////////////////////
/// @file Modele3D.h
/// @author Julien Gascon-Samson
/// @date 2011-05-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __MODELE3D_H__
#define __MODELE3D_H__

#include <assimp.hpp>      // Interface d'importation "assimp" C++
#include <aiScene.h>       // Structure de données de sortie "assimp"
#include <aiPostProcess.h> // Drapaux de post-traitement "assimp"
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"

#include <string>
#include <map>

#include "Vecteur.h"

class GestionnaireModelesTest;


///////////////////////////////////////////////////////////////////////////
/// @class Modele3D
/// @brief Classe qui encapsule un modèle 3d de la librairie 'assimp'.
///        Cette classe permet de charger un modèle 3d d'un fichier
///        exporté par un outil (en utilisant ladite librairie) et de
///        l'afficher.
///
/// @author Julien Gascon-Samson
/// @date 2011-07-21
///////////////////////////////////////////////////////////////////////////
class Modele3D
{
public:

	/// Pour les tests unitaires
	friend GestionnaireModelesTest;

	/// Création du modèle 3d
	Modele3D();

	/// Destruction du modèle 3d
	~Modele3D();

	/// Charger le modèle 3d à partir d'un fichier
    bool charger(const std::string& nomFichier);

	/// Effectuer le rendu
	void dessiner(bool avecTexture);

   /// Calcule la sphère englobante
   float calculerSphereEnglobante() const;

   /// Calcule le cylindre englobant
   void calculerCylindreEnglobant(float& rayon, float& bas, float& haut) const;

   /// Calcule le rayon du cylindre englobant
   void calculerBoiteEnglobante(Vecteur3& coinMin, Vecteur3& coinMax) const;

	/// Assigner le facteur d'agrandissement
	inline void assignerFacteurAgrandissement(float facteurAgrandissement);

	/// Obtenir le facteur d'agrandissement
	inline const float obtenirFacteurAgrandissement();

	/// Obtenir la scene
	const aiScene* obtenirScene() {return scene_;}

private:

	/// Charger les textures
	void chargerTextures();

	/// Appliquer un matériau
	void appliquerMateriau(const aiMaterial* materiau);

	/// Effectuer le rendu d'un noeud donné
	void dessinerNoeud(const aiScene* scene, const aiNode* noeud, bool avecTexture);

	/// Fonctions utilitaires :

	/// Appliquer une couleur OpenGL
	inline void appliquerCouleur4f(const aiColor4D* couleur);

	/// Remplir un tableau de couleurs
	inline void assignerFloat4(float f[4], float a, float b, float c, float d);

	/// Convertir un tableau de couleurs vers des 'float'
	inline void couleurVersFloat4(const struct aiColor4D *c, float f[4]);

	/// Recherche des points "extrêmes"
	void obtenirPointsExtremes(Vecteur3& xMin, Vecteur3& xMax,
										Vecteur3& yMin, Vecteur3& yMax, 
										Vecteur3& zMin, Vecteur3& zMax) const;

	/// Recherche des points "extrêmes"
	void obtenirPointsExtremes(Vecteur3& xMin, Vecteur3& xMax,
										Vecteur3& yMin, Vecteur3& yMax, 
										Vecteur3& zMin, Vecteur3& zMax,
										const aiScene* scene, const aiNode* noeud) const;

	/// Nom associé au modèle
	std::string nomModele_;

	/// Objet importeur assimp
	Assimp::Importer importer_;

	/// Objet scène assimp représentant le modèle chargé
	const aiScene* scene_;

	/// Identificateurs des textures OpenGL
	GLuint* identificateursTextures_;
	
	/// Map des textures, pour associer chaque nom de texture à un identificateur GLuint
	std::map<std::string, GLuint*> mapTextures_;

	/// Facteur d'agrandissement de l'objet
	float facteurAgrandissement_;
};

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::assignerFacteurAgrandissement( 
///                    float facteurAgrandissement )
///
/// Cette fonction permet d'assigner un facteur de zoom qui peut être
/// utilisé pour grossir ou réduir l'objet. Notez que vous n'êtes pas
/// obligés de recourir à cette fonction, vous pouvez aussi bien
/// manipuler les matrices OpenGL avant d'appeler la fonction de rendu.
///
/// @param[in] facteurAgrandissement : facteur d'agrandissement.
///            Un facteur de 1 correspond à la taille originale.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::assignerFacteurAgrandissement(float facteurAgrandissement)
{
	facteurAgrandissement_ = facteurAgrandissement;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::obtenirFacteurAgrandissement()
///
/// Cette fonction retourne le facteur d'agrandissement actuel de
/// l'objet, qui sera appliqué au rendu.
///
/// @return Le facteur d'agrandissement. Un facteur de 1 correspond à
///         la taille originale.
///
////////////////////////////////////////////////////////////////////////
const float Modele3D::obtenirFacteurAgrandissement()
{
	return facteurAgrandissement_;
}

#endif // __MODELE3D_H__

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
