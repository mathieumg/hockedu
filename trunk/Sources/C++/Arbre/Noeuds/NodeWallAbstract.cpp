///////////////////////////////////////////////////////////////////////////////
/// @file NodeWallAbstract.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NodeWallAbstract.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "VisiteurCollision.h"
#include "FacadeModele.h"
#include "Modele3D.h"
#include "XMLUtils.h"
#include "GestionnaireModeles.h"

const Vecteur3 NodeWallAbstract::DEFAULT_SIZE = Vecteur3(1, 5, 15);

CreateListDelegateImplementation(Wall)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = NodeWallAbstract::DEFAULT_SIZE / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallAbstract::NodeWallAbstract( const std::string& typeNoeud, bool estSurTable )
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud.
/// @param[in] bool estSurTable : si le muret est sur la table ou si c'est une bordure
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeWallAbstract::NodeWallAbstract(const std::string& typeNoeud)
	: Super(typeNoeud)
{
	coefRebond_ = 0.75;
    updatePhysicBody();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeWallAbstract::~NodeWallAbstract()
///
/// Ce destructeur d�sallouee la liste d'affichage du muret.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeWallAbstract::~NodeWallAbstract()
{
	FacadeModele::getInstance()->supprimerElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::updatePhysicBody()
{
#if BOX2D_INTEGRATED

    if(getWorld())
    {
        clearPhysicsBody();

        float halfLength = echelleCourante_[VX]*DEFAULT_SIZE[VX]/2.f*utilitaire::ratioWorldToBox2D;
        float halfHeight = echelleCourante_[VY]*DEFAULT_SIZE[VY]/2.f*utilitaire::ratioWorldToBox2D;

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
        shape.SetAsBox(halfLength,halfHeight,b2Vec2(posB2.x,posB2.y),utilitaire::DEG_TO_RAD(mAngle));

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shape above
        myFixtureDef.density = 1;
        myFixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
        myFixtureDef.filter.maskBits = CATEGORY_PUCK | CATEGORY_MALLET;

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
//     mPhysicBody->SetUserData(this);
//     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
    }
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::afficherConcret() const
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::afficherConcret() const
{
	// Sauvegarde de la matrice.
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0.76f, 0.64f, 0.31f);

	// Appel � la version de la classe de base pour l'affichage des enfants.
	Super::afficherConcret();

	// Restauration de la matrice.
	glPopAttrib();
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::assignerAttributVisiteurCollision( VisiteurCollision* v )
///
/// Permet d'assigner les attribut n�cessaire � la collision.
///
/// @param[in] VisiteurCollision * v : le visiteur de collision.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::assignerAttributVisiteurCollision( VisiteurCollision* v )
{
	v->modifierTypeCollision(SEGMENT);
	v->modifierCoin1(obtenirCoin1());
	v->modifierCoin2(obtenirCoin2());
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
void NodeWallAbstract::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNoeudMuret(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NodeWallAbstract::creerNoeudXML()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NodeWallAbstract::creerNoeudXML()
{
	XmlElement* elementNoeud = Super::creerNoeudXML();
	// Ajouter la position des coins des murets
	XMLUtils::writeAttribute(elementNoeud,"coefRebond",coefRebond_);
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallAbstract::initialiser( const XmlElement* element )
///
/// Initialisation du NodeWallAbstract � partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallAbstract::initialiser( const XmlElement* element )
{
	if(!Super::initialiser(element))
		return false;
	float floatElem;
	if( !XMLUtils::readAttribute(element,"coefRebond",floatElem) )
		return false;
	coefRebond_ = floatElem;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const std::string& NodeWallAbstract::get3DModelKey()
///
/// accessor to the key to retrieve the list and/or 3D Model
///
///
/// @return const std::string&
///
////////////////////////////////////////////////////////////////////////
const std::string& NodeWallAbstract::get3DModelKey() const
{
    // in all case for the wall we want to use the same model
    return RazerGameUtilities::NOM_MURET;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::updateWallProperties()
///
/// updates angle, scale and position according to its corners
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::updateWallProperties()
{
    Vecteur3 corner1 = obtenirCoin1(), corner2 = obtenirCoin2();
    Vecteur3 vecteurEntre(corner1-corner2);

    // just to be sure
    vecteurEntre[VZ] = 0;

    // arctan(y/x) = angle
    float angle = utilitaire::RAD_TO_DEG(atan2(vecteurEntre[VY], vecteurEntre[VX]));
    if(angle<0)
    {
        angle += 360.f;
    }

    // pour conserver l'echelle en Y et Z
    Vecteur3 echelle;
    obtenirEchelleCourante(echelle);
    float distance = vecteurEntre.norme();
    echelle[VX] = distance / DEFAULT_SIZE[VX];

    /* pour s'assurer de ne pas appeler la version virtuelle
    * puisque la modification de ceux-ci pourrait
    * entrainer une mise a jour des coins, ce qui n'est pas
    * desire dans cette situation
    * Aussi, on n'utilise pas les accesseurs pour eviter
    * de refaire la matrice de tranformation et 
    * le body box2d plusieurs fois pour rien
    */
    positionRelative_ = corner2+(vecteurEntre/2.0f);
    echelleCourante_ = echelle;
    mAngle = angle;

//     vecteurEntre.normaliser();
//     mAngle = (360.0f+(utilitaire::RAD_TO_DEG(acos(vecteurEntre[VX]))*( (vecteurEntre[VY] < 0)?-1:1) ) );
//     mAngle /= 360.0f;
//     mAngle -= (int)(mAngle);
//     mAngle *= 360.0f;



    // necessaire pour s'assurer de l'integrite des proprietes
    // physiques et d'affichage
    updateMatrice();
    updatePhysicBody();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////