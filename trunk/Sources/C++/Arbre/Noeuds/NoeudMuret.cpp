///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMuret.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudMuret.h"
#include <Box2D/Box2D.h>
#include "Utilitaire.h"
#include "VisiteurCollision.h"
#include "FacadeModele.h"
#include "Modele3D.h"
#include "tinyxml.h"
#include "XMLUtils.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuret::NoeudMuret( const std::string& typeNoeud, bool estSurTable )
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud.
/// @param[in] bool estSurTable : si le muret est sur la table ou si c'est une bordure
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuret::NoeudMuret(const std::string& typeNoeud, bool estSurTable)
	: NoeudComposite(typeNoeud)
{
	if(estSurTable)
		FacadeModele::getInstance()->ajouterElementSurTable(this);
	coefRebond_ = 0.75;
    updatePhysicBody();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::~NoeudMuret()
///
/// Ce destructeur désallouee la liste d'affichage du muret.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuret::~NoeudMuret()
{
	FacadeModele::getInstance()->supprimerElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::updatePhysicBody()
{
#if BOX2D_INTEGRATED

    clearPhysicsBody();

    float32 halfLength = (float32)echelleCourante_[VX]/2.f*utilitaire::ratioWorldToBox2D;//(float)(coin2-coin1).norme()/2.f;

    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody; //this will be a dynamic body
    myBodyDef.position.Set(0, 0); //set the starting position
    myBodyDef.angle = 0; //set the starting angle

    mPhysicBody = getWorld()->CreateBody(&myBodyDef);
    b2PolygonShape shape;
    Vecteur3 pos = obtenirPositionAbsolue();
    b2Vec2 posB2;
    utilitaire::VEC3_TO_B2VEC(pos,posB2);
    myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
    shape.SetAsBox(halfLength,2.5f*utilitaire::ratioWorldToBox2D,b2Vec2(posB2.x,posB2.y),utilitaire::DEG_TO_RAD(mAngle));

    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &shape; //this is a pointer to the shape above
    myFixtureDef.density = 1;
    myFixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
    myFixtureDef.filter.maskBits = CATEGORY_PUCK | CATEGORY_MALLET;

    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
#endif

}



////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::afficherConcret() const
{
	// Sauvegarde de la matrice.
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0.76f, 0.64f, 0.31f);

	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudComposite::afficherConcret();

	// Restauration de la matrice.
	glPopAttrib();
	glPopMatrix();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::animer( const float& temps)
{
	// Appel à la version de la classe de base pour l'animation des enfants.
	NoeudComposite::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::acceptVisitor( const VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudMuret(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirCoin1()
///
/// Retourne la position du coin1.
///
/// @param[in] void
///
/// @return Vecteur3 positionCoin1 : vecteur contenant la position.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuret::obtenirCoin1()
{
	return positionCoin1_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirCoin2()
///
/// Retourne la position du coin2.
///
/// @param[in] void
///
/// @return Vecteur3 positionCoin2 : vecteur contenant la position.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuret::obtenirCoin2()
{
	return positionCoin2_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::majPosCoins(  )
///
/// Permet de recalculer la position des coin des muret suite a une rotation
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::majPosCoins()
{
	//double deltaX = (cos(utilitaire::DEG_TO_RAD(mAngle)) *echelleCourante_[VX])/2.0;
	//double deltaY = (sin(utilitaire::DEG_TO_RAD(mAngle)) *echelleCourante_[VX])/2.0;
	Vecteur3 deplacement( cos(utilitaire::DEG_TO_RAD(mAngle) ), sin(utilitaire::DEG_TO_RAD(mAngle) ) );
	deplacement*= echelleCourante_[VX];
	deplacement /= 2.0;
	positionCoin1_ = positionRelative_+deplacement;
	positionCoin2_ = positionRelative_-deplacement;
    updatePhysicBody();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::trouverAngle(  )
///
/// Permet de calculer l'angle du muret selon ces coins
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::trouverAngle()
{
	Vecteur3 coin1Relatif(positionCoin1_-positionRelative_);
	coin1Relatif.normaliser();
	mAngle = (360.0+(utilitaire::RAD_TO_DEG(acos(coin1Relatif[VX]))*( (coin1Relatif[VY] < 0)?-1:1) ) );
	mAngle /= 360.0;
	mAngle -= (int)(mAngle);
	mAngle *= 360.0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::assignerPositionCoin( int lequel, Vecteur3 position )
///
/// Modifie la position des coins
///
/// @param[in] int			lequel		: lequel des deux coins ( 1 ou 2)
///				Vecteur3	position	: position a assigner
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerPositionCoin( int lequel, Vecteur3 position )
{
	if(lequel==1)
	{
		positionCoin1_ = position;
	}
	else if(lequel==2)
	{
		positionCoin2_ = position;
	}


	double	deltaX = positionCoin2_[VX]-positionCoin1_[VX],
		deltaY = positionCoin2_[VY]-positionCoin1_[VY];

	NoeudAbstrait::assignerPositionRelative(Vecteur3(positionCoin1_[VX]+deltaX/2.0, positionCoin1_[VY]+deltaY/2.0, 0));
	trouverAngle();
    updatePhysicBody();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirRayon() const
///
/// Retourne le rayon du noeud.
///
/// @param[in]	void
///
/// @return double : rayon du noeud
///
////////////////////////////////////////////////////////////////////////
double NoeudMuret::obtenirRayon() const
{
	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::assignerAttributVisiteurCollision( VisiteurCollision* v )
///
/// Permet d'assigner les attribut nécessaire à la collision.
///
/// @param[in] VisiteurCollision * v : le visiteur de collision.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerAttributVisiteurCollision( VisiteurCollision* v )
{
	v->modifierTypeCollision(SEGMENT);
	v->modifierCoin1(positionCoin1_);
	v->modifierCoin2(positionCoin2_);
	v->modifierRayonAVerifier(obtenirRayon());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* NoeudMuret::creerNoeudXML()
///
/// /*Description*/
///
///
/// @return TiXmlElement*
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* NoeudMuret::creerNoeudXML()
{
	TiXmlElement* elementNoeud = NoeudComposite::creerNoeudXML();
	// Ajouter la position des coins des murets
	elementNoeud->SetDoubleAttribute("coefRebond",coefRebond_);
    XMLUtils::ecrireVecteur3Dxml(&positionCoin1_,elementNoeud,"coinA");
    XMLUtils::ecrireVecteur3Dxml(&positionCoin2_,elementNoeud,"coinB");
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudMuret::initialiser( const TiXmlElement* element )
///
/// Initialisation du NoeudMuret à partir d'un element XML
///
/// @param[in] const TiXmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudMuret::initialiser( const TiXmlElement* element )
{
	if(!NoeudComposite::initialiser(element))
		return false;
	double doubleElem;
	if( element->QueryDoubleAttribute("coefRebond", &doubleElem) != TIXML_SUCCESS )
		return false;
	coefRebond_ = doubleElem;

    if( !XMLUtils::lectureVecteur3Dxml(&positionCoin1_,element,"coinA") )
		return false;
    if( !XMLUtils::lectureVecteur3Dxml(&positionCoin2_,element,"coinB") )
		return false;
	trouverAngle();
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::assignerAngle( double angle )
///
/// Permet de modifier l'angle du muret et garde les coins a jour
///
/// @param[in] double angle : nouvel angle du muret
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerAngle( const double& angle )
{
	NoeudAbstrait::assignerAngle(angle);
	majPosCoins();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::modifierEchelleCourante( const Vecteur3& echelleCourante )
///
/// Mutateur des facteurs d'echelle et conserve les coins a jour
///
/// @param[in] const Vecteur3 & echelleCourante
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::modifierEchelleCourante( const Vecteur3& echelleCourante )
{
	NoeudAbstrait::modifierEchelleCourante(echelleCourante);
	majPosCoins();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::assignerPositionRelative( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerPositionRelative( const Vecteur3& positionRelative )
{
	NoeudAbstrait::assignerPositionRelative(positionRelative);
	majPosCoins();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////