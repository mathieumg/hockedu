///////////////////////////////////////////////////////////////////////////
/// @file NodeBonus.cpp
/// @author Michael Ferris
/// @date 2012-04-12
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#include "NodeBonus.h"
#include "Terrain.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "VisiteurNoeud.h"
#include "BonusStratAbstract.h"

const Vecteur3 DEFAULT_SIZE = Vecteur3(15, 15, 1);
const float DEFAULT_RADIUS = 10;

#if WIN32
#include "Modele3D.h"
#include "GestionnaireModeles.h"
CreateListDelegateImplementation(EmptyBonus)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = DEFAULT_SIZE / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}

CreateListDelegateImplementation(Bonus)
{
    const float height = 15;
    GLuint liste = glGenLists(1);


    glNewList(liste, GL_COMPILE);

    glRotatef(45,1,0,0);
    glRotatef(35,0,1,0);
    glScalef(DEFAULT_RADIUS,DEFAULT_RADIUS,DEFAULT_RADIUS);
    glBegin(GL_QUADS);

    GLfloat vertex[3];
    GLfloat alternance[2][4][2] =
    {{
        {0,0},
        {0,1},
        {1,1},
        {1,0},
    },
    {
        {0,0},
        {1,0},
        {1,1},
        {0,1},
    }};
    for(int axe=0; axe<3; ++axe)
    {
        for(int fixedValue=0; fixedValue<2; ++fixedValue)
        {
            vertex[axe] = fixedValue-0.5f;
            for(int it=0; it<4; ++it)
            {
                for(int pos=1; pos<3; ++pos)
                {
                    vertex[(axe+pos)%3] = alternance[fixedValue][it][pos-1]-0.5f;
                }
                glColor3f(vertex[0],vertex[1],vertex[2]);
                glVertex3f(vertex[0],vertex[1],vertex[2]);
            }
        }
    }
    glEnd();


    glEndList();
    return liste;

    return RazerGameUtilities::CreateListSphereDefault(pModel,DEFAULT_RADIUS);
}
#endif
#include "BonusModifierAbstract.h"
#include "BonusModifierFactory.h"
#include "FacadeModele.h"
#include "BonusStratGoThroughWall.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeBonus::NodeBonus(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeBonus::NodeBonus(const std::string& typeNoeud)
   : Super(typeNoeud),mMinTimeSpawn(10.5f), mMaxTimeSpawn(30.f),mHeightAngle(0)
{
    // temp workaround, l'édition va le considérer comme un cercle pour un moment
    setDefaultRadius(DEFAULT_SIZE[VX]);


    forceFullUpdate();
    ResetTimeLeft();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeBonus::~NodeBonus()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeBonus::~NodeBonus()
{
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::renderReal(  )
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::renderReal() const
{
#if WIN32
    glDisable(GL_LIGHTING);
    FacadeModele::getInstance()->DeActivateShaders();
	// Appel à la version de la classe de base pour l'affichage des enfants.

    // static to make all bonus hover at the same height
    const float zTranslated = DEFAULT_RADIUS*1.2f+sin(mHeightAngle)*3.f;
    glPushMatrix();
    glTranslatef( 0,0,zTranslated);
#endif

    Super::renderReal();
#if WIN32
    glPopMatrix();
    glEnable(GL_LIGHTING);
    FacadeModele::getInstance()->ActivateShaders();

    GLuint liste;	
    GestionnaireModeles::obtenirInstance()->obtenirListe(RazerGameUtilities::NAME_EMPTY_BONUS,liste);
    // Si aucune liste n'est trouvé, on sort de la fonction.
    if(liste==NULL)
        return;

    glPushMatrix();
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // permet de s'assurer de bien le positionner si le bonus n'est pas affiché
    glTranslatef(mPosition[0], mPosition[1], mPosition[2]-DEFAULT_SIZE[VZ]);
    glCallList(liste);

    glPopAttrib();
    glPopMatrix();
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::tick( const float& dt )
///
/// Anime le noeud.
///
/// @param[in] const float & dt
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::tick( const float& dt )
{
    if(isVisible())
    {
        mAngle = (float)((int)(mAngle+dt*500.0f)%360);
        mHeightAngle += dt*3;
        updateMatrice();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::playTick( float temps )
///
/// node tick received when actually playing the game (simulation running)
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::playTick( float temps)
{
    Super::playTick(temps);
    if(!isActive())
    {
        // game tick
        mSpawnTimeLeft -= temps;
        if(mSpawnTimeLeft < 0)
        {
            ResetTimeLeft();
            setVisible(true);
            // activate collision on strat creation
            activate(true);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::ExecuteBonus( class NoeudRondelle* )
///
/// Creates and apply a bonus from the puck
///
/// @param[in] class NoeudRondelle *
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::ExecuteBonus( class NoeudRondelle* rondelle )
{
    if(isActive())
    {
        int b = rand()%NB_BONUS_TYPE;
        auto factory = FactoryBonusModifier::getFactory(BonusType(b));
        auto bonus = factory->createBonus();
        if(!bonus->Attach(rondelle))
        {
            // the modifier couldn't attach itself on the node so we delete it
            delete bonus;
        }
        else if(!bonus->Apply())
        {
            /// the bonus doesn't need more time to execute
            // so we finish it now
            bonus->Complete();
        }
    }
    // deactivate the bonus, the timer will resume
    setVisible(false);
    activate(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const std::string& NodeBonus::get3DModelKey()
///
/// Retrieves the key to find the 
///
///
/// @return const std::string&
///
////////////////////////////////////////////////////////////////////////
const std::string& NodeBonus::get3DModelKey() const
{
    return RazerGameUtilities::NAME_BONUS;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::forceFullUpdate()
///
/// Recreates everything needed for the game
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::forceFullUpdate()
{
    Super::forceFullUpdate();
    if(IsInGame())
    {
        setVisible(false);
#if BOX2D_INTEGRATED
        if(mPhysicBody)
        {
            mPhysicBody->SetActive(false);
        }
#endif
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::updatePhysicBody()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        float halfLength = mScale[VX]*DEFAULT_SIZE[VX]/2.f*utilitaire::ratioWorldToBox2D;
        float halfHeight = mScale[VY]*DEFAULT_SIZE[VY]/2.f*utilitaire::ratioWorldToBox2D;

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
        // Il s'agit ici d'un bonus qui peut entré en collision avec une rondelle
        myFixtureDef.filter.categoryBits = CATEGORY_BONUS;
        myFixtureDef.filter.maskBits = CATEGORY_PUCK;

        // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
        myFixtureDef.isSensor = true;

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        //mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
        mPhysicBody->SetActive(!IsInGame());
    }
#endif
}

void NodeBonus::renderOpenGLES() const
{
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    Super::renderOpenGLES();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::ResetTimeLeft()
///
/// creates a random value before spawning the next bonus
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::ResetTimeLeft()
{
    // precision 2 decimale
    int min = (int)(mMinTimeSpawn*100.f);
    int max = (int)(mMaxTimeSpawn*100.f);

    mSpawnTimeLeft = (rand()%(max-min)+min)/100.f;
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
