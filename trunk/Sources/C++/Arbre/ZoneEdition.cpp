///////////////////////////////////////////////////////////////////////////////
/// @file ZoneEdition.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "ZoneEdition.h"

#if BOX2D_INTEGRATED
#include <Box2D/Box2D.h>
#include "Utilitaire.h"
#endif

#if WIN32
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include "FacadeModele.h"
#elif __APPLE__
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "RazerGameUtilities.h"
#endif

#include "XMLUtils.h"
#include "Terrain.h"
#include "Enum_Declarations.h"

const float ZoneEdition::DEFAUT_LIMITE_INT_X = 75;
const float ZoneEdition::DEFAUT_LIMITE_INT_Y = 50;
const float ZoneEdition::DEFAUT_LIMITE_EXT_Y = 150; 
const float ZoneEdition::DEFAUT_LIMITE_EXT_X = 200;

////////////////////////////////////////////////////////////////////////
///
/// @fn  ZoneEdition::ZoneEdition()
///
/// Constructeur par defaut
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ZoneEdition::ZoneEdition(Terrain* owner):
    mOwner(owner)
#if BOX2D_INTEGRATED
    , mPhysicsBody(NULL)
#endif
{
	mLimitIntX = DEFAUT_LIMITE_INT_X;
	mLimitIntY  = DEFAUT_LIMITE_INT_Y ;
	mLimitExtY  = DEFAUT_LIMITE_EXT_Y ; 
	mLimitExtX = DEFAUT_LIMITE_EXT_X;
    rebuild();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  ZoneEdition::~ZoneEdition()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ZoneEdition::~ZoneEdition()
{
#if BOX2D_INTEGRATED  
    clearPhysicsBody();
#endif //BOX2D_INTEGRATED
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::modifierLimitesExt( const float* boiteEnglobantTable, const float& longueur,const float& largeur )
///
/// Permet de modifier les limites de la zone d'edition de facon relative à une table
///
/// @param[in] const float * boiteEnglobantTable : Tableau a 2 dimension contenant la longueur et hauteur max d'une table
///													boiteEnglobantTable[0] : hauteur
///													boiteEnglobantTable[1] : largeur
/// @param[in] const float & longueur : nouvelle longueur à assigner à la zone d'édition
/// @param[in] const float & largeur : nouvelle largeur à assigner à la zone d'édition
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::modifierLimitesExt( const float* boiteEnglobantTable, const float& longueur,const float& largeur )
{
	// Si la longueur a assigner est plus petite que la longueur maximale de la table, on cap a la longueur de la table
	if(longueur< boiteEnglobantTable[1] )
	{
		mLimitExtX=boiteEnglobantTable[1];

	}
	// si plus grand ou egale
	else
	{
		mLimitExtX=longueur;
	}

	// Si la largeur a assigner est plus petite que la largeur maximale de la table, on cap a la largeur de la table
	if(largeur< boiteEnglobantTable[0] )
	{
		mLimitExtY=boiteEnglobantTable[0];
	}
	// si plus grand ou egale
	else
	{
		mLimitExtY=largeur;
	}
    rebuild();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* ZoneEdition::creerNoeudXML()
///
/// Creation du noeud XML de la zone d'édition
///
///
/// @return XmlElement* : le noeud XML avec la propriété de la zone
///
////////////////////////////////////////////////////////////////////////
XmlElement* ZoneEdition::creerNoeudXML()
{
	XmlElement* element = XMLUtils::createNode("ZoneEdition");

    XMLUtils::writeAttribute(element,"ExtLar",mLimitExtY);
    XMLUtils::writeAttribute(element,"ExtLon",mLimitExtX);
    XMLUtils::writeAttribute(element,"IntLar",mLimitIntY);
    XMLUtils::writeAttribute(element,"IntLon",mLimitIntX);

	return element;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ZoneEdition::initialisationXML( XmlElement* element )
///
/// Initialistion de la zone d'édition à partir d'un noeud XML
///
/// @param[in] XmlElement * : noeud contenant les informations de la zone d'édition
///
/// @return bool vrai si l'initialisation c'est bien passé
///
////////////////////////////////////////////////////////////////////////
bool ZoneEdition::initialisationXML( const XmlElement* element )
{
	const XmlElement* zone = XMLUtils::FirstChildElement(element,"ZoneEdition");

	if(!zone)
		return false;

    if(!XMLUtils::readAttribute(zone,"ExtLar",mLimitExtY) ) return false;
    if(!XMLUtils::readAttribute(zone,"ExtLon",mLimitExtX)) return false;
    if(!XMLUtils::readAttribute(zone,"IntLar",mLimitIntY) ) return false;
    if(!XMLUtils::readAttribute(zone,"IntLon",mLimitIntX)) return false;

    rebuild();
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::reinitialiser()
///
/// Remet la zone d'edition a son etat par defaut
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::reinitialiser()
{
	mLimitIntX = DEFAUT_LIMITE_INT_X;
	mLimitIntY  = DEFAUT_LIMITE_INT_Y ;
	mLimitExtY  = DEFAUT_LIMITE_EXT_Y ; 
	mLimitExtX = DEFAUT_LIMITE_EXT_X;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::afficher()
///
/// Effectue l'affichage de la zone d'edition
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::afficher()
{
#if WIN32
    // États de la lumière 
    GLboolean lighting_state;
    // Désactiver l'éclairage
    glGetBooleanv(GL_LIGHTING, &lighting_state);
    glDisable(GL_LIGHTING);
    FacadeModele::getInstance()->DeActivateShaders();
    glEnableClientState(GL_VERTEX_ARRAY);
#endif

	// Dessin de la zone d'édition
    glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(0,0.749f,1,1);
    GLfloat vertices[12] =
    {
        mLimitExtX, mLimitExtY,-3,
        -mLimitExtX, mLimitExtY, -3,
        -mLimitExtX, -mLimitExtY, -3,
        mLimitExtX, -mLimitExtY, -3
    };
    glVertexPointer (3, GL_FLOAT , 0, vertices); 
    glDrawArrays (GL_TRIANGLE_FAN, 0, 4);

	glPopAttrib();
    glPopMatrix();
#if WIN32
    glDisableClientState(GL_VERTEX_ARRAY);
    FacadeModele::getInstance()->ActivateShaders();

    // Réactiver l'éclairage et (s'il y a lieu)
    if (lighting_state == GL_TRUE) {
        glEnable(GL_LIGHTING);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ZoneEdition::equals( ZoneEdition * zone )
///
/// /*Description*/
///
/// @param[in] ZoneEdition * zone
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool ZoneEdition::equals( ZoneEdition * zone )
{
    if(!zone)
    {
        return false;
    }
    if(zone == this)
    {
        return true;
    }

    return
    mLimitIntX == zone->mLimitIntX &&
    mLimitIntY  == zone->mLimitIntY  &&
    mLimitExtY  == zone->mLimitExtY  &&
    mLimitExtX == zone->mLimitExtX ;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::rebuild()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::rebuild()
{
    mAABBExt = BoundingBox::BuildAABB(Vecteur3(),Vecteur3(mLimitExtX,mLimitExtY));
    mAABBInt = BoundingBox::BuildAABB(Vecteur3(),Vecteur3(mLimitIntX,mLimitIntY));
    
#if BOX2D_INTEGRATED  
    clearPhysicsBody();
    if(mOwner)
    {
        auto world = mOwner->GetWorld();
        if(world)
        {
            b2BodyDef def;
            def.type = b2_staticBody;
            
            mPhysicsBody = world->CreateBody(&def);

            b2FixtureDef fixtureDef;
            fixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
            fixtureDef.filter.maskBits = 0xFFFF;

            /// default filter for edition, collides with everything

            float extY = mLimitExtY * utilitaire::ratioWorldToBox2D;
            float extX = mLimitExtX * utilitaire::ratioWorldToBox2D;

            b2PolygonShape box;
            fixtureDef.shape = &box;

            box.SetAsBox(extX,extY*3,b2Vec2(extX*2,0),0);
            mPhysicsBody->CreateFixture(&fixtureDef);
            box.SetAsBox(extX,extY*3,b2Vec2(-extX*2,0),0);
            mPhysicsBody->CreateFixture(&fixtureDef);
            box.SetAsBox(extX*3,extY,b2Vec2(0,extY*2),0);
            mPhysicsBody->CreateFixture(&fixtureDef);
            box.SetAsBox(extX*3,extY,b2Vec2(0,-extY*2),0);
            mPhysicsBody->CreateFixture(&fixtureDef);

            /*b2EdgeShape shape;
            fixtureDef.shape = &shape;

            shape.Set(b2Vec2(-extX,-extY),b2Vec2(-extX,extY));
            mPhysicsBody->CreateFixture(&fixtureDef);

            shape.Set(b2Vec2(-extX,extY),b2Vec2(extX,extY));
            mPhysicsBody->CreateFixture(&fixtureDef);

            shape.Set(b2Vec2(extX,extY),b2Vec2(extX,-extY));
            mPhysicsBody->CreateFixture(&fixtureDef);

            shape.Set(b2Vec2(extX,-extY),b2Vec2(-extX,-extY));
            mPhysicsBody->CreateFixture(&fixtureDef);*/
        }
    }
#endif //BOX2D_INTEGRATED
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::clearPhysicsBody()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
#if BOX2D_INTEGRATED  
void ZoneEdition::clearPhysicsBody()
{
    if(mPhysicsBody && mOwner)
    {
        auto world = mOwner->GetWorld();
        if(world)
        {
            world->DestroyBody(mPhysicsBody);
        }
    }
    mPhysicsBody = NULL;
}



#endif //BOX2D_INTEGRATED

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


