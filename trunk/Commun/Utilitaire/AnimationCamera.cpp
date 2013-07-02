#include "AnimationCamera.h"
#include "AnimationAlgo.h"
#include "Camera.h"

AnimationCamera::AnimationCamera():
    mCamera(NULL)
{
}

/// Execute the animation and returns true if the animation is over (can be deleted)
bool AnimationCamera::Execute()
{
    bool isAnimationFinished = false;
    AnimationExecuteWithTime(isAnimationFinished);

    CameraAnimationFrame result;
    switch (mAlgo)
    {
    case ANIMATION_LINEAR:
        result[Position()] = CalculateLinear<CameraAnimationFrame,Position>(this);
        result[Aim()] = CalculateLinear<CameraAnimationFrame,Aim>(this);
        result[UpVector()] = CalculateLinear<CameraAnimationFrame,UpVector>(this);
        break;
    case ANIMATION_BEZIER:
        result[Position()] = CalculateBezier<CameraAnimationFrame,Position>(this);
        result[Aim()] = CalculateBezier<CameraAnimationFrame,Aim>(this);
        result[UpVector()] = CalculateBezier<CameraAnimationFrame,UpVector>(this);
        break;
    default:
        checkf(false, "New algo type not handled by Camera Animations");
        break;
    }

    mCamera->assignerPosition(result[Position()]);
    mCamera->assignerPointVise(result[Aim()]);
    mCamera->assignerDirectionHaut(result[UpVector()]);

    return isAnimationFinished;
}

void AnimationCamera::SetCamera( vue::Camera* val )
{
    mCamera = val;
    checkf(mCamera);
    mCamera->attach(this);
}
