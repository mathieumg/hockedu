//////////////////////////////////////////////////////////////////////////////
/// @file HUDSurfaceGL.cpp
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDSurfaceGL.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDSurfaceGL::HUDSurfaceGL(int typeSurfaceGL, ConteneurVertex2D* vertexes, Vecteur4& couleur)
///
/// Constructeur par paramètres.
///
/// @param[in] int typeSurfaceGL : le type de la surface.
/// @param[in] ConteneurVertex2D* vertexes : le conteneur des vertexes de la surface
/// @param[in] Vecteur4& couleur : la couleur de la surface
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDSurfaceGL::HUDSurfaceGL(int typeSurfaceGL, ConteneurVertex2D* vertexes, Vecteur4f& couleur):
typeSurface_(typeSurfaceGL), vertexes_(vertexes), couleur_(couleur), texture(0)
{
	// Extrapolation de la surface en rectangle (facilite le placement)
	float hauteur = 0;
	float largeur = 0;
	float plusPetitX = vertexes_->operator[](0)[VX];
	float plusPetitY = vertexes_->operator[](0)[VY];
	for(int i = 0; i < (int)vertexes_->size()-1; ++i)
	{
		for(int j = i+1; j < (int)vertexes_->size(); ++j)
		{
			float temp = abs(vertexes_->operator[](i)[VX] - vertexes_->operator[](j)[VX]);
			if(largeur < temp)
				largeur = temp;

			temp = abs(vertexes_->operator[](i)[VY] - vertexes_->operator[](j)[VY]);
			if(hauteur < temp)
				hauteur = temp;

			temp = vertexes_->operator[](j)[VX];
			if(plusPetitX > temp)
				plusPetitX = temp;

			temp = vertexes_->operator[](j)[VY];
			if(plusPetitY > temp)
				plusPetitY = temp;
		}
	}
	modifierPosition(plusPetitX, plusPetitY);
	modifierTaille(largeur, hauteur);
	//Calcul des coordonnées relatives des vertex par rapport à la surface.
	for(int i = 0; i < (int)vertexes_->size(); ++i)
	{
		vertexes_->operator[](i)[VX] = (vertexes_->operator[](i)[VX] - plusPetitX) / largeur;
		vertexes_->operator[](i)[VY] = (vertexes_->operator[](i)[VY] - plusPetitY) / hauteur;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn ConteneurVertex2D* HUDSurfaceGL::obtenirVertexes()
///
/// Accesseur au conteneurs des vertexes de la surface.
///
/// @return : le conteneur des vertexes.
///
////////////////////////////////////////////////////////////////////////
ConteneurVertex2D* HUDSurfaceGL::obtenirVertexes()
{
	return vertexes_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDSurfaceGL::peindreElement()
///
/// Applique une couleur à la surface.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDSurfaceGL::peindreElement()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(couleur_[0], couleur_[1], couleur_[2], couleur_[3]);
	glTranslatef(obtenirX(), obtenirY(), 0);

	/*glBegin(GL_QUADS);
	glVertex2d(0,0);
	glVertex2d(0,obtenirHauteur());
	glVertex2d(obtenirLargeur(), obtenirHauteur());
	glVertex2d(obtenirLargeur(), 0);
	glEnd();*/

	if(texture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glBegin(typeSurface_);
	for(int i=0;i<(int)vertexes_->size();++i)
	{
		glTexCoord2f(vertexes_->operator[](i)[VX],vertexes_->operator[](i)[VY]);
		glVertex2f(obtenirLargeur()*vertexes_->operator[](i)[VX],obtenirHauteur()*vertexes_->operator[](i)[VY]);
	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDSurfaceGL::~HUDSurfaceGL()
///
/// Destructeur.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDSurfaceGL::~HUDSurfaceGL()
{
	delete vertexes_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDSurfaceGL::assignerTexture(std::string &nomTexture)
///
/// Assignation d'une texture.
///
/// @param[out] std::string &nomTexture : le nom de la texture à assigner.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDSurfaceGL::assignerTexture(std::string &nomTexture)
{
	aidegl::glLoadTexture(nomTexture, texture);
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



