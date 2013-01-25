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
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::NoeudPortail(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::NoeudPortail(const std::string& typeNoeud)
   : NoeudAbstrait(typeNoeud), mIsAttractionFieldActive(true)
{   

   FacadeModele::getInstance()->ajouterElementSurTable(this);

    updatePhysicBody();

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::~NoeudPortail()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::~NoeudPortail()
{
	FacadeModele::getInstance()->supprimerElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::afficherConcret() const
{
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Appel à la version de la classe de base pour l'affichage des enfants.
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
	mAngle = (float)((int)(mAngle+temps*1000.0f)%360);
	updateMatrice();
   // Appel à la version de la classe de base pour l'animation des enfants.
   NoeudAbstrait::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::acceptVisitor( VisiteurNoeudComposite& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeudComposite & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudPortail(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    clearPhysicsBody();

    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody; //this will be a dynamic body
    Vecteur3 pos = obtenirPositionAbsolue();
    b2Vec2 posB2;
    utilitaire::VEC3_TO_B2VEC(pos,posB2);
    myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
    myBodyDef.angle = 0; //set the starting angle

    mPhysicBody = getWorld()->CreateBody(&myBodyDef);
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = (float32)obtenirRayon()*utilitaire::ratioWorldToBox2D; //radius

    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
    myFixtureDef.density = 1;

    // Il s'agit ici d'un portail qui peut entré en collision avec une rondell
    myFixtureDef.filter.categoryBits = CATEGORY_PORTAL;
    myFixtureDef.filter.maskBits = CATEGORY_PUCK;

    // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
    myFixtureDef.isSensor = true;

    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
    mPhysicBody->SetUserData(this);
    mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
#endif
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
