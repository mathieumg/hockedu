#pragma once

template<typename FrameDefinition, typename Element>
typename Element::value_type CalculateLinear(const AnimationBase<FrameDefinition>* animation )
{
    const std::vector<FrameDefinition>& frames = animation->GetFrames();
    int iFrame = animation->CurrentFrame();
    // at last frame
    if(iFrame == frames.size()-1)
    {
        return frames[iFrame][Element()];
    }

    float currentFrameTime = frames[iFrame][Time()];
    float nextFrameTime = frames[iFrame+1][Time()];

    float deltaTime = nextFrameTime - currentFrameTime;
    float ratio = (animation->CurrentTime() - currentFrameTime)/deltaTime;

    return (frames[iFrame][Element()]*(1.f-ratio)) + (ratio* frames[iFrame+1][Element()]);
}

template<typename FrameDefinition, typename Element>
typename Element::value_type CalculateBezier(const AnimationBase<FrameDefinition>* animation)
{
    const std::vector<FrameDefinition>& frames = animation->GetFrames();

    float deltaTemps = frames[frames.size()-1][Time()]-frames[0][Time()];
    float t = animation->CurrentTime()/deltaTemps;


    return calculerBezier<FrameDefinition,Element>(frames,t,0,(int)frames.size()-1);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::calculerBezier( int indexPremier, int indexDernier )
///
/// Methode recursive pour calculer la position avec une courbe bezier a n points
///
/// @param attribut : attribut a calculer (position, angle ou echelle)
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
template<typename FrameDefinition, typename Element>
typename Element::value_type calculerBezier(const std::vector<FrameDefinition>& frames, float t,  int first, int last )
{
    if(first==last)
    {
        return frames[first][Element()];
    }
    float tm = 1.0f-t;

    Element::value_type nouvelAttribut = tm*calculerBezier<FrameDefinition,Element>(frames, t, first, last-1)+t*calculerBezier<FrameDefinition,Element>(frames, t, first+1, last);

    return nouvelAttribut;
}