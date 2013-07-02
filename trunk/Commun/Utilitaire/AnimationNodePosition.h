#pragma once
#include "Vecteur.h"
#include "AnimationFrameDefinitions.h"
#include "VariableTemplate.h"
#include "AnimationBase.h"

using namespace AnimationFrame;
class NoeudAbstrait;
typedef ctmap<Time,Position> NodePositionFrame;

class AnimationNodePosition : public AnimationBase<NodePositionFrame>
{
protected:
    AnimationNodePosition();
public:
    // only the manager can create new animations
    friend class ManagerAnimations;

    void AddFrame(float time, const Vecteur3& pos)
    {
        NodePositionFrame frame;
        frame[Time()]     = time;
        frame[Position()] = pos;
        mFrames.push_back(frame);
    }

    /// Execute the animation and returns true if the animation is over (can be deleted)
    virtual bool Execute();
    void SetNode(NoeudAbstrait* val);

private:
    NoeudAbstrait* mNode;
};