///////////////////////////////////////////////////////////////////////////////
/// @file NoeudPortail.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudPortail.h"
#include "FacadeModele.h"
#include <Box2D/Box2D.h>

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::NoeudPortail(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::NoeudPortail(const std::string& typeNoeud)
   : NoeudAbstrait(typeNoeud), champAttractionActif_(true)
{   

   FacadeModele::obtenirInstance()->ajouterElementSurTable(this);

   b2BodyDef myBodyDef;
   myBodyDef.type = b2_staticBody; //this will be a dynamic body
   myBodyDef.position.Set(0, 0); //set the starting position
   myBodyDef.angle = 0; //set the starting angle

   mPhysicBody = FacadeModele::obtenirInstance()->getWorld()->CreateBody(&myBodyDef);
   b2CircleShape circleShape;
   circleShape.m_p.Set(0, 0); //position, relative to body position
   circleShape.m_radius = (float32)obtenirRayon(); //radius

   b2FixtureDef myFixtureDef;
   myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
   myFixtureDef.density = 1;
   myFixtureDef.filter.categoryBits = CATEGORY_NONE;
   myFixtureDef.filter.maskBits = CATEGORY_NONE;

   mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::~NoeudPortail()
///
/// Ce destructeur d�sallouee la liste d'affichage du cube.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::~NoeudPortail()
{
	FacadeModele::obtenirInstance()->supprimerElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::afficherConcret() const
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::afficherConcret() const
{
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Appel � la version de la classe de base pour l'affichage des enfants.
	NoeudAbstrait::afficherConcret();

    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::animer( const float& temps)
{
	mAngle = (int)(mAngle+temps*1000.0)%360;
	updateMatrice();
   // Appel � la version de la classe de base pour l'animation des enfants.
   NoeudAbstrait::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::accueillirVisiteurNoeud( VisiteurNoeudComposite& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeudComposite & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::accueillirVisiteurNoeud( VisiteurNoeud& v )
{
	v.visiterNoeudPortail(this);
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
