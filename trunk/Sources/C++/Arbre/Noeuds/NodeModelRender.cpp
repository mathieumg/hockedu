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
#include "GestionnaireModeles.h"


const Vecteur3 ZamboniDefaultSize(40,20,20);

CreateListDelegateImplementation(Zamboni)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = ZamboniDefaultSize / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeModelRender::NodeModelRender(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeModelRender::NodeModelRender(const std::string& typeNoeud)
   : NoeudComposite(typeNoeud)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeModelRender::~NodeModelRender()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeModelRender::~NodeModelRender()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeModelRender::afficherConcret(  )
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeModelRender::afficherConcret() const
{
	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudComposite::afficherConcret();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps. L'animation est laisser au parent.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeModelRender::animer( const float& temps)
{
   
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
