///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireAnimations.cpp
/// @author Michael Ferris
/// @date 2012-03-09
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GestionnaireAnimations.h"
#include "Vue/Camera.h"


// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireAnimations);

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireAnimations::GestionnaireAnimations(  )
///
/// Constructeur, initialise le gestionnaire
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireAnimations::GestionnaireAnimations()
{
	replay_ = new AnimationReprise(LINEAIRE, true, true, true);
	jouerReplay_ = false;
	replayTermine_ = false;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireAnimations::~GestionnaireAnimations(  )
///
/// Destructeur Vide
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireAnimations::~GestionnaireAnimations()
{
	ListeAnimations::iterator itAnimations = animations_.begin();
	for(;itAnimations!=animations_.end(); itAnimations++)
	{
		delete (*itAnimations);
		(*itAnimations) = 0;
	}
	delete replay_;
	replay_ = 0;
}

// 
// ////////////////////////////////////////////////////////////////////////
// ///
// /// @fn GestionnaireAnimations* GestionnaireAnimations::obtenirInstance()
// ///
// /// Cette fonction retourne un pointeur vers l'instance unique de la
// /// classe.  Si cette instance n'a pas été créée, elle la crée.  Cette
// /// création n'est toutefois pas nécessairement "thread-safe", car
// /// aucun verrou n'est pris entre le test pour savoir si l'instance
// /// existe et le moment de sa création.
// ///
// /// @return Un pointeur vers l'instance unique de cette classe.
// ///
// ////////////////////////////////////////////////////////////////////////
// GestionnaireAnimations* GestionnaireAnimations::obtenirInstance()
// {
// 	if (instance_ == 0)
// 		instance_ = new GestionnaireAnimations;
// 
// 	return instance_;
// }
// 
// 
// ////////////////////////////////////////////////////////////////////////
// ///
// /// @fn void GestionnaireAnimations::libererInstance()
// ///
// /// Cette fonction libère l'instance unique de cette classe.
// ///
// /// @return Aucune.
// ///
// ////////////////////////////////////////////////////////////////////////
// void GestionnaireAnimations::libererInstance()
// {
// 	delete instance_;
// 	instance_ = 0;
// }


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::ajouterAnimation( Animation* animation )
///
/// Ajoute l'animation au groupe
///
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::ajouterAnimation( Animation* animation )
{
	animations_.push_back(animation);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::retirerAnimation( Animation* animation )
///
/// Retire l'animation du groupe
///
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::retirerAnimation( Animation* animation )
{
	ListeAnimations::iterator it = animations_.begin();
	for(; it!=animations_.end(); it++)
	{
		if((*it)==animation)
		{
			delete (*it);
			(*it) = 0;
			animations_.erase(it);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::retirerAnimation( float temps )
///
/// Anime les animations
///
/// @param temps : temps depuis la derniere animation
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::animer( float temps )
{
	ListeAnimations::iterator aSuprimer = animations_.end();
	ListeAnimations::iterator itAnimations = animations_.begin();
	for(;itAnimations!=animations_.end(); itAnimations++)
	{
		Animation* courant = (*itAnimations);
		if(courant->estTermine())
			aSuprimer = itAnimations;
		else
			courant->animer(temps);
	}

	if(jouerReplay_)
	{
		if(!replay_->estTermine())
			replay_->animer(temps);
		else
			jouerReplay_ = false; // Desactive le replay une fois termine
	}

	// On supprime la nimation qui n'est plus bonne

	if(aSuprimer!=animations_.end())
	{
		
		Animation* courant = (*aSuprimer);
		delete courant;
		courant = 0;
		animations_.erase(aSuprimer);
	}

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::delierObjet( ObjetAnimable* objet )
///
/// Delie un objet des animations
/// ex: il n'existe plus
///
/// @param objet : objet a delier
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::delierObjet( ObjetAnimable* objet )
{
	ListeAnimations::iterator itAnimations = animations_.begin();
	for(;itAnimations!=animations_.end(); itAnimations++)
	{
		(*itAnimations)->delierObjet(objet);
	}

	// Pour effecer une animation de la liste si elle est vide
	ListeAnimations::iterator itAnimations2 = animations_.begin();
	for(;itAnimations2!=animations_.end(); itAnimations2++)
	{
		if((*itAnimations2)->obtenirNbObjets()==0)
		{
			Animation* aEffacer = (*itAnimations2);
			delete aEffacer;
			aEffacer = 0;
			animations_.erase(itAnimations2);
			break;
		}
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::jouerReplay()
///
/// Demarre l'animation de replay
///
/// @param objet : objet a delier
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::jouerReplay(vue::Camera camera)
{
	jouerReplay_ = true;
	ancienneCamera_ = camera;
	replayTermine_ = false;
	replay_->resetTerminer();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::estJouerReplay()
///
/// Retourne true si le replay est en cours de lecture
///
/// 
/// @return bool	: true si en cours de lecture de l'animation
///
////////////////////////////////////////////////////////////////////////
bool GestionnaireAnimations::estJouerReplay() const
{
	return jouerReplay_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::saveReplayFrame(ObjetAnimable* objet, AnimationFrame* frame )
///
/// Ajoute une frame pour le replay
///
/// @param	frame	: frame a ajouter
/// @param	objet	: objet lie a la frame
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::saveReplayFrame( IterationReplay* frame )
{
	replay_->ajouterFrame(frame);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::saveReplaySound( int choix )
///
/// Ajoute une frame avec le sou pour le replay
///
/// @param	choix	: int correspondant au son a jouer
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::saveReplaySound( int choix )
{

	replay_->ajouterSon(choix);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool GestionnaireAnimations::replayTermine( )
///
/// Envoie a facadeModele de faire un delais de la partie courante
///
/// @param[out]	bool	: true si le replay est termine
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
bool GestionnaireAnimations::replayEstTermine( ) 
{
	if(replayTermine_)
	{
		replayTermine_ = false;
		return true;
	}
	return false;

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::viderBufferReplay()
///
/// Vide les frames du replay.
/// 
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::viderBufferReplay()
{
	replay_->viderListe();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireAnimations::viderAnimationCamera(  )
///
/// Permet de supprimer les animation existantes qui affecte la camera
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireAnimations::viderAnimationCamera()
{
	// Important de laisser i en unsigned int
	for(unsigned int i= 0; i<animations_.size(); ++i)
	{
		for(int j = 0; j< animations_[i]->obtenirNbObjets(); ++j)
		{
			// On regarde si l'objet est une camera
			if( dynamic_cast<vue::Camera*>(animations_[i]->obtenirObjets(j)) )
			{
				// si oui on supprime cette animation
				retirerAnimation(animations_[i]);
				// on reajuste l'index
				--i;
				break;
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////