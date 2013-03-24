//////////////////////////////////////////////////////////////////////////////
/// @file HUDForeverAlone.cpp
/// @author Mathieu Parent
/// @date 2013-03-23
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDForeverAlone.h"
#include "HUDSurfaceGL.h"
#include "RazerGameUtilities.h"
#include "HUDTexte.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDForeverAlone::initSurfaces()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDForeverAlone::initSurfaces()
{
    memset(mTexture,0,sizeof(unsigned int));
    aidegl::glLoadTexture(RazerGameUtilities::NOM_DOSSIER_MEDIA+"ForeverAlone.png", mTexture[0]);

    ConteneurVertex2D* vertex = new ConteneurVertex2D();
    vertex->push_back(Vecteur2f(0.25f,0.25f));
    vertex->push_back(Vecteur2f(0.25f,0.75f));
    vertex->push_back(Vecteur2f(0.75f,0.75f));
    vertex->push_back(Vecteur2f(0.75f,0.25f));
    mSurface = new HUDSurfaceGL(GL_QUADS,vertex,Vecteur4f(1,1,1,1));
    mSurface->modifierTaille(0.5f,0.5f);

    mSurface->setTexture(mTexture[0]);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDForeverAlone::clearSurfaces()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDForeverAlone::clearSurfaces()
{
    delete mSurface;
    glDeleteTextures(1,mTexture);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDForeverAlone::peindreElement()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDForeverAlone::peindreElement()
{
    mSurface->repeindre();

    /*if(mModifiers)
    {
        float curX=0, curY=0;
        for(auto it=mModifiers->begin(); it!= mModifiers->end(); ++it)
        {
            BonusType type = (*it)->getType();
            mSurface->setTexture(mTextures[type]);

            float temps = (*it)->getTimeToLive();
            char buffer[8];
            sprintf(buffer,"%2.2f",temps);
            mTimerElement->setMessage(buffer);

            float pX = curX+obtenirX(), pY = curY+obtenirY();
            mSurface->modifierPosition(pX,pY);
            mTimerElement->modifierPosition(pX,pY+mSurface->obtenirHauteur()+0.02f);

            mSurface->repeindre();
            mTimerElement->repeindre();

            curX += mSurface->obtenirLargeur();
        }
    }*/
}
