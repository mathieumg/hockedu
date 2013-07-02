#pragma once

#include "Singleton.h"
#include <map>

class AnimationInterface;

class ManagerAnimations : public Singleton<ManagerAnimations>
{
    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(ManagerAnimations);
    ManagerAnimations();
public:
    void Tick(float deltaTime);
    template<typename T>

    /// Creates an animation and returns it to assign its frames
    /// do not keep references to the animations, only the id
    std::pair<unsigned int,T*> CreateAnimation()
    {
        ++mAnimID; 
        AnimationInterface* anim = new T();
        anim->mId = mAnimID;
        AddAnimation(mAnimID,anim);
        return std::pair<unsigned int,T*>(mAnimID,(T*)anim);
    }
    void DeleteAnimation(unsigned int id);

    /// do not keep references to the animations, only the id
    AnimationInterface* GetAnimation(unsigned int id);
private:
    std::map<int,AnimationInterface*> mAnimations;
    void AddAnimation(unsigned int id, AnimationInterface* anim);

    unsigned int mAnimID;
};

