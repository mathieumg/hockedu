///////////////////////////////////////////////////////////////////////////////
/// @file AnimationReprise.cpp
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "AnimationReprise.h"
#if WIN32
#include "SoundFMOD.h"
#endif
#include <iostream>
#include "GestionnaireAnimations.h"



///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::AnimationReprise( int typeInterpolation )
///
/// Ce constructeur initialise les valeurs de l'AnimationReprise
///
/// @return Aucune (constructeur).
///
///////////////////////////////////////////////////////////////////////////////
AnimationReprise::AnimationReprise( int typeInterpolation,bool modParam1 /*= true*/, bool modParam2 /*= true*/, bool modParam3 /*= true*/, bool repeat/*=false*/  )
	: Animation(typeInterpolation, modParam1, modParam2, modParam3, repeat)
{
	
}




///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::~AnimationReprise(  )
///
/// Destructeur de AnimationReprise
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
AnimationReprise::~AnimationReprise( void )
{
	viderListe();

}





///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::ajouterFrame( AnimationRepriseFrame* )
///
/// Methode pour ajouter un frame a la liste
///
/// @param frame : frame a ajouter
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::ajouterFrame( IterationReplay* frame)
{
	// On supprime le premier avant d'ajouter afin d'avoir 10 sec de frames
	while(obtenirDureeTotale()>10000)
	{
		// Bien supprimer la memoire

		for(int i=0; i<(int)frames_.front()->obtenirListe().size(); i++)
		{
			AnimationRepriseFrame* frameReprise = frames_.front()->obtenirListe()[i];
			delete frameReprise->first;
			frameReprise->first = 0;
			delete frameReprise;
			frameReprise = 0;
		}
		delete frames_.front();
		frames_.front() = 0;

		frames_.pop_front();
	}
	if(frames_.size()>0)
	{
		float tempsDernier = frames_.back()->obtenirTemps();
		//AnimationFrame* frameCourant = frame.second;
		frame->modifierTemps(frame->obtenirTemps()+tempsDernier);
		for(int i=0; i<(int)frame->obtenirListe().size(); i++)
		{
			frame->obtenirListe()[i]->first->modifierTemps(frame->obtenirTemps());
		}
		
	}
	else
	{
		frame->modifierTemps(0.0);

	}

	frames_.push_back(frame);
	frameCourant_ = frames_.begin();
}




///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::ajouterObjet( ObjetAnimable* objet )
///
/// Methode pour ajouter un objet a la liste
///
/// @param objet : objet a ajouter
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::ajouterObjet( ObjetAnimable* objet )
{
// 	objet->assignerModificateurs(modParam1_, modParam2_, modParam3_);
// 	objets_.push_back(objet);
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::delierObjet( ObjetAnimable* objet )
///
/// Methode pour delier l'objet de l'AnimationReprise
/// ex: l'objet est supprimer
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::delierObjet( ObjetAnimable* objet )
{
// 	ListeObjetsAnimable::iterator it = objets_.begin();
// 	for(;it!=objets_.end(); it++)
// 	{
// 		if((*it)==objet)
// 		{
// 			objets_.erase(it);
// 			break;
// 		}
// 	}
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::obtenirDureeTotale()
///
/// Methode pour obtenir le delta temps entre le premier et le dernier frame
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
float AnimationReprise::obtenirDureeTotale()
{
	if(frames_.size()<2)
		return 0.0;

	float premier = frames_.front()->obtenirTemps();
	float dernier = frames_.back()->obtenirTemps();
	float delta = dernier-premier;

	return delta;

}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::animer( float temps )
///
/// Methode pour jouer l'animation
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::animer( float temps )
{
	if(termine_ || frames_.size()==0)
		return;
	
	tempsCourant_ += temps;

	if(tempsCourant_<frames_.front()->obtenirTemps())
		tempsCourant_ = frames_.front()->obtenirTemps();

	float tempsFrameCourant = (*frameCourant_)->obtenirTemps();

	if(*frameCourant_ == frames_.back())
	{
		arreterAnimation();
		return;
	}

	ListeFrameReprise::iterator suivant = frameCourant_;
	suivant++;

	if(*suivant == frames_.back())
	{
		arreterAnimation();
		return;
	}

	
	while(tempsCourant_ > (*suivant)->obtenirTemps())
	{
		frameCourant_++;
		int son = (*frameCourant_)->obtenirChoixSon();
#if WIN32
		if(son!=-1)
			SoundFMOD::obtenirInstance()->playEffect(effect(son));
#endif
		if(*frameCourant_!=frames_.back())
		{
			suivant++;
			if(*suivant!=frames_.back())
			{
				indexFrameCourant_++;
			}
			else
			{
				GestionnaireAnimations::obtenirInstance()->terminerReplay();
				return;
			}
		}
		else
		{
			GestionnaireAnimations::obtenirInstance()->terminerReplay();
			return;
		}
		
	}

	for(int i=0; i<(int)(*frameCourant_)->obtenirListe().size(); i++)
	{
		objets_.clear();
		objets_.push_back((*frameCourant_)->obtenirListe()[i]->second);

		// On anime avec le frame courant et le suivant
		switch(interpolation_)
		{
		case CONSTANT:
			animerConstant((*frameCourant_)->obtenirListe()[i]->first);
			break;
		case LINEAIRE:
			animerLineaire((*frameCourant_)->obtenirListe()[i]->first, (*suivant)->obtenirListe()[i]->first);
			break;
		case BEZIER:
			break;
		default:
			break;
		}
	}
	

}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::ajouterSon( int choix )
///
/// Methode pour ajouter un son au dernier frame de la liste
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::ajouterSon( int choix )
{
	if(
#if WIN32
       choix!=BEEP_EFFECT &&
#endif
       frames_.size()>0)
	{
		IterationReplay* dernierFrame = frames_.back();

		dernierFrame->modifierChoixSon(choix);
	}

}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::arreterAnimation()
///
/// Methode pour arreter une animation en cours
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::arreterAnimation()
{
	Animation::arreterAnimation(); // Met le bool termine_ a true
	frameCourant_ = frames_.begin();
	tempsCourant_ = 0.0;
	
	/*
	// On replace les elements a leur position finale
	if(frames_.size()<1)
		return;

	IterationReplay* dernierFrame = frames_.back();

	for(int i=0; i<(*frameCourant_)->obtenirListe().size(); i++)
	{
		objets_.clear();
		objets_.push_back(dernierFrame->obtenirListe()[i]->second);

		animerConstant(dernierFrame->obtenirListe()[i]->first);
	}
	viderListe();*/
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn AnimationReprise::viderListe()
///
/// Methode pour vider la liste de frames
///
/// 
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void AnimationReprise::viderListe()
{
	ListeFrameReprise::iterator it = frames_.begin();

	for(; it!=frames_.end(); it++)
	{
		for(int i=0; i<(int)(*it)->obtenirListe().size(); i++)
		{
			AnimationRepriseFrame* frameReprise = (*it)->obtenirListe()[i];
			delete frameReprise->first;
			frameReprise->first = 0;
			delete frameReprise;
			frameReprise = 0;
		}
		delete (*it);
		(*it) = 0;
	}
	frames_.clear();
    termine_ = true;
}





///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




