//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurCollision.cpp
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "Noeuds\NodeControlPoint.h"



////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurCollision::VisiteurCollision( NoeudAbstrait* noeudAVerifier , bool flag /* = true*/ )
///
/// Constructeur par paramètre
///
/// @param[in] NoeudAbstrait * noeudAVerifier : Noeud à vérifier
/// @param[in] bool flag : indique si on surligne les noeuds visités
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurCollision::VisiteurCollision( NoeudAbstrait* noeudAVerifier , bool flag /* = true*/)
{
	flag_ = flag;
	collision_ = false;
	noeudAVerifier_ = noeudAVerifier;
	noeudAVerifier->assignerAttributVisiteurCollision(this);
	positionAVerifier_ = noeudAVerifier->obtenirPositionAbsolue().convertir<2>();


	/// PAS OUBLIER DE REINITIALISER LES NOUVELLES VARIABLE DANS LA METHODE REINITIALISER
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurCollision::VisiteurCollision(Vecteur2 position)
///
/// Constructeur qui prend en parametre une position
///
/// @param[in] position	: position d'un point a verifier
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurCollision::VisiteurCollision( Vecteur2 position , bool flag /* = true*/, float rayon /*= 5*/)
{
	flag_ = flag;
	collision_ = false;
	noeudAVerifier_ = NULL;
	rayonAVerifier_ = rayon;
	positionAVerifier_ = position;
	typeCollision_ = CERCLE;
	/// PAS OUBLIER DE REINITIALISER LES NOUVELLES VARIABLE DANS LA METHODE REINITIALISER
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurCollision::~VisiteurCollision()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurCollision::~VisiteurCollision(void)
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// vérification de collision avec un noeudAbstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	
	
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visiteur de collision pour un noeud composite
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudComposite( NoeudComposite* noeud )
{
	
	
	for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
	}

	
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visiteur de collision pour un muret
///
/// @param[in] NodeWallAbstract * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	if(noeud == noeudAVerifier_)
		return;

	if(flag_)
		noeud->modifierSurligner(false);

	switch(typeCollision_)
	{
	case SEGMENT:
		{
			aidecollision::DetailsCollision detailsCollision =aidecollision::calculerCollisionSegmentSegment(
				noeud->obtenirCoin1().convertir<2>(),
				noeud->obtenirCoin2().convertir<2>(),
				coin1_.convertir<2>(),
				coin2_.convertir<2>(),
				Vecteur2()	// Pas besoin de connaitre le point d'intersection des segments
				);
			if(detailsCollision.type != aidecollision::COLLISION_AUCUNE)
			{
				collision_ = true;
				conteneurDetailsCollision.push_back(detailsCollision);
				noeudsCollision_.push_back(noeud);
				if(flag_)
					noeud->modifierSurligner(true);
			}
			break;
		}
	default: // Aussi pour cercle
		aidecollision::DetailsCollision detailsCollision = aidecollision::calculerCollisionSegment(noeud->obtenirCoin1(), noeud->obtenirCoin2(), positionAVerifier_.convertir<3>(), rayonAVerifier_);
		if(detailsCollision.type != aidecollision::COLLISION_AUCUNE)
		{
			collision_ = true;
			noeudsCollision_.push_back(noeud);
			conteneurDetailsCollision.push_back(detailsCollision);
			if(flag_)
				noeud->modifierSurligner(true);
		}
		break;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudBut( NoeudBut* noeud )
///
/// Visiteur de collision pour un but
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudBut( NoeudBut* noeud )
{
	if(noeud == noeudAVerifier_)
		return;

	if(flag_)
		noeud->modifierSurligner(false);

	switch(typeCollision_)
	{
	case SEGMENT:
		{
			aidecollision::DetailsCollision detailsCollision1 =aidecollision::calculerCollisionSegmentSegment(
				noeud->obtenirParent()->obtenirPositionAbsolue().convertir<2>(),
				noeud->obtenirPositionHaut().convertir<2>(),
				coin1_.convertir<2>(),
				coin2_.convertir<2>(),
				Vecteur2()	// Pas besoin de connaitre le point d'intersection des segments
				);
			aidecollision::DetailsCollision detailsCollision2 =aidecollision::calculerCollisionSegmentSegment(
				noeud->obtenirParent()->obtenirPositionAbsolue().convertir<2>(),
				noeud->obtenirPositionBas().convertir<2>(),
				coin1_.convertir<2>(),
				coin2_.convertir<2>(),
				Vecteur2()	// Pas besoin de connaitre le point d'intersection des segments
				);
			if(detailsCollision1.type == aidecollision::COLLISION_AUCUNE && detailsCollision2.type == aidecollision::COLLISION_AUCUNE)
				break;
			aidecollision::DetailsCollision detailsCollision;
			detailsCollision.type = aidecollision::COLLISION_SEGMENT;
			detailsCollision.direction.remetAZero();
			detailsCollision.enfoncement = 0;
			
			if(detailsCollision1.type != aidecollision::COLLISION_AUCUNE)
			{
				detailsCollision.direction += detailsCollision1.direction;
				detailsCollision.enfoncement += detailsCollision1.enfoncement;
			}
			if(detailsCollision2.type != aidecollision::COLLISION_AUCUNE)
			{
				detailsCollision.direction += detailsCollision2.direction;
				detailsCollision.enfoncement += detailsCollision2.enfoncement;
			}
			{
				detailsCollision.direction.normaliser();
				collision_ = true;
				conteneurDetailsCollision.push_back(detailsCollision);
				noeudsCollision_.push_back(noeud);
				if(flag_)
					noeud->modifierSurligner(true);
			}
			break;
		}
	default: // Aussi pour cercle
		aidecollision::DetailsCollision detailsCollision1 = aidecollision::calculerCollisionSegment(noeud->obtenirParent()->obtenirPositionAbsolue(), noeud->obtenirPositionHaut(), positionAVerifier_.convertir<3>(), rayonAVerifier_);
		aidecollision::DetailsCollision detailsCollision2 = aidecollision::calculerCollisionSegment(noeud->obtenirParent()->obtenirPositionAbsolue(), noeud->obtenirPositionBas(), positionAVerifier_.convertir<3>(), rayonAVerifier_);
		
		if(detailsCollision1.type == aidecollision::COLLISION_AUCUNE && detailsCollision2.type == aidecollision::COLLISION_AUCUNE)
			break;
		
		aidecollision::DetailsCollision detailsCollision;
		detailsCollision.type = aidecollision::COLLISION_SEGMENT;
		detailsCollision.direction.remetAZero();
		detailsCollision.enfoncement = 0;

		if(detailsCollision1.type != aidecollision::COLLISION_AUCUNE)
		{
			detailsCollision.direction += detailsCollision1.direction;
			detailsCollision.enfoncement += detailsCollision1.enfoncement;
		}
		if(detailsCollision2.type != aidecollision::COLLISION_AUCUNE)
		{
			detailsCollision.direction += detailsCollision2.direction;
			detailsCollision.enfoncement += detailsCollision2.enfoncement;
		}
		{
			detailsCollision.direction.normaliser();
			collision_ = true;
			conteneurDetailsCollision.push_back(detailsCollision);
			noeudsCollision_.push_back(noeud);
			if(flag_)
				noeud->modifierSurligner(true);
		}
		break;
	}
	visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Visiteur de collision pour un maillet
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	switch(typeCollision_)
	{
	case SEGMENT: // Collision Cercle-Segment
		detectionCollisionCercleSegment(noeud);
		break;
	default: // Collision Cercle-Cercle
		detectionCollisionCercleCercle(noeud);
		break;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Visiteur de collision pour un portail
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudPortail( NoeudPortail* noeud )
{
	switch(typeCollision_)
	{
	case SEGMENT: // Collision Cercle-Segment
		detectionCollisionCercleSegment(noeud);
		break;
	default: // Collision Cercle-Cercle
		detectionCollisionCercleCercle(noeud);
		break;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Visiteur de collision pour la rondelle
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	switch(typeCollision_)
	{
	case SEGMENT: // Collision Cercle-Segment
		detectionCollisionCercleSegment(noeud);
		break;
	default: // Collision Cercle-Cercle
		detectionCollisionCercleCercle(noeud);
		break;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNoeudTable( NoeudTable* noeud )
///
/// Visiteur de collision pour la table
///
/// @param[in] NoeudTable * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudTable( NoeudTable* noeud )
{
	visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurCollision::visiterNoeudPoint 
///
/// Visiteur de collision pour un point
///
/// @param[in] NoeudTable * noeud	: Noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudPoint( NoeudPoint* noeud )
{
    // do not do collision detection with control points

// 	switch(typeCollision_)
// 	{
// 	case SEGMENT: // Collision Cercle-Segment
// 		detectionCollisionCercleSegment(noeud);
// 		break;
// 	default: // Collision Cercle-Cercle
// 		detectionCollisionCercleCercle(noeud);
// 		break;
// 	}
	visiterNoeudComposite(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurCollision::visiterNoeudAccelerateur()
///
/// Visiteur de collision pour un accélérateur
///
/// @param[in] NoeudAccelerateur * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	switch(typeCollision_)
	{
	case SEGMENT: // Collision Cercle-Segment
		detectionCollisionCercleSegment(noeud);
		break;
	default: // Collision Cercle-Cercle
		detectionCollisionCercleCercle(noeud);
		break;
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::detectionCollisionCercleCercle( NoeudAbstrait* noeud )
///
/// Algorithme de détection de collision entre 2 cercle
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::detectionCollisionCercleCercle( NoeudAbstrait* noeud )
{
	if(noeud == noeudAVerifier_)
		return;

	aidecollision::DetailsCollision detailsCollision = aidecollision::calculerCollisionSphere(positionAVerifier_.convertir<3>(),rayonAVerifier_,noeud->obtenirPositionAbsolue(),noeud->obtenirRayon());

	if(detailsCollision.type != aidecollision::COLLISION_AUCUNE)
	{
		collision_ = true;
		conteneurDetailsCollision.push_back(detailsCollision);
		noeudsCollision_.push_back(noeud);
		if(flag_)
			noeud->modifierSurligner(true);
	}
	else if(flag_)
	{
		noeud->modifierSurligner(false);
	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurCollision::obtenirListeCollision( ConteneurNoeuds &listeDeRetour )
///
/// Copie la liste de noeuds etant en collision avec le noeud de reference
///
/// @param[out] ConteneurNoeuds &listeDeRetour	: Variable pour le retour
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::obtenirListeCollision( ConteneurNoeuds &listeDeRetour )
{
	listeDeRetour = noeudsCollision_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurCollision::detectionCollisionCercleSegment( NoeudAbstrait* noeud )
///
/// Visite dans le cas d'une collision entre un noeud de reference qui est un segment et un noeud qui est un cercle
///
/// @param[in] NoeudAbstrait* noeud	: noeud a valider
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::detectionCollisionCercleSegment( NoeudAbstrait* noeud )
{
	if(noeud == noeudAVerifier_)
		return;

	Vecteur3 positionNoeudCercle = noeud->obtenirPositionAbsolue();

	aidecollision::DetailsCollision detailsCollision = aidecollision::calculerCollisionSegment(coin1_, coin2_, noeud->obtenirPositionAbsolue(), noeud->obtenirRayon());
	if(detailsCollision.type!=aidecollision::COLLISION_AUCUNE)
	{
		collision_ = true;
		conteneurDetailsCollision.push_back(detailsCollision);
		noeudsCollision_.push_back(noeud);
		if(flag_)
			noeud->modifierSurligner(true);
	}
	else if(flag_)
	{
		noeud->modifierSurligner(false);
	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool VisiteurCollision::collisionPresente()
///
/// Retourne le resultat
///
///
/// @return true s'il y a une collision
///
////////////////////////////////////////////////////////////////////////
bool VisiteurCollision::collisionPresente() const
{
	if(noeudAVerifier_!=NULL)
		if(noeudAVerifier_->obtenirType() == RazerGameUtilities::NAME_TABLE_CONTROL_POINT)
			return false;
	return collision_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int VisiteurCollision::obtenirNbrNoeudEncollision(  )
///
/// Permet de connaitre le nombre de noeud en collision detecter par le visiteur
///
///
/// @return unsigned int
///
////////////////////////////////////////////////////////////////////////
unsigned int VisiteurCollision::obtenirNbrNoeudEncollision()
{
	return (unsigned int)noeudsCollision_.size();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::reinitialiser()
///
/// Remet les paramètre du visiteur aux valeurs de départ pour une prochaine visitation
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::reinitialiser()
{
	collision_ = false;
	noeudsCollision_.clear();
	conteneurDetailsCollision.clear();
	noeudAVerifier_->assignerAttributVisiteurCollision(this);
	positionAVerifier_ = noeudAVerifier_->obtenirPositionAbsolue().convertir<2>();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurCollision::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurCollision::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    // do not do collision detection with control points
    VisitParent(NodeControlPoint);
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


