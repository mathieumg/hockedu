///////////////////////////////////////////////////////////////////////////
/// @file NodeModelRender.cpp
/// @author Michael Ferris
/// @date 2012-04-12
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#include "NodeModelRender.h"

const Vecteur3 ZamboniDefaultSize(40,20,20);

#if WIN32
#include "GestionnaireModeles.h"
CreateListDelegateImplementation(Zamboni)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = ZamboniDefaultSize / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}
#endif


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeModelRender::NodeModelRender(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeModelRender::NodeModelRender( RazerKey  id)
   : NoeudComposite(id)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeModelRender::~NodeModelRender()
///
/// Ce destructeur d�sallouee la liste d'affichage du cube.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeModelRender::~NodeModelRender()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeModelRender::renderReal(  )
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeModelRender::renderReal() const
{
	// Appel � la version de la classe de base pour l'affichage des enfants.
	NoeudComposite::renderReal();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::tick( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps. L'animation est laisser au parent.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeModelRender::tick( const float& temps)
{
   
}

void NodeModelRender::renderOpenGLES() const
{
    
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
