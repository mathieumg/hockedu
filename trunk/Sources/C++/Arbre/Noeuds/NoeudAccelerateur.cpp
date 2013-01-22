///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAccelerateur.cpp
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudAccelerateur.h"
#include "FacadeModele.h"
#include "NoeudRondelle.h"
#include "GestionnaireAnimations.h"
#include "XMLUtils.h"
#include <Box2D/Box2D.h>
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAccelerateur::NoeudAccelerateur( const std::string& typeNoeud )
///
/// Constructeur de la classe qui charge le modele dans la banque de modele.
///
/// @param[in] const std::string & typeNoeud : Le type de noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudAccelerateur::NoeudAccelerateur(const std::string& typeNoeud)
	: NoeudAbstrait(typeNoeud),bonusAccel_(1.50), activer_(true)
{
	FacadeModele::getInstance()->ajouterElementSurTable(this);


	AnimationFrame* frame[5];
	frame[0] = new AnimationFrame(0, Vecteur3(50, 0, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[1] = new AnimationFrame(200, Vecteur3(0, 50, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[2] = new AnimationFrame(400, Vecteur3(-50, 0, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[3] = new AnimationFrame(600, Vecteur3(0, -50, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[4] = new AnimationFrame(800, Vecteur3(50, 0, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));

	Animation* animation = new Animation(BEZIER, true, true, true, true);
	for(int i=0; i<5; i++)
		animation->ajouterFrame(frame[i]);


	animation->ajouterObjet(this);

    updatePhysicBody();

	//GestionnaireAnimations::getInstance()->ajouterAnimation(animation);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAccelerateur::~NoeudAccelerateur(  )
///
/// Ce destructeur désallouee la liste d'affichage de l'accélérateur.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudAccelerateur::~NoeudAccelerateur()
{
	FacadeModele::getInstance()->supprimerElementSurTable(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::afficherConcret(  )
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::afficherConcret() const
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
/// @fn void NoeudAccelerateur::animer( const float& temps )
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] float temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::animer( const float& temps)
{
	mAngle = (int)(mAngle+temps*500.0)%360;
	updateMatrice();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'acceuillir les différents visiteur pour les transformations.
///
/// @param[in] VisiteurNoeud & v :  le visiteur qui fait la transformation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudAccelerateur(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* NoeudAccelerateur::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
/// @return TiXmlElement*
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* NoeudAccelerateur::creerNoeudXML()
{
	TiXmlElement* elementNoeud = NoeudAbstrait::creerNoeudXML();

    XMLUtils::ecrireAttribute(elementNoeud,"bonusAccel",bonusAccel_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAccelerateur::initialiser( const TiXmlElement* element )
///
/// Initialisation du NoeudAccelerateur à partir d'un element XML
///
/// @param[in] const TiXmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAccelerateur::initialiser( const TiXmlElement* element )
{
	if(!NoeudAbstrait::initialiser(element))
		return false;
	double doubleElem;
	if( !XMLUtils::LireAttribute(element,"bonusAccel",doubleElem) )
		return false;
	bonusAccel_ = doubleElem;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::gestionCollision( const float& temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::gestionCollision( const float& temps )
{
	NoeudRondelle* rondelle = FacadeModele::getInstance()->obtenirRondelle();
	Vecteur3 distance = obtenirPositionAbsolue()- rondelle->obtenirPositionAbsolue();
	double rayon = obtenirRayon()+rondelle->obtenirRayon();
	if(distance.norme2() > rayon*rayon+25)
	{
		modifierActiver(true);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::modifierActiver( bool val )
///
/// /*Description*/
///
/// @param[in] bool val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::modifierActiver( bool val )
{
	activer_ = val;
	surligne_ = !val;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::updatePhysicBody()
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
    myFixtureDef.filter.categoryBits = CATEGORY_NONE;
    myFixtureDef.filter.maskBits = CATEGORY_NONE;

    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
//     mPhysicBody->SetUserData(this);
//     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
#endif

}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



