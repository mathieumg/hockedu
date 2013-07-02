#include "AnimationNodePosition.h"
#include "AnimationAlgo.h"
#include "NoeudAbstrait.h"

AnimationNodePosition::AnimationNodePosition():
    mNode(NULL)
{
}

/// Execute the animation and returns true if the animation is over (can be deleted)
bool AnimationNodePosition::Execute()
{
    checkf(mNode);
    bool isAnimationFinished = false;
    AnimationExecuteWithTime(isAnimationFinished);

    NodePositionFrame result;
    switch (mAlgo)
    {
    case ANIMATION_LINEAR:
        result[Position()] = CalculateLinear<NodePositionFrame,Position>(this);
        break;
    case ANIMATION_BEZIER:
        result[Position()] = CalculateBezier<NodePositionFrame,Position>(this);
        break;
    default:
        checkf(false, "New algo type not handled by Node Position Animations");
        break;
    }

    mNode->setPosition(result[Position()]);

    return isAnimationFinished;
}

void AnimationNodePosition::SetNode( NoeudAbstrait* val )
{
    mNode = val;
    checkf(mNode);
    mNode->attach(this);
}
