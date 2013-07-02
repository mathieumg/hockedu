#include "ManagerAnimations.h"
#include "AnimationBase.h"
#include <vector>


SINGLETON_DECLARATION_CPP(ManagerAnimations);

void ManagerAnimations::Tick( float deltaTime )
{
    for (auto it = mAnimations.cbegin(); it != mAnimations.cend() /* not hoisted */; /* no increment */)
    {
        auto anim = it->second;
        if (anim->IsCanceled() || anim->ExecuteBase(deltaTime))
        {
            AnimationInterface* anim = it->second;
            mAnimations.erase(it++);
            delete anim;
        }
        else
        {
            ++it;
        }
    }
}

void ManagerAnimations::AddAnimation( unsigned int id, AnimationInterface* anim )
{
    mAnimations[id] = anim;
}

void ManagerAnimations::DeleteAnimation( unsigned int id )
{
    auto it = mAnimations.find(id);
    if(it != mAnimations.end())
    {
        AnimationInterface* anim = it->second;
        mAnimations.erase(it);
        delete anim;
    }
}

AnimationInterface* ManagerAnimations::GetAnimation( unsigned int id )
{
    auto it = mAnimations.find(id);
    if(it != mAnimations.end())
    {
        return it->second;
    }
    return nullptr;
}

ManagerAnimations::ManagerAnimations():
    mAnimID(0)
{

}
