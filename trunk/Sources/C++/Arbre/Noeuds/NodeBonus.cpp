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
#include "BonusModifierAbstract.h"
#include "BonusModifierFactory.h"

const Vecteur3 DEFAULT_SIZE = Vecteur3(15, 15, 1);
const float DEFAULT_RADIUS = 8;

const int nbFaces = 12;
const int nbVertexPerFace = 3;
const int vertexSize = 3;
const int colorSize = 4;
const int vertexArraySize = nbFaces*nbVertexPerFace*vertexSize;
const int colorArraySize = nbFaces*nbVertexPerFace*colorSize;

bool cubeModelArraysInit = false;
GLfloat vertexArray[vertexArraySize];
GLfloat colorArray[colorArraySize];

#if WIN32
#include "Modele3D.h"
#include "GestionnaireModeles.h"
#include "FacadeModele.h"
#include "../Reseau/RelayeurMessage.h"
#include "../Reseau/Paquets/PaquetBonus.h"

CreateListDelegateImplementation(EmptyBonus)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = DEFAULT_SIZE / delta;

    pModel->assignerFacteurAgrandissement(delta);
    GLuint liste = glGenLists(1);
    glNewList(liste, GL_COMPILE);
    glTranslatef(0, 0, -DEFAULT_SIZE[VZ]);
    pModel->dessiner(true);	
    glEndList();
    return liste;
    return GestionnaireModeles::CreerListe(pModel);
}

CreateListDelegateImplementation(Bonus)
{
    const float height = 15;
    GLuint liste = glGenLists(1);

    glNewList(liste, GL_COMPILE);
    if(!cubeModelArraysInit)
    {
        NodeBonus::getCubeColorVertexArrays(vertexArray,colorArray);
        cubeModelArraysInit = true;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, colorArray);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, nbFaces*nbVertexPerFace);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glEndList();
    return liste;

    //return RazerGameUtilities::CreateListSphereDefault(pModel,DEFAULT_RADIUS);
}
#endif



/// vertexArray size must be 81, colorArray size must be 108
void NodeBonus::getCubeColorVertexArrays(float* vertexArray, float* colorArray)
{
    /// cube infos
    float vertices[8][vertexSize]=
    {
        {1,0,0},
        {1,0,1},
        {0,0,1},
        {0,0,0},
        {1,1,0},
        {1,1,1},
        {0,1,1},
        {0,1,0},
    };
    /// index to vertices for faces
    int faces[nbFaces][nbVertexPerFace] =
    {
        {5,1,4},
        {5,4,8},
        {3,7,8},
        {3,8,4},
        {2,6,3},
        {6,7,3},
        {1,5,2},
        {5,6,2},
        {5,8,6},
        {8,7,6},
        {1,2,3},
        {1,3,4},
    };

    int countVertex = 0;
    int countColor = 0;
    for(int iFace=0; iFace<nbFaces; ++iFace)
    {
        for(int iVertex=0; iVertex<nbVertexPerFace; ++iVertex)
        {
            for(int iAxe=0; iAxe<vertexSize;++iAxe)
            {
                vertexArray[countVertex++] = vertices[faces[iFace][iVertex]-1][iAxe]-0.5f;
                colorArray[countColor++] = vertices[faces[iFace][iVertex]-1][iAxe];
            }
            /// alpha value
            colorArray[countColor++] = 0.2f;
        }
    }
    checkf(countVertex == vertexArraySize);
    checkf(countColor == colorArraySize);
}

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
   : Super(RAZER_KEY_BONUS,typeNoeud),mHeightAngle(0),mCounting(0)
{
    // temp workaround, l'édition va le considérer comme un cercle pour un moment
    setDefaultRadius(DEFAULT_RADIUS);

    if(!cubeModelArraysInit)
    {
        getCubeColorVertexArrays(vertexArray,colorArray);
        cubeModelArraysInit = true;
    }

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
    if(containsModifiers())
    {
        // Renders all the modifiers present on the node
        for(auto it = mModifiers.begin(); it != mModifiers.end(); ++it)
        {
            (*it)->render();
        }

        /// only render modifiers
        return;
    }
#if WIN32
    glDisable(GL_LIGHTING);
    FacadeModele::getInstance()->DeActivateShaders();
#endif
    // static to make all bonus hover at the same height
    const float zTranslated = getRadius()*1.5f+sin(mHeightAngle)*3.f;
    glPushMatrix();
    glTranslatef( 0,0,zTranslated);
    
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    Super::renderReal();
    if(!blendEnabled)
    {
        glDisable(GL_BLEND);
    }
    glPopMatrix();
#if WIN32
    
    glEnable(GL_LIGHTING);
    FacadeModele::getInstance()->ActivateShaders();

    GLint renderMode;
    glGetIntegerv(GL_RENDER_MODE,&renderMode);
    if(renderMode == GL_SELECT)
    {
        // dont draw emptyBonus model when selecting
        return;
    }

    /// retrait de la platforme pour le moment, bug la selection
    GLuint liste;	
    GestionnaireModeles::obtenirInstance()->obtenirListe(RAZER_KEY_EMPTY_BONUS,liste);
    // Si aucune liste n'est trouvé, on sort de la fonction.
    if(liste==NULL)
        return;

    glPushMatrix();
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Descend la platforme pour ne voir que la surface
    glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
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
        setAngle((float)((int)(mAngle+dt*500.0f)%360));
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
    if(!isActive() && !containsModifiers())
    {
        float timeLeft = 2;
        Partie* game = NULL;
        auto field = getField();
        if(field)
        {
            game = field->GetGame();
        }
        if(!mCounting)
        {
            mBeginTime = clock()/CLOCKS_PER_SEC;
            if(game)
            {
                mBeginTime = game->obtenirGameTime()->Elapsed_Time_sec();
            }
            mCounting = true;
        }
        // game tick
        if(game)
        {
            timeLeft = mSpawnTimeDelaiTotal - (game->obtenirGameTime()->Elapsed_Time_sec() - mBeginTime);
        }

        if(timeLeft < 0)
        {
            mCounting = false;
            mBonusType = (BonusType)(rand()%NB_BONUS_TYPE);
            ResetTimeLeft();
            setVisible(true);
            // activate collision on strat creation
            activate(true);

#if WIN32
            Partie* wGame = getField()->GetGame();
            if(wGame && wGame->isNetworkServerGame())
            {
                PaquetBonus* wPaquet = (PaquetBonus*) GestionnaireReseau::obtenirInstance()->creerPaquet(BONUS);
                wPaquet->setGameId(wGame->getUniqueGameId());
                wPaquet->setBonusType((BonusType)(mBonusType));
                wPaquet->setBonusAction(BONUS_ACTION_SPAN);
                wPaquet->setBonusPosition(getPosition());
                RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquet->getGameId(), wPaquet, TCP);
            }
#endif
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::displayBonus( class NoeudRondelle* )
///
/// Called from paquet to display a bonus.
///
/// @param[in] class NoeudRondelle *
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::displayBonus(BonusType pBonusType)
{
    mBonusType = pBonusType;
    ResetTimeLeft();
    setVisible(true);
    activate(true);
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
        //int b = rand()%NB_BONUS_TYPE;
#if MIKE_DEBUG_
        //b = BONUS_TYPE_GO_THROUGH_WALL; // testing value
#endif
        auto factory = FactoryBonusModifier::getFactory(mBonusType);
        if(factory)
        {
            auto bonus = factory->createBonus(this);
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
    }
    // deactivate the bonus, the timer will resume
    setVisible(false);
    activate(false);
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
    ResetTimeLeft();
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


        b2BodyDef myBodyDef;
        myBodyDef.type = IsInGame() ? b2_staticBody : b2_dynamicBody; //this will be a dynamic body
        myBodyDef.angle = 0; //set the starting angle
        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position

        mPhysicBody = world->CreateBody(&myBodyDef);
        b2CircleShape shape;
        auto radius = getRadius();
        shape.m_p.Set(0, 0); //position, relative to body position
        shape.m_radius = radius*utilitaire::ratioWorldToBox2D; //radius

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shape above
        myFixtureDef.density = 1;

        // Il s'agit ici d'un bonus qui peut entré en collision avec une rondelle
        myFixtureDef.filter.categoryBits = CATEGORY_BONUS;
        if(IsInGame())
        {
            myFixtureDef.filter.maskBits = CATEGORY_PUCK;

            // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
            myFixtureDef.isSensor = true;
        }
        else
        {
            myFixtureDef.filter.maskBits = 0xFFFF;
            myFixtureDef.filter.groupIndex = 1;
        }

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;
        mPhysicBody->SetActive(!IsInGame());
    }
#endif
}

void NodeBonus::renderOpenGLES() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, colorArray);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, nbFaces*nbVertexPerFace);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
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
    auto field = getField();
    if(field)
    {
        int min = (int)(field->getBonusesMinTimeSpawn()*100.f);
        int max = (int)(field->getBonusesMaxTimeSpawn()*100.f);
        if(min == max)
        {
            mSpawnTimeDelaiTotal = (float)min/100.f;
        }
        else
        {
            mSpawnTimeDelaiTotal = (float)(rand()%(max-min)+min)/100.f;
        }
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::acceptVisitor( VisiteurNoeud& v )
///
/// /*Description*/
///
/// @param[in] VisiteurNoeud & v
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNodeBonus(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::updateMatrice()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::updateMatrice()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRotatef(mAngle, 0.0, 0.0, 1.0);
    static float angleX = 45;
    static float angleY = 35;
    glRotatef(angleX,1,0,0);
    glRotatef(angleY,0,1,0);
    glScalef(mScale[VX], mScale[VY], mScale[VZ]);
    glScalef(DEFAULT_RADIUS*1.5f,DEFAULT_RADIUS*1.5f,DEFAULT_RADIUS*1.5f);

    glGetFloatv(GL_MODELVIEW_MATRIX, mTransformationMatrix); // Savegarde de la matrice courante dans le noeud

    glPopMatrix();
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
