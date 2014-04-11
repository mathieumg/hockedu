#pragma once
#include "Singleton.h"
#include "Enum_Declarations.h"

class IRenderComponent;
class RenderComponentManager : public Singleton<RenderComponentManager>
{
    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR( RenderComponentManager );
    RenderComponentManager();
public:
    IRenderComponent* createRenderComponent( class NoeudAbstrait* node, const RazerKey key ) const;


};