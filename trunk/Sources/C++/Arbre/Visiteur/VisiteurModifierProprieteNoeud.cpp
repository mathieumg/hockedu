//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurModifierProprieteNoeud.cpp
/// @author Samuel Ledoux
/// @date 2012-02-17
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurModifierProprieteNoeud.h"
#include "VisiteurDeplacement.h"
#include "VisiteurRotation.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "NoeudPortail.h"
#include "ZoneEdition.h"
#include "Terrain.h"
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurModifierProprieteNoeud::~VisiteurModifierProprieteNoeud( void )
///
/// Destructeur desallouant le pointeur sur l'environnement JNI.
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VisiteurModifierProprieteNoeud::~VisiteurModifierProprieteNoeud( void )
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Visiteur du noeud abstrait.
///
/// @param[in] NoeudAbstrait * noeud : le noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visiteur du noeud composite.
///
/// @param[in] NoeudComposite * noeud : le noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudComposite( NoeudComposite* noeud )
{
	for (unsigned int i=0; i<noeud->childCount(); i++)
	{
		noeud->getChild(i)->acceptVisitor(*this);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visiteur du noeud muret.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudMuret( NodeWallAbstract* noeud )
{

	/// On applique le nouveau coefficient de rebond
	if(noeud->IsSelected())
	{
		if(coefRebond_!=-1)
			noeud->setReboundRatio(coefRebond_);		
		if(unSeulSelect_)
		{
			const Vecteur3& oldPos = noeud->getPosition();
			noeud->setPosition(position_);
			float oldAngle = noeud->getAngle();
			noeud->setAngle((float)rotation_);
			Vecteur3 oldEchelle; noeud->getScale(oldEchelle);
			noeud->setScale(Vecteur3(echelle_*10, oldEchelle[VY], oldEchelle[VZ]));
            Terrain* field = noeud->getField();
			// Si on arrive pas à assigner les nouvelles positions on annule les modifications et l'indique à l'usager
			if(!field || !field->FixCollindingNode(noeud,20))
			{
				noeud->setPosition(oldPos);
				noeud->setAngle(oldAngle);
				noeud->setScale(oldEchelle);
				utilitaire::afficherErreur("Nouvelles propriétés du Muret ne sont pas valides");
			}
		}
	}
    visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudBut( NoeudBut* noeud )
///
/// Visiteur du noeud but.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudBut( NoeudBut* noeud )
{
    float length = noeud->getLength();
	
	noeud->updateLongueur((echelle_*10)/length);
	noeud->obtenirButAdverse()->updateLongueur((echelle_*10)/length);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Visiteur du noeud maillet.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudNeutre(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Visiteur du noeud portail.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudPortail( NoeudPortail* noeud )
{
	visiterNoeudNeutre(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Visiteur du noeud rondelle.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudNeutre(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudTable( NoeudTable* noeud )
///
/// Visiteur du noeud table.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudTable( NoeudTable* noeud )
{
	for(int i=0;i<8;++i)
	{
		noeud->modifierCoefRebond(i,coefRebondBandes_[i]);
	}
	noeud->modifierCoefFriction(coefFriction_);

	ZoneEdition* zone;
	if(noeud->getField() && (zone = noeud->getField()->getZoneEdition()) )
	{
		// On déclare un pointeur sur un float qui servira a contenir la longueur et hauteur max de la table
		float boiteEnglobantTable[2];
		// On obtient la longueur et hauteur max
		noeud->calculerHautLongMax(boiteEnglobantTable);

		// Si la longueur a assigner est plus petite que la longueur maximale de la table, on cap a la longueur de la table
		if(longueurZoneEdition_<(boiteEnglobantTable[1]))
		{
			zone->modifierLimiteExtX(boiteEnglobantTable[1]);
		}
		// si plus grand ou egale
		else
		{
			zone->modifierLimiteExtX(longueurZoneEdition_);
		}

		// Si la largeur a assigner est plus petite que la largeur maximale de la table, on cap a la largeur de la table
		if(hauteurZoneEdition_<(boiteEnglobantTable[0]))
		{
			zone->modifierLimiteExtY(boiteEnglobantTable[0]);
		}
		// si plus grand ou egale
		else
		{
			zone->modifierLimiteExtY(hauteurZoneEdition_);
		}
	}


	visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visiteur du noeud point.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudPoint( NoeudPoint* noeud )
{
	if(noeud->IsSelected() && noeud->canBeVisitedAndRemoveFlag())
	{
        noeud->flagSelectedAssociatedPoints();
		// Assigner la position avant pour que le calcul de la longueur max des buts soit mise a jour
		const Vecteur3& positionCourante = noeud->getPosition();
		
		if(noeud->getField() )
		{
			ZoneEdition* zone = noeud->getField()->getZoneEdition();
            checkf(zone,"Tentative de modifier un noeud qui n'est pas dans un terrain pour le mode édition");
            if(zone)
            {
                int signe[2];
                for(int i=0; i<2; i++)
                {
                    if(position_[i]<0)
                        signe[i] = -1;
                    else
                        signe[i] = 1;
                }
                if(noeud->obtenirTypePosNoeud() == POSITION_HAUT_MILIEU || noeud->obtenirTypePosNoeud() == POSITION_BAS_MILIEU)
                {

                    if(utilitaire::ABSOLUTETABARNAK(position_[VY])>zone->obtenirLimiteExtY())
                        position_[VY] = signe[VY]*zone->obtenirLimiteExtY();
                    if(utilitaire::ABSOLUTETABARNAK(position_[VY])<zone->obtenirLimiteIntY())
                        position_[VY] = signe[VY]*zone->obtenirLimiteIntY();
                }
                else if(noeud->obtenirTypePosNoeud() == POSITION_MILIEU_GAUCHE || noeud->obtenirTypePosNoeud() == POSITION_MILIEU_DROITE)
                {
                    if(utilitaire::ABSOLUTETABARNAK(position_[VX])>zone->obtenirLimiteExtX())
                        position_[VX] = signe[VX]*zone->obtenirLimiteExtX();
                    if(utilitaire::ABSOLUTETABARNAK(position_[VX])<zone->obtenirLimiteIntX())
                        position_[VX] = signe[VX]*zone->obtenirLimiteIntX();
                }
                else if(noeud->obtenirTypePosNoeud() == POSITION_HAUT_GAUCHE || noeud->obtenirTypePosNoeud() == POSITION_HAUT_DROITE || noeud->obtenirTypePosNoeud() == POSITION_BAS_DROITE || noeud->obtenirTypePosNoeud() == POSITION_BAS_GAUCHE)
                {
                    if(utilitaire::ABSOLUTETABARNAK(position_[VY])>zone->obtenirLimiteExtY())
                        position_[VY] = signe[VY]*zone->obtenirLimiteExtY();
                    if(utilitaire::ABSOLUTETABARNAK(position_[VY])<zone->obtenirLimiteIntY())
                        position_[VY] = signe[VY]*zone->obtenirLimiteIntY();
                    if(utilitaire::ABSOLUTETABARNAK(position_[VX])>zone->obtenirLimiteExtX())
                        position_[VX] = signe[VX]*zone->obtenirLimiteExtX();
                    if(utilitaire::ABSOLUTETABARNAK(position_[VX])<zone->obtenirLimiteIntX())
                        position_[VX] = signe[VX]*zone->obtenirLimiteIntX();
                }
            }
		}
		// Le visiteur permet de mettre a jour les buts en meme temps
		Vecteur2 deplacement = position_-positionCourante;
		VisiteurDeplacement visiteur(deplacement);
		noeud->acceptVisitor(visiteur);

        Terrain* field = noeud->getField();
		if(!field || !field->FixCollidingObjects())
		{
			VisiteurDeplacement visiteur(deplacement*-1);
			noeud->acceptVisitor(visiteur);
			utilitaire::afficherErreur("Nouvelles propriétés du point ne sont pas valides");
		}


		visiterNoeudComposite(noeud);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurModifierProprieteNoeud::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visiteur du noeud accelerateur.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurModifierProprieteNoeud::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	visiterNoeudNeutre(noeud);

	/// On applique le nouveau bonus d'acceleration des accelerateurs
	if(noeud->IsSelected() && bonusAccel_!=-1)
		noeud->modifierBonusAccel(bonusAccel_);
}


void VisiteurModifierProprieteNoeud::visiterNoeudNeutre( NoeudAbstrait* noeud )
{
	if(unSeulSelect_ && noeud->IsSelected())
	{
		/// On fait le deplacement contenu dans position_ par rapport à l'origine
		/*Vecteur3 deplacement = ((position_.convertir<3>())-(noeud->getPosition()));
		VisiteurDeplacement visiteurDeplacement(deplacement,true);
		noeud->acceptVisitor(visiteurDeplacement);*/
		const Vecteur3& oldPos = noeud->getPosition();
		noeud->setPosition(position_);

		float oldAngle = noeud->getAngle();
		/// On applique la nouvelle rotation
		VisiteurRotation rotationAFaire((float)rotation_,position_);
		noeud->acceptVisitor(rotationAFaire);

		Vecteur3 oldEchelle; noeud->getScale(oldEchelle);


		if(noeud->getKey() == RAZER_KEY_PORTAL)
			noeud->setScale(Vecteur3(echelle_, echelle_, 1));
		else
			noeud->setScale(Vecteur3(echelle_, echelle_, echelle_));


		noeud->updateMatrice();
        Terrain* field = noeud->getField();
		/// On regle les nouvelles collision créé
        if(!field || !field->FixCollindingNode(noeud,20))
		{
			noeud->setPosition(oldPos);
			noeud->setAngle(oldAngle);
			noeud->setScale(oldEchelle);
			noeud->updateMatrice();
			utilitaire::afficherErreur("Nouvelles propriétés du noeud ne sont pas valides");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



