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


#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

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
/// et donner des valeurs par défaut aux variables membres.
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
/// Ce destructeur désallouee la liste d'affichage du muret.
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

        float halfLength = echelleCourante_[VX]*DEFAULT_SIZE[VX]/2.f*utilitaire::ratioWorldToBox2D;
        float halfHeight = echelleCourante_[VY]*DEFAULT_SIZE[VY]/2.f*utilitaire::ratioWorldToBox2D;

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.position.Set(0, 0); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = world->CreateBody(&myBodyDef);
        b2PolygonShape shape;
        const Vecteur3& pos = getPosition();
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
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallAbstract::afficherConcret() const
{
#if WIN32
	glColor3f(0.76f, 0.64f, 0.31f);
#endif
	// Appel à la version de la classe de base pour l'affichage des enfants.
	Super::afficherConcret();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallAbstract::assignerAttributVisiteurCollision( VisiteurCollision* v )
///
/// Permet d'assigner les attribut nécessaire à la collision.
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
/// Initialisation du NodeWallAbstract à partir d'un element XML
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
    const Vecteur3& corner1 = obtenirCoin1(), corner2 = obtenirCoin2();
    Vecteur3 vecteurEntre(corner1-corner2);

    // just to be sure
    vecteurEntre[VZ] = 0;

    if(vecteurEntre.estNul())
    {
        mAngle = 0;
        echelleCourante_[VX] = 0;
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
        mPosition = corner2+(vecteurEntre/2.0f);
        echelleCourante_ = echelle;
        mAngle = angle;
    }
    // necessaire pour s'assurer de l'integrite des proprietes
    // physiques et d'affichage
    updateMatrice();
    updatePhysicBody();
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
    GLfloat lineWidth;
    glGetFloatv(GL_LINE_WIDTH,&lineWidth);
    glLineWidth(echelleCourante_[VY]);
    glColor4f(1,1,0,1);

    auto c1 = obtenirCoin1(), c2 = obtenirCoin2();
    c1 -= mPosition;
    c2 -= mPosition;
    
    // Find the 4 corners of the wall and draw a triangle fan with it
    float deltaX = c2[VX] - c1[VX];
    float deltaY = c2[VY] - c1[VY];
    Vecteur2 vecteurDir = Vecteur2(deltaX,deltaY);
    vecteurDir.normaliser();
    float distFromMiddlePoint = echelleCourante_[VY] * DEFAULT_SIZE[VY]/2;
    Vecteur2 cornerUpLeft = Vecteur2(c1[VX],c1[VY]) + (distFromMiddlePoint * vecteurDir.tournerMoinsPiSur2());
    Vecteur2 cornerBotLeft = Vecteur2(c1[VX],c1[VY]) + (distFromMiddlePoint * vecteurDir.tournerPiSur2());
    Vecteur2 cornerUpRight = Vecteur2(c2[VX],c2[VY]) + (distFromMiddlePoint * vecteurDir.tournerPiSur2());
    Vecteur2 cornerBotRight = Vecteur2(c2[VX],c2[VY]) + (distFromMiddlePoint * vecteurDir.tournerMoinsPiSur2());
    GLfloat vertices[8] = {cornerUpLeft[VX],cornerUpLeft[VY],cornerBotLeft[VX],cornerBotLeft[VY],cornerUpRight[VX],cornerUpRight[VY],cornerBotRight[VX],cornerBotRight[VY]};
    glVertexPointer (2, GL_FLOAT , 0, vertices); 
    glDrawArrays (GL_TRIANGLE_FAN, 0, 4);

    glLineWidth(lineWidth);
}


#ifdef MIKE_DEBUG
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////