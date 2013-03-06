///////////////////////////////////////////////////////////////////////////////
/// @file Animation.cpp
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "Animation.h"
#include "ObjetAnimable.h"
#include "AnimationFrame.h"
#include <iostream>
#include "ExceptionJeu.h"

///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::Animation( int typeInterpolation )
///
/// Ce constructeur initialise les valeurs de l'animation
///
/// @return Aucune (constructeur).
///
///////////////////////////////////////////////////////////////////////////////
Animation::Animation( int typeInterpolation,bool modParam1 /*= true*/, bool modParam2 /*= true*/, bool modParam3 /*= true*/, bool repeat/*=false*/  )
{
	indexFrameCourant_ = 0;
	tempsCourant_ = 0;
	termine_ = false;
	repeat_ = repeat;
    mAnimationResult.setCanUpdatedPosition(modParam1);
    mAnimationResult.setCanUpdatedAngle(modParam2);
    mAnimationResult.setCanUpdatedScale(modParam3);
	if(typeInterpolation<4 && typeInterpolation>=0)
	{
		interpolation_ = typeInterpolation;
	}
	else
	{
		throw ExceptionJeu("Type pas supporte");
	}
}



///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::~Animation(  )
///
/// Destructeur de Animation
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
Animation::~Animation( void )
{
	// On supprime les frames
	ListeFrame::iterator it = frames_.begin();
	for(;it!=frames_.end(); it++)
	{
		delete (*it);
		(*it) = 0;
	}
	frames_.clear();

	// On ne supprime pas les objets car ils sont utilises ailleurs
	objets_.clear();
}





///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::ajouterFrame( AnimationFrame* )
///
/// Methode pour ajouter un frame a la liste
///
/// @param frame : frame a ajouter
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::ajouterFrame( AnimationFrame* frame)
{
	
	frames_.push_back(frame);
	
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::animer(float temps) 
///
/// Methode pour avancer l'animaition dans le temps
///
/// @param temps: temps ecoule en ms
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::animer( float temps )
{
	if(termine_)
		return;

	tempsCourant_ += temps;

	// On ne fait rien si le premier frame n'est pas encore arrive ou que le dernier est passe
	if(tempsCourant_ < frames_[0]->obtenirTemps())
	{
		return;
	}

	AnimationFrame* frameCourant = frames_[indexFrameCourant_];
	float tempsFrameCourant = frameCourant->obtenirTemps();
	if(indexFrameCourant_<((int)frames_.size()-1))
	{
		// Pas dernier frame
		AnimationFrame* frameSuivant = frames_[indexFrameCourant_+1];
		float tempsFrameSuivant = frameSuivant->obtenirTemps();
		if((tempsCourant_)>tempsFrameSuivant)
		{
			// Si le temps a augmente de plus qu'une frame
			if(interpolation_!=BEZIER)
				indexFrameCourant_++;
			else if(tempsCourant_>frames_[frames_.size()-1]->obtenirTemps())
			{
				if(repeat_)
				{
					tempsCourant_ = 0.0;
					indexFrameCourant_ = 0;
				}
				else
				{
					termine_ = true;
				}
			}

		}
		
		// On anime avec le frame courant et le suivant
		switch(interpolation_)
		{
		case CONSTANT:
			animerConstant(frameCourant);
			break;
		case LINEAIRE:
			animerLineaire(frameCourant, frameSuivant);
			break;
		case BEZIER:
			animerBezier();
			break;
		default:
			break;
		}
		
	}
	else
	{
		// Dernier frame
		if(repeat_)
		{
			tempsCourant_ = 0.0;
			indexFrameCourant_ = 0;
		}
		else
			termine_ = true;

	}
	
}



///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::ajouterObjet( ObjetAnimable* objet )
///
/// Methode pour ajouter un objet a la liste
///
/// @param objet : objet a ajouter
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::ajouterObjet( ObjetAnimable* objet )
{
	objets_.push_back(objet);
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::animerConstant(AnimationFrame* courant, AnimationFrame* suivant)
///
/// Methode pour avancer l'animaition dans le temps de maniere constante
///
/// @param[in]	courant	: pointeur vers le frame courant
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::animerConstant(AnimationFrame* courant)
{
	if(mAnimationResult.CanUpdatedPosition())
		mAnimationResult.mPosition = courant->getPosition();
	if(mAnimationResult.CanUpdatedAngle())
		mAnimationResult.mAngle = courant->getAngle();
	if(mAnimationResult.CanUpdatedScale())
		mAnimationResult.mScale = courant->getScale();

    ListeObjetsAnimable::iterator it = objets_.begin();
    for(;it!=objets_.end(); it++)
	{
		(*it)->appliquerAnimation(mAnimationResult);
	}
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::animerLineaire(AnimationFrame* courant, AnimationFrame* suivant)
///
/// Methode pour avancer l'animaition dans le temps de maniere lineaire
///
/// @param[in]	courant	: pointeur vers le frame courant
/// @param[in]	suivant	: pointeur vers le frame suivant
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::animerLineaire( AnimationFrame* courant, AnimationFrame* suivant )
{
	if(indexFrameCourant_>=(int)frames_.size()-1)
		return;

	const Vecteur3& positionCourante = courant->getPosition();
	const Vecteur3& angleCourant = courant->getAngle();
	const Vecteur3& echelleCourante = courant->getScale();

	float deltaTemps = suivant->obtenirTemps()-courant->obtenirTemps();
	float ratio = (tempsCourant_-courant->obtenirTemps())/(deltaTemps);

	const Vecteur3& deltaPosition = (suivant->getPosition()-positionCourante)*ratio;
	const Vecteur3& deltaAngle = (suivant->getAngle()-angleCourant)*ratio;
	const Vecteur3& deltaEchelle = (suivant->getScale()-echelleCourante)*ratio;

    if(mAnimationResult.CanUpdatedPosition())
        mAnimationResult.mPosition = positionCourante+deltaPosition;
    if(mAnimationResult.CanUpdatedAngle())
        mAnimationResult.mAngle = angleCourant+deltaAngle;
    if(mAnimationResult.CanUpdatedScale())
        mAnimationResult.mScale = echelleCourante+deltaEchelle;

    ListeObjetsAnimable::iterator it = objets_.begin();
    for(;it!=objets_.end(); it++)
    {
        (*it)->appliquerAnimation(mAnimationResult);
    }
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::animerBezier(AnimationFrame* courant, AnimationFrame* suivant)
///
/// Methode pour avancer l'animaition dans le temps de maniere bezier
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::animerBezier( )
{
	float deltaTemps = frames_[frames_.size()-1]->obtenirTemps()-frames_[0]->obtenirTemps();
	float t = tempsCourant_/deltaTemps;

    if(mAnimationResult.CanUpdatedPosition())
        mAnimationResult.mPosition = calculerBezier(ANIMPOSITION, t, 0,(int)(frames_.size()-1));
    if(mAnimationResult.CanUpdatedAngle())
        mAnimationResult.mAngle = calculerBezier(ANIMANGLE, t, 0, (int)(frames_.size()-1));
    if(mAnimationResult.CanUpdatedScale())
        mAnimationResult.mScale = calculerBezier(ANIMECHELLE, t, 0, (int)(frames_.size()-1));

    ListeObjetsAnimable::iterator it = objets_.begin();
    for(;it!=objets_.end(); it++)
    {
        (*it)->appliquerAnimation(mAnimationResult);
    }
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::delierObjet( ObjetAnimable* objet )
///
/// Methode pour delier l'objet de l'animation
/// ex: l'objet est supprimer
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::delierObjet( ObjetAnimable* objet )
{
	ListeObjetsAnimable::iterator it = objets_.begin();
	for(;it!=objets_.end(); it++)
	{
		if((*it)==objet)
		{
			objets_.erase(it);
			break;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::obtenirNbObjets() const
///
/// Methode pour obtenir le nombre d'objets a animer
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
int Animation::obtenirNbObjets() const
{
	return (int)objets_.size();
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
Vecteur3 Animation::calculerBezier(int attribut, float t,  int indexPremier, int indexDernier )
{
	if(indexPremier==indexDernier)
	{
		switch(attribut)
		{
		case ANIMPOSITION:
			return frames_[indexPremier]->getPosition();
			break;
		case ANIMANGLE:
			return frames_[indexPremier]->getAngle();
			break;
		case ANIMECHELLE:
			return frames_[indexPremier]->getScale();
			break;
		}
	}

	
	float tm = 1.0f-t;

	Vecteur3 nouvelAttribut = tm*calculerBezier(attribut, t, indexPremier, indexDernier-1)+t*calculerBezier(attribut, t, indexPremier+1, indexDernier);


	return nouvelAttribut;
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::resetTerminer()
///
/// Methode pour remettre le flag terminer a false
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::resetTerminer()
{
	termine_ = false;
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




