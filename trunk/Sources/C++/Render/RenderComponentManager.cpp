#include "RenderComponentManager.h"
#include "RenderOpenGL2.h"
#include "RenderOpenGL2Bonus.h"


SINGLETON_DECLARATION_CPP( RenderComponentManager );

typedef IRenderComponent* (*RenderComponentFactory)();

RenderComponentFactory factories[NB_RAZER_KEYS];
RenderComponentManager::RenderComponentManager()
{
    for (int i = 0; i < NB_RAZER_KEYS ; ++i)
    {
        factories[i] = 0;
    }
#define createFactory(type,key) factories[key] = []()->IRenderComponent*{return new type;};
    createFactory( RenderOpenGL2, RAZER_KEY_PORTAL );
    createFactory( RenderOpenGL2, RAZER_KEY_TABLE );
    createFactory( RenderOpenGL2, RAZER_KEY_TABLE_CONTROL_POINT );
    createFactory( RenderOpenGL2, RAZER_KEY_CONTROL_POINT );
    createFactory( RenderOpenGL2, RAZER_KEY_WALL );
    createFactory( RenderOpenGL2, RAZER_KEY_PUCK );
    createFactory( RenderOpenGL2, RAZER_KEY_MALLET );
    createFactory( RenderOpenGL2Bonus, RAZER_KEY_BONUS );
    createFactory( RenderOpenGL2, RAZER_KEY_BOOST );
    createFactory( RenderOpenGL2, RAZER_KEY_GOAL );
}




IRenderComponent* RenderComponentManager::createRenderComponent( NoeudAbstrait* node, const RazerKey key ) const
{
    IRenderComponent* component = NULL;
    if( factories[key] )
    {
        component = factories[key]();
    }

    if( component )
    {
        component->setNode( node );
    }
    return component;
}



