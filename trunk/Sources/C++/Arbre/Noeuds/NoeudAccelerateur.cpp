///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAccelerateur.cpp
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudAccelerateur.h"
#include "FacadeModele.h"
#include "NoeudRondelle.h"
#include "GestionnaireAnimations.h"
#include "XMLUtils.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "VisiteurNoeud.h"

const float NoeudAccelerateur::DEFAULT_RADIUS = 7;

CreateListDelegateImplementation(Boost)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudAccelerateur::DEFAULT_RADIUS);
}

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
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);

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
	mAngle = (float)((int)(mAngle+temps*500.0f)%360);
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
/// @fn XmlElement* NoeudAccelerateur::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudAccelerateur::creerNoeudXML()
{
	XmlElement* elementNoeud = NoeudAbstrait::creerNoeudXML();

    XMLUtils::ecrireAttribute(elementNoeud,"bonusAccel",bonusAccel_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAccelerateur::initialiser( const XmlElement* element )
///
/// Initialisation du NoeudAccelerateur à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAccelerateur::initialiser( const XmlElement* element )
{
	if(!NoeudAbstrait::initialiser(element))
		return false;
	auto doubleElem = bonusAccel_;
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
	float rayon = obtenirRayon()+rondelle->obtenirRayon();
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

    // Il s'agit ici d'un boost qui peut entré en collision avec une rondell
    myFixtureDef.filter.categoryBits = CATEGORY_BOOST;
    myFixtureDef.filter.maskBits = CATEGORY_PUCK;

    // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
    myFixtureDef.isSensor = true;

    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
    mPhysicBody->SetUserData(this);
//     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
{
    if(pAnimationResult.CanUpdatedPosition())
        assignerPositionRelative(pAnimationResult.mPosition);
    if(pAnimationResult.CanUpdatedAngle())
        mAngle = pAnimationResult.mAngle[VZ];
    if(pAnimationResult.CanUpdatedScale())
        echelleCourante_ = pAnimationResult.mScale;
    updateMatrice();
}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



