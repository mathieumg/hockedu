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

#include "XMLUtils.h"



const Vecteur3 NodeWallAbstract::DEFAULT_SIZE = Vecteur3(1, 5, 15);

#if WIN32
#include "Modele3D.h"
#include "GestionnaireModeles.h"
CreateListDelegateImplementation(Wall)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = NodeWallAbstract::DEFAULT_SIZE / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}
#endif


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
NodeWallAbstract::NodeWallAbstract( const RazerKey typeNoeud )
	: Super(typeNoeud)
{
	mReboundRatio = 0.75f;
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

    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        float halfLength = mScale[VX]*DEFAULT_SIZE[VX]/2.f*utilitaire::ratioWorldToBox2D;
        float halfHeight = mScale[VY]*DEFAULT_SIZE[VY]/2.f*utilitaire::ratioWorldToBox2D;

        b2BodyDef myBodyDef;

        /// Lines cant be dynamic, so if either size is null, force it to static
        if(IsInGame() || halfLength == 0 || halfHeight == 0)
        {
            myBodyDef.type =  b2_staticBody; //this will be a static body
        }
        else
        {
            myBodyDef.type =  b2_dynamicBody; //this will be a dynamic body
        }
        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
        myBodyDef.angle = utilitaire::DEG_TO_RAD(mAngle); //set the starting angle

        mPhysicBody = world->CreateBody(&myBodyDef);
        b2PolygonShape shape;
        shape.SetAsBox(halfLength,halfHeight,b2Vec2(0,0),0);

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shape above
        myFixtureDef.density = 1;
        myFixtureDef.restitution = getReboundRatio();
        RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_WALL,IsInGame());

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;
    }
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::renderReal() const
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::renderReal() const
{
#if WIN32
	glColor3f(0.76f, 0.64f, 0.31f);
#endif
	// Appel � la version de la classe de base pour l'affichage des enfants.
	Super::renderReal();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::setCollisionVisitorAttributes( VisiteurCollision* v )
///
/// Permet d'assigner les attribut n�cessaire � la collision.
///
/// @param[in] VisiteurCollision * v : le visiteur de collision.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::setCollisionVisitorAttributes( VisiteurCollision* v )
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
/// @fn XmlElement* NodeWallAbstract::createXmlNode()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NodeWallAbstract::createXmlNode()
{
	XmlElement* elementNoeud = Super::createXmlNode();
	// Ajouter la position des coins des murets
	XMLUtils::writeAttribute(elementNoeud,"coefRebond",mReboundRatio);
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallAbstract::initFromXml( const XmlElement* element )
///
/// Initialisation du NodeWallAbstract � partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallAbstract::initFromXml( const XmlElement* element )
{
	if(!Super::initFromXml(element))
		return false;
	float floatElem;
	if( !XMLUtils::readAttribute(element,"coefRebond",floatElem) )
		return false;
	mReboundRatio = floatElem;

	return true;
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
    const Vecteur3& corner1 = obtenirCoin1(), corner2 = obtenirCoin2();
    Vecteur3 vecteurEntre(corner1-corner2);

    // just to be sure
    vecteurEntre[VZ] = 0;

    if(vecteurEntre.estNul())
    {
        mAngle = 0;
        mScale[VX] = 0;
        mPosition = corner1;
    }
    else
    {
        // arctan(y/x) = angle
        float angle = utilitaire::RAD_TO_DEG(atan2(vecteurEntre[VY], vecteurEntre[VX]));
        // L'angle doit rester toujours positif pour garder la coherence dans tous les modifications
        if(angle<0)
        {
            angle += 360.f;
        }

        // pour conserver l'echelle en Y et Z
        Vecteur3 echelle;
        getScale(echelle);
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
        mPosition = corner2+(vecteurEntre/2.0f);
        mScale = echelle;
        mAngle = angle;
    }
    // necessaire pour s'assurer de l'integrite des proprietes
    // physiques et d'affichage
    updateMatrice();
    if(!isWorldLocked())
    {
        updatePhysicBody();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::renderOpenGLES()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::renderOpenGLES() const
{
    glColor4f(1,1,0,1);

    {
        /// half width and hlaf height
        const float hw = DEFAULT_SIZE[VX]/2.f;
        const float hh = DEFAULT_SIZE[VY]/2.f;
        GLfloat vertices[8] = {-hw,-hh,hw,-hh,hw,hh,-hw,hh};        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer (2, GL_FLOAT , 0, vertices);
        glDrawArrays (GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        return;
    }
    
    
    
    {
    auto c1 = obtenirCoin1(), c2 = obtenirCoin2();
    c1 -= mPosition;
    c2 -= mPosition;
    
    // Find the 4 corners of the wall and draw a triangle fan with it
    float deltaX = c2[VX] - c1[VX];
    float deltaY = c2[VY] - c1[VY];
    Vecteur2 vecteurDir = Vecteur2(deltaX,deltaY);
    vecteurDir.normaliser();
    float distFromMiddlePoint = mScale[VY] * DEFAULT_SIZE[VY]/2;
    Vecteur2 cornerUpLeft = Vecteur2(c1[VX],c1[VY]) + (distFromMiddlePoint * vecteurDir.tournerMoinsPiSur2());
    Vecteur2 cornerBotLeft = Vecteur2(c1[VX],c1[VY]) + (distFromMiddlePoint * vecteurDir.tournerPiSur2());
    Vecteur2 cornerUpRight = Vecteur2(c2[VX],c2[VY]) + (distFromMiddlePoint * vecteurDir.tournerPiSur2());
    Vecteur2 cornerBotRight = Vecteur2(c2[VX],c2[VY]) + (distFromMiddlePoint * vecteurDir.tournerMoinsPiSur2());
    GLfloat vertices[8] = {cornerUpLeft[VX],cornerUpLeft[VY],cornerBotLeft[VX],cornerBotLeft[VY],cornerUpRight[VX],cornerUpRight[VY],cornerBotRight[VX],cornerBotRight[VY]};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer (2, GL_FLOAT , 0, vertices); 
    glDrawArrays (GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::setAngle( float angle )
///
/// Permet de modifier l'angle du muret et garde les coins a jour
///
/// @param[in] float angle : nouvel angle du muret
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::setAngle( float angle )
{
    Super::setAngle(angle);
    updateCornerPosition();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::setScale( const Vecteur3& echelleCourante )
///
/// Mutateur des facteurs d'echelle et conserve les coins a jour
///
/// @param[in] const Vecteur3 & echelleCourante
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::setScale( const Vecteur3& echelleCourante )
{
    Super::setScale(echelleCourante);
    updateCornerPosition();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::setPosition( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::setPosition( const Vecteur3& positionRelative )
{
    Super::setPosition(positionRelative);
    updateCornerPosition();
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////