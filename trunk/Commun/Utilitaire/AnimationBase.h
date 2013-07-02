#pragma once
#include "AnimationObserver.h"
#include <vector>
class AnimationAlgoBase;

enum AnimationAlgo{ANIMATION_LINEAR,ANIMATION_BEZIER};

#define AnimationExecuteWithTime(isAnimationFinished)  \
const int nbFrames = (int)mFrames.size();                                           \
                                                                                    \
/* Do not run the animation if the first time frame hasn't been reached*/           \
if(mCurrentTime < mFrames[0][Time()])                                               \
{                                                                                   \
    return false;                                                                   \
}                                                                                   \
                                                                                    \
while(mCurrentFrame < nbFrames-1 && mCurrentTime > mFrames[mCurrentFrame+1][Time()])\
{                                                                                   \
    ++mCurrentFrame;                                                                \
                                                                                    \
    /* check if on last frame  */                                                   \
    if(mCurrentFrame >= nbFrames-1)                                                 \
    {                                                                               \
        /* animation over,finish   */                                               \
        isAnimationFinished = true;                                                 \
    }                                                                               \
}

class AnimationInterface
{
public:
    friend class ManagerAnimations;
    AnimationInterface():mIsCanceled(false){}
    virtual ~AnimationInterface(){}
    virtual bool ExecuteBase(float deltaTime) = 0;
    bool IsCanceled() const { return mIsCanceled; }
    void Cancel()
    {
        mIsCanceled = true;
    }
private:
    bool mIsCanceled;
    // Unique ID of the animation in the manager
    unsigned int mId;
};

// use observer pattern to cancel animations
template<typename FrameDefinition>
class AnimationBase : public AnimationObserver, public AnimationInterface
{
protected:
    AnimationBase(): mAlgo(ANIMATION_LINEAR),mCurrentTime(0),mCurrentFrame(0)
    {

    }
public:

    virtual ~AnimationBase(){}
    /// Execute the animation and returns true if the animation is over (can be deleted)
    virtual bool ExecuteBase(float deltaTime)
    {
        // empty animation
        if(mFrames.empty())
        {
            return true;
        }
        mCurrentTime += deltaTime;
        return Execute();
    }

    virtual void updateObserver( const AnimationSubject* pSubject )
    {
        Cancel();
    }

    inline AnimationAlgo GetAlgo() const { return mAlgo; }
    inline void SetAlgo(AnimationAlgo val) { mAlgo = val; }
    inline float CurrentTime() const { return mCurrentTime; }
    inline int CurrentFrame() const { return mCurrentFrame; }
    const std::vector<FrameDefinition>& GetFrames() const { return mFrames; }
protected:
    /// Execute the animation and returns true if the animation is over (can be deleted)
    virtual bool Execute() = 0;



    std::vector<FrameDefinition> mFrames;
    int mCurrentFrame;
    float mCurrentTime;

    AnimationAlgo mAlgo;
};

