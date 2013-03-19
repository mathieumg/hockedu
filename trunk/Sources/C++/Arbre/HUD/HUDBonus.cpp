//////////////////////////////////////////////////////////////////////////////
/// @file HUDBonus.cpp
/// @author Michael Ferris
/// @date 2013-03-18
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDBonus.h"
#include "HUDSurfaceGL.h"
#include "RazerGameUtilities.h"
#include "HUDTexte.h"
#include "BonusModifierAbstract.h"

unsigned int HUDBonus::mTextures[NB_BONUS_TYPE];
HUDSurfaceGL* HUDBonus::mSurface = NULL;
HUDTexte* HUDBonus::mTimerElement = NULL;

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDBonus::initSurfaces()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDBonus::initSurfaces()
{
    const std::string iconPath = RazerGameUtilities::NOM_DOSSIER_MEDIA+"BonusIcons/";
    memset(mTextures,0,NB_BONUS_TYPE*sizeof(mTextures));
    aidegl::glLoadTexture(iconPath+"God.jpg", mTextures[BONUS_TYPE_GO_THROUGH_WALL]);
    aidegl::glLoadTexture(iconPath+"Goaler.png", mTextures[BONUS_TYPE_BLOCK_GOAL]);

    ConteneurVertex2D* vertex = new ConteneurVertex2D();
    vertex->push_back(Vecteur2f(0,0));
    vertex->push_back(Vecteur2f(0,1));
    vertex->push_back(Vecteur2f(1,1));
    vertex->push_back(Vecteur2f(1,0));
    mSurface = new HUDSurfaceGL(GL_QUADS,vertex,Vecteur4f(1,1,1,1));
    mSurface->modifierTaille(0.05f,0.05f);
    mTimerElement = new HUDTexte(TEXTE,Vecteur4f(1,1,1,1));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDBonus::clearSurfaces()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDBonus::clearSurfaces()
{
    delete mSurface;
    delete mTimerElement;
    glDeleteTextures(NB_BONUS_TYPE,mTextures);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDBonus::peindreElement()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDBonus::peindreElement()
{
    if(mModifiers)
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
            mTimerElement->modifierPosition(pX,pY+mSurface->obtenirHauteur()+0.02);

            mSurface->repeindre();
            mTimerElement->repeindre();

            curX += mSurface->obtenirLargeur();
        }
    }
}