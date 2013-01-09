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
	modParam1_ = modParam1;
	modParam3_ = modParam3;
	modParam2_ = modParam2;
	mode_ = OBJET;
	if(typeInterpolation<4 && typeInterpolation>=0)
	{
		interpolation_ = typeInterpolation;
	}
	else
	{
		throw std::exception("Type pas supporte");
	}
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::Animation( int mode, int typeInterpolation )
///
/// Ce constructeur initialise les valeurs de l'animation selon le mode (utile pour les cameras)
///
/// @param	mode				: mode de l'animation
///			typeInterpolation	: type d'interpolation de l'animation
/// 
/// @return Aucune (constructeur).
///
///////////////////////////////////////////////////////////////////////////////
Animation::Animation( int mode, int typeInterpolation )
{
	switch(mode)
	{
	case CAMERAPOINT:
		modParam1_ = true;
		modParam2_ = true;
		modParam3_ = false;
		repeat_ = false;
		break;
	case CAMERALIBRE:
		modParam1_ = true;
		modParam2_ = false;
		modParam3_ = false;
		repeat_ = false;
		break;
	case OBJET: // Cas par defaut = cas pour un objet avec tous les modificateurs
	default:
		modParam1_ = true;
		modParam3_ = true;
		modParam2_ = true;
		repeat_ = true;
		break;
	}
	mode_ = mode;
	indexFrameCourant_ = 0;
	tempsCourant_ = 0;
	termine_ = false;
	if(typeInterpolation<4 && typeInterpolation>=0)
	{
		interpolation_ = typeInterpolation;
	}
	else
	{
		throw std::exception("Type pas supporte");
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
/// @fn Animation::afficher() const 
///
/// Methode pour afficher le contenu de l'animation
///
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::afficher() const
{
	// PAS ENTIEREMENT TESTE

	std::cout << "Liste des frames: " << std::endl;
	ListeFrame::const_iterator it = frames_.begin();
	for(; it!=frames_.end(); it++)
	{
		std::cout << "\t";
		(*it)->afficher();
	}
	std::cout << std::endl << "Objets affectes: " << std::endl;
	ListeObjetsAnimable::const_iterator itObjets = objets_.begin();
	for(; itObjets!=objets_.end(); itObjets++)
	{
		std::cout << "\t";
		std::cout << (*itObjets)->obtenirNom() << std::endl;
	}
	std::cout << std::endl;

}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn Animation::animer(double temps) 
///
/// Methode pour avancer l'animaition dans le temps
///
/// @param temps: temps ecoule en ms
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void Animation::animer( double temps )
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
	double tempsFrameCourant = frameCourant->obtenirTemps();
	if(indexFrameCourant_<((int)frames_.size()-1))
	{
		// Pas dernier frame
		AnimationFrame* frameSuivant = frames_[indexFrameCourant_+1];
		double tempsFrameSuivant = frameSuivant->obtenirTemps();
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
	objet->assignerModificateurs(modParam1_, modParam2_, modParam3_);
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
	
	ListeObjetsAnimable::iterator it = objets_.begin();
	std::cout << "Index courant: " << indexFrameCourant_ << std::endl;
	std::cout << "Position: " << courant->obtenirParam1() << std::endl << std::endl;
	
	Vecteur3 nouvellePosition = NULL;
	Vecteur3 nouvelAngle = NULL;
	Vecteur3 nouvelleEchelle = NULL;
	if(modParam1_)
		nouvellePosition = courant->obtenirParam1();
	if(modParam2_)
		nouvelAngle = courant->obtenirParam2_();
	if(modParam3_)
		nouvelleEchelle = courant->obtenirParam3_();

	
	for(;it!=objets_.end(); it++)
	{
		(*it)->updateAttributs(nouvellePosition, nouvelAngle, nouvelleEchelle);
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
	ListeObjetsAnimable::iterator it = objets_.begin();

	Vecteur3 positionCourante = courant->obtenirParam1();
	Vecteur3 angleCourant = courant->obtenirParam2_();
	Vecteur3 echelleCourante = courant->obtenirParam3_();

	double deltaTemps = suivant->obtenirTemps()-courant->obtenirTemps();

	double ratio = (tempsCourant_-courant->obtenirTemps())/(deltaTemps);

	Vecteur3 deltaPosition = (suivant->obtenirParam1()-positionCourante)*ratio;
	Vecteur3 deltaAngle = (suivant->obtenirParam2_()-angleCourant)*ratio;
	Vecteur3 deltaEchelle = (suivant->obtenirParam3_()-echelleCourante)*ratio;

	Vecteur3 nouvellePosition = NULL;
	Vecteur3 nouvelAngle = NULL;
	Vecteur3 nouvelleEchelle = NULL;
	if(modParam1_)
		nouvellePosition = positionCourante+deltaPosition;
	if(modParam2_)
		nouvelAngle = angleCourant+deltaAngle;
	if(modParam3_)
		nouvelleEchelle = echelleCourante+deltaEchelle;

	for(;it!=objets_.end(); it++)
	{
		(*it)->updateAttributs(nouvellePosition, nouvelAngle, nouvelleEchelle);
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
	double deltaTemps = frames_[frames_.size()-1]->obtenirTemps()-frames_[0]->obtenirTemps();


	double t = tempsCourant_/deltaTemps;

	Vecteur3 nouvellePosition = NULL;
	Vecteur3 nouvelAngle = NULL;
	Vecteur3 nouvelleEchelle = NULL;
	Vecteur3 anciennePositionCamera = frames_[frames_.size()-1]->obtenirParam1();

	if(mode_==CAMERALIBRE)
		anciennePositionCamera = objets_[0]->obtenirAnimationParam1();

	if(modParam1_)
		nouvellePosition = calculerBezier(ANIMPOSITION, t, 0,(int)(frames_.size()-1));
	if(modParam2_)
		nouvelAngle = calculerBezier(ANIMANGLE, t, 0, (int)(frames_.size()-1));

	if(modParam3_)
		nouvelleEchelle = calculerBezier(ANIMECHELLE, t, 0, (int)(frames_.size()-1));

	ListeObjetsAnimable::iterator it = objets_.begin();

	
	for(;it!=objets_.end(); it++)
	{
		if(mode_==CAMERALIBRE)
		{
			Vecteur3 deltaPosition = nouvellePosition-anciennePositionCamera;
			deltaPosition.normaliser();
			deltaPosition[VZ] = (-1.0+deltaPosition[VZ])/2.0;
			nouvelAngle = nouvellePosition + deltaPosition;
		}
		
		(*it)->updateAttributs(nouvellePosition, nouvelAngle, nouvelleEchelle);
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
Vecteur3 Animation::calculerBezier(int attribut, double t,  int indexPremier, int indexDernier )
{
	if(indexPremier==indexDernier)
	{
		switch(attribut)
		{
		case ANIMPOSITION:
			return frames_[indexPremier]->obtenirParam1();
			break;
		case ANIMANGLE:
			return frames_[indexPremier]->obtenirParam2_();
			break;
		case ANIMECHELLE:
			return frames_[indexPremier]->obtenirParam3_();
			break;
		}
		
	}

	
	double tm = 1.0-t;

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




