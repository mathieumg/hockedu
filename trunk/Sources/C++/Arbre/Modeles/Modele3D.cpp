////////////////////////////////////////////////////////////////////////////////////
/// @file Modele3D.cpp
/// @author Julien Gascon-Samson
/// @date 2011-07-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

/*
	Mention : certaines portions du code source servant d'implémentation à cette classe sont inspirées
	de l'exemple SimpleTexturedOpenGL fourni avec le cadriciel de développement de la librairie AssImp.
	(Licence Apache/BSD)
*/

#include "Modele3D.h"
#include "Utilitaire.h"
#include "AideGL.h"
#include "RazerGameUtilities.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn Modele3D::Modele3D()
///
/// Ne fait qu'initialiser les variables membres de la classe.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
Modele3D::Modele3D():
	facteurAgrandissement_(1.0f,1.0f,1.0f), identificateursTextures_(NULL)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Modele3D::~Modele3D()
///
/// Destructeur qui désalloue les ressources allouées par 'assimp' ainsi
/// que les textures OpenGL.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
Modele3D::~Modele3D()
{
	// Libérer les textures
	glDeleteTextures((GLsizei)mapTextures_.size(), identificateursTextures_);
    if(identificateursTextures_)
    {
	    delete [] identificateursTextures_;
    }

	// Libérer la scène
	scene_ = NULL;

	importer_.FreeScene();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::charger( const std::string& nomModele, const std::string& nomFichier )
///
/// Cette fonction charge un modèle 3D à partir d'un fichier supporté
/// par la librairie 'assimp'. Les textures OpenGL afférentes sont
/// également chargées.
///
/// @param[in] nomFichier : nom du fichier modèle (normalement .mtl)
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
bool Modele3D::charger( const std::string& nomFichier )
{
	if(!utilitaire::fichierExiste(nomFichier))
	{
		utilitaire::afficherErreur("Fichier du modele "+nomFichier+" Inexistant");
		return false;
	}
	// Construire l'objet scene
	const aiScene* scene = importer_.ReadFile(nomFichier, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene==NULL) {
		utilitaire::afficherErreur("Impossible de charger l'objet 3d!");
		return false;
	}

	scene_ = scene;

	// Charger les textures
	chargerTextures();
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::chargerTextures()
///
/// Cette fonction charge les textures OpenGL associées au modèle
/// 'assimp' préalablement chargé.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::chargerTextures()
{
	if (scene_->HasTextures()) {
		utilitaire::afficherErreur("Les textures intégrées ne sont pas prises en charge pour le moment.");
	}

	// Itérer parmi l'ensemble des matériaux
	for (unsigned int i=0; i<scene_->mNumMaterials; i++) {
		aiMaterial* materiau = scene_->mMaterials[i];
        aiString matName;
        aiGetMaterialString(materiau,AI_MATKEY_NAME,&matName);
        mapMaterial_[matName.data] = materiau;
		// POINT D'EXTENSION : il vous serait possible de prendre en charge d'autres types de textures que celle
		// en diffusion (DIFFUSE)...

		// Parcourir l'ensemble des textures pour ce matériau et stocker les noms de fichiers
		// Par la suite, nous initialiserons la mémoire de textures OpenGL et procéderons
		// au chargement des fichiers graphiques.
		int idTexture = 0;
		while (true) {

			// Chaîne UTF-8 (fort possiblement compatible ASCII) qui contient le nom de fichier associé à la texture
			// On peut l'utiliser comme une chaîne C standard (char *) sans réel risque (selon la doc. assimp !)
			aiString cheminTexture = "";

			// Récupérer le chemin vers la texture...
			aiReturn retour = materiau->GetTexture(aiTextureType_DIFFUSE, idTexture, &cheminTexture);
			// Vérifie que la texture est valide...
			if (retour != AI_SUCCESS)
				break;

			// Stocker le nom de texture dans le tableau associatif
			mapTextures_[cheminTexture.data] = NULL;
			
			idTexture++;
		}
	}

	// Pour chaque texture trouvée, procéder au chargement
	
	// Nombre de textures
	GLsizei nombreTextures = (GLsizei)mapTextures_.size();

	// Générer les textures OpenGL
	identificateursTextures_ = new GLuint[nombreTextures];
	glGenTextures(nombreTextures, identificateursTextures_);

	// Itérateur STL
	std::map<std::string, GLuint*>::iterator itr = mapTextures_.begin();

	for (int j=0; j<nombreTextures; j++) {
		std::string nomFichier = itr->first;
		itr->second = &(identificateursTextures_[j]);
		itr++;

		// Charger la texture
		aidegl::glLoadTexture( RazerGameUtilities::NOM_DOSSIER_MEDIA + nomFichier, identificateursTextures_[j], false);
	}

}

bool Modele3D::appliquerMateriau(const std::string& materialName)
{
    auto it = mapMaterial_.find(materialName);
    if(it != mapMaterial_.end())
    {
        appliquerMateriau(it->second);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::appliquerMateriau( const aiMaterial* materiau )
///
/// Cette fonction applique un matériau 'assimp' à l'état OpenGL 
/// courant (puisque certains meshes peuvent en dépendre). Le code est
/// chaotique; rassurons-nous cette fonction ne fait qu'effectuer
/// des appels OpenGL selon l'état de la structure interne du matériau
/// 'assimp' ainsi que quelques calculs.
///
/// @param[in] materiau : matériau 'assimp' à appliquer
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::appliquerMateriau(const aiMaterial* materiau)
{
	// Obtenir la texture du matériau
	int indexTexture = 0;
	aiString nomFichier = "";
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();

	if (materiau->GetTexture(aiTextureType_DIFFUSE, indexTexture, &nomFichier) == AI_SUCCESS) {
		// Activer le texturage OpenGL et lier la texture appropriée
		glEnable ( GL_TEXTURE_2D);
		GLuint* idTexture = mapTextures_[nomFichier.data];
		glScalef(1.0,-1.0,1.0);
		glBindTexture(GL_TEXTURE_2D, *idTexture);
	}
	else {
		// Désactiver le texturage OpenGL puisque cet objet n'a aucune texture
		glDisable ( GL_TEXTURE_2D);
	}

	glMatrixMode(GL_MODELVIEW);

	// Autres paramètres à appliquer... (couleurs)
	float c[4];
	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changé pour: unsigned
	assignerFloat4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	//assignerFloat4(c, 1.0f, 1.0f, 1.0f, 1.0f);
	
	
	if (aiGetMaterialColor(materiau, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
		couleurVersFloat4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
	
	assignerFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(materiau, AI_MATKEY_COLOR_SPECULAR, &specular))
		couleurVersFloat4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	assignerFloat4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(materiau, AI_MATKEY_COLOR_AMBIENT, &ambient))
		couleurVersFloat4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	assignerFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(materiau, AI_MATKEY_COLOR_EMISSIVE, &emission))
		couleurVersFloat4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
	
	max = 1;
	ret1 = aiGetMaterialFloatArray(materiau, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(materiau, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		assignerFloat4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	


	}


	
	
	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(materiau, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(materiau, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::appliquerCouleur4f( const aiColor4D* couleur )
///
/// Cette fonction applique une couleur 'assimp' à l'état OpenGL
/// courant.
///
/// @param[in] couleur : couleur 'assimp' à appliquer
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::appliquerCouleur4f(const aiColor4D* couleur)
{
	glColor4f(couleur->r, couleur->g, couleur->b, couleur->a);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::assignerFloat4( float f[4], float a, float b,
///                                    float c, float d )
///
/// Fonction 'helper' qui assigne 4 éléments 'float' à un tableau
///
/// @param[in] a : premier élément 'float'
/// @param[in] b : second élément 'float'
/// @param[in] c : troisième élément 'float'
/// @param[in] d : quatrième élément 'float'
/// @param[in] f : tableau de 'float'
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::assignerFloat4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::couleurVersFloat4( float f[4], float a, float b,
///                                       float c, float d )
///
/// Fonction 'helper' qui convertit une couleur 'assimp' en tableau
///
/// @param[in] c : structure de couleur 'assimp'
/// @param[in] f : tableau de 'float'
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::couleurVersFloat4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::dessiner()
///
/// Cette fonction effectue le rendu récursif OpenGL du modèle 'assimp'
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::dessiner(bool avecTexture)
{
    glPushMatrix();
    glScalef(facteurAgrandissement_[VX], facteurAgrandissement_[VY], facteurAgrandissement_[VZ]);

	dessinerNoeud(scene_, scene_->mRootNode, avecTexture);

    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Modele3D::dessinerNoeud( const aiScene* scene,
///                                   const aiNode* noeud,
///									  bool avecTexture)
///
/// Cette fonction dessine un noeud donné de la scène 'assimp'. Le
/// rendu est récursif; cette fonction sera donc appelée en boucle pour
/// les noeuds-enfants.
///
/// Cette fonction procède également à l'application des matériaux,
/// textures et des matrices OpenGL.
///
/// @param[in] scene : objet scène 'assimp'
/// @param[in] noeud : objet noeud 'assimp'
/// @param[in] avecTexture : dit si la texture de l'objet doit etre dessinee
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void Modele3D::dessinerNoeud(const aiScene* scene, const aiNode* noeud, bool avecTexture)
{
	// Matrice de transformation
	//aiMatrix4x4 m = noeud->mTransformation;

	//m.Scaling(aiVector3D(facteurAgrandissement_[VX],facteurAgrandissement_[VY],facteurAgrandissement_[VZ]), m);

	//m.Transpose();
	glPushMatrix();
	//glMultMatrixf((float*)&m);

	for (unsigned int i=0; i<noeud->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[noeud->mMeshes[i]];

		// Appliquer le matériau pour le mesh courant
		if(avecTexture)
		{
			appliquerMateriau(scene->mMaterials[mesh->mMaterialIndex]);
			//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

			// Pas besoin de ce check
			/*if(mesh->mNormals == NULL)
			{
				glDisable(GL_LIGHTING);
			}
			else
			{
				//glEnable(GL_LIGHTING);
			}*/

			if(mesh->mColors[0] != NULL)
			{
				glEnable(GL_COLOR_MATERIAL);
			}
			else
			{
				glDisable(GL_COLOR_MATERIAL);
			}
		}
		else
		{
			glDisable(GL_LIGHTING);
			
			glColor3f(0, 1, 0);
			//glEnable(GL_COLOR_LOGIC_OP);
			//glLogicOp(GL_XOR);
		}

		// Effectuer le rendu de chaque face
		for (unsigned int j=0; j<mesh->mNumFaces; j++) {
			const aiFace* face = &mesh->mFaces[j];
			GLenum modeRendu;
			switch (face->mNumIndices) {
				case 1: modeRendu = GL_POINTS; break;
				case 2: modeRendu = GL_LINES; break;
				case 3: modeRendu = GL_TRIANGLES; break;
				default: modeRendu = GL_POLYGON; break;
			}

			// Effectuer le rendu de la face
			glBegin(modeRendu);

			for (unsigned int k=0; k<face->mNumIndices; k++) {
				int indexVertex = face->mIndices[k];	// get group index for current index

				// Le mesh possède-t-il une couleur ?
				if(mesh->mColors[0] != NULL)
					appliquerCouleur4f(&mesh->mColors[0][indexVertex]);
				
				// Normales pour applications des textures
				if(mesh->mNormals != NULL)
				{
					if(mesh->HasTextureCoords(0))
					{
						glTexCoord2f(mesh->mTextureCoords[0][indexVertex].x, 1 - mesh->mTextureCoords[0][indexVertex].y);
					}
					else
					{
						//glDisable(GL_TEXTURE_2D); // Il faut la laisser commente sinon le shader ou qqch du genre crash
					}
				}

				glNormal3fv(&mesh->mNormals[indexVertex].x);
				glVertex3fv(&mesh->mVertices[indexVertex].x);
			}

			glEnd();
		}
		
		if(avecTexture)
		{
			glMatrixMode(GL_TEXTURE);
			glPopMatrix();
		}
		else
		{
			//glDisable(GL_COLOR_LOGIC_OP);
		}
		
		glMatrixMode(GL_MODELVIEW);
	}

	// Rendu récursif des enfants
	for (unsigned int i=0; i<noeud->mNumChildren; i++) {
		dessinerNoeud(scene, noeud->mChildren[i], avecTexture);
	}
	
	glPopMatrix();
}

float Modele3D::calculerSphereEnglobante() const
{
	Vecteur3 xMin,xMax,yMin,yMax,zMin,zMax;
	this->obtenirPointsExtremes(xMin,xMax,yMin,yMax,zMin,zMax);

	// Le rayon de la sphère englobante est le point le plus éloigné parmi les 6 points extrêmes possibles...
	float max = 0;
	float norme = 0;
	norme = xMin.norme2(); if (norme>max) max=norme;
	norme = yMin.norme2(); if (norme>max) max=norme;
	norme = zMin.norme2(); if (norme>max) max=norme;
	norme = xMax.norme2(); if (norme>max) max=norme;
	norme = yMax.norme2(); if (norme>max) max=norme;
	norme = zMax.norme2(); if (norme>max) max=norme;

	return sqrt(max);
}

void Modele3D::calculerCylindreEnglobant(float& rayon, float& bas, float& haut) const
{
	Vecteur3 xMin,xMax,yMin,yMax,zMin,zMax;
	this->obtenirPointsExtremes(xMin,xMax,yMin,yMax,zMin,zMax);

	// Obtenir le point extrême le plus éloigné parmi les axes xy. L'axe z servira
	// à établir le bas et le haut du cylindre.

	float max = 0;
	float norme = 0;
	norme = xMin[0]*xMin[0]+xMin[1]*xMin[1]; if (norme>max) max=norme;
	norme = yMin[0]*yMin[0]+yMin[1]*yMin[1]; if (norme>max) max=norme;
	norme = zMin[0]*zMin[0]+zMin[1]*zMin[1]; if (norme>max) max=norme;
	norme = xMax[0]*xMax[0]+xMax[1]*xMax[1]; if (norme>max) max=norme;
	norme = yMax[0]*yMax[0]+yMax[1]*yMax[1]; if (norme>max) max=norme;
	norme = zMax[0]*zMax[0]+zMax[1]*zMax[1]; if (norme>max) max=norme;

	// Nous avons trouvé le rayon...
	rayon = sqrt(max);

	// Le bas et le dessus sont les extrémums en z.
	bas = 999999; haut = 0;
	if (xMin[2]<bas) bas=xMin[2]; if (xMin[2]>haut) haut=xMin[2];
	if (yMin[2]<bas) bas=yMin[2]; if (yMin[2]>haut) haut=yMin[2];
	if (zMin[2]<bas) bas=zMin[2]; if (zMin[2]>haut) haut=zMin[2];
	if (xMax[2]<bas) bas=xMax[2]; if (xMax[2]>haut) haut=xMax[2];
	if (yMax[2]<bas) bas=yMax[2]; if (yMax[2]>haut) haut=yMax[2];
	if (zMax[2]<bas) bas=zMax[2]; if (zMax[2]>haut) haut=zMax[2];
}

void Modele3D::calculerBoiteEnglobante(Vecteur3& coinMin, Vecteur3& coinMax) const
{
	Vecteur3 xMin,xMax,yMin,yMax,zMin,zMax;
	this->obtenirPointsExtremes(xMin,xMax,yMin,yMax,zMin,zMax);

	// Le coin min est simplement les coordonnées minimales et le coin max est
	// simplement les coordonnées maximales.
	coinMin[0]=xMin[0]; coinMin[1]=yMin[1]; coinMin[2]=zMin[2];
	coinMax[0]=xMax[0]; coinMax[1]=yMax[1]; coinMax[2]=zMax[2];
}

void Modele3D::obtenirPointsExtremes(Vecteur3& xMin, Vecteur3& xMax,
												 Vecteur3& yMin, Vecteur3& yMax, 
												 Vecteur3& zMin, Vecteur3& zMax) const
{
	this->obtenirPointsExtremes(xMin, xMax, yMin, yMax, zMin, zMax, scene_, scene_->mRootNode);
}

void Modele3D::obtenirPointsExtremes(Vecteur3& xMin, Vecteur3& xMax,
												 Vecteur3& yMin, Vecteur3& yMax, 
												 Vecteur3& zMin, Vecteur3& zMax,
												 const aiScene* scene, const aiNode* noeud) const
{
	// Pour chacun des 'meshes'...
	for (unsigned int i=0; i<noeud->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[noeud->mMeshes[i]];

		// Pour chaque face...
		for (unsigned int j=0; j<mesh->mNumFaces; j++) {
			const aiFace* face = &mesh->mFaces[j];

			// Pour chaque vertex...
			for (unsigned int k=0; k<face->mNumIndices; k++) {
				int indexVertex = face->mIndices[k];	// get group index for current index

				// On vérifie les coordonnées...
				float x=mesh->mVertices[indexVertex].x;
				float y=mesh->mVertices[indexVertex].y;
				float z=mesh->mVertices[indexVertex].z;

				if (x<xMin[0]) { xMin[0]=x; xMin[1]=y; xMin[2]=z; } else if (x>xMax[0]) { xMax[0]=x; xMax[1]=y; xMax[2]=z; }
				if (y<yMin[1]) { yMin[0]=x; yMin[1]=y; yMin[2]=z; } else if (y>yMax[1]) { yMax[0]=x; yMax[1]=y; yMax[2]=z; }
				if (z<zMin[2]) { zMin[0]=x; zMin[1]=y; zMin[2]=z; } else if (z>zMax[2]) { zMax[0]=x; zMax[1]=y; zMax[2]=z; }
			}
		}
	}

	// Calcul récursif des points extrêmes
	for (unsigned int i=0; i<noeud->mNumChildren; i++) {
		obtenirPointsExtremes(xMin,xMax,yMin,yMax,zMin,zMax,scene,noeud->mChildren[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
