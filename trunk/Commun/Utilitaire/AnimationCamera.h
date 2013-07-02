#pragma once
#include "Vecteur.h"
#include "AnimationFrameDefinitions.h"
#include "VariableTemplate.h"
#include "AnimationBase.h"

namespace vue
{
    class Camera;
}

using namespace AnimationFrame;

typedef ctmap<Time,Position,Aim,UpVector> CameraAnimationFrame;

class AnimationCamera : public AnimationBase<CameraAnimationFrame>
{
protected:
    AnimationCamera();
public:
    // only the manager can create new animations
    friend class ManagerAnimations;

    void AddFrame(float time, const Vecteur3& pos, const Vecteur3& aim, const Vecteur3& upVector)
    {
        CameraAnimationFrame frame;
        frame[Time()]     = time;
        frame[Position()] = pos;
        frame[Aim()]      = aim;
        frame[UpVector()] = upVector;
        mFrames.push_back(frame);
    }

    /// Execute the animation and returns true if the animation is over (can be deleted)
    virtual bool Execute();
    void SetCamera(vue::Camera* val);

private:
    vue::Camera* mCamera;
    
};