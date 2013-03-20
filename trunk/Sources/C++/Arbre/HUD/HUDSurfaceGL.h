//////////////////////////////////////////////////////////////////////////////
/// @file HUDSurfaceGL.h
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include "vecteur.h"

typedef std::vector<Vecteur2f> ConteneurVertex2D;

///////////////////////////////////////////////////////////////////////////////
/// @class HUDSurfaceGL
/// @brief Classe qui représente une surface pouvant être dessinée dans le HUD.
///
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-09
//////////////////////////////////////////////////////////////////////////////
class HUDSurfaceGL:public HUDElement
{
public:
	/// Constructeur par paramètres
	HUDSurfaceGL(int typeSurfaceGL, ConteneurVertex2D* vertexes, Vecteur4f& couleur);
	/// Destructeur
	~HUDSurfaceGL();
	/// Accesseur sur les vertexes
	ConteneurVertex2D* obtenirVertexes();
	/// Assignation d'une texture
	void assignerTexture(std::string &nomTexture);
    /// Accessors of texture
    inline void setTexture(const unsigned int& pVal) { texture = pVal; }
protected:
	/// Appliquer la couleur
	virtual void peindreElement();
private:
	/// Conteneurs des vertexes
	ConteneurVertex2D* vertexes_;
	/// Le type de la surface
	int typeSurface_;
	/// La texture à appliquer
	unsigned int texture;



    /// La couleur à appliquer
	Vecteur4f couleur_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




