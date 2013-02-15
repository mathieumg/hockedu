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
#include "FacadeModele.h"
#include "ZoneEdition.h"
#include "Terrain.h"
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurModifierProprieteNoeud::VisiteurModifierProprieteNoeud( JNIEnv* env, jobject& evenementSouris )
///
/// Constructeur qui prend en paramètre l'environnement de JNI et 
/// l'objet qui contient les modifications a apporter.
///
/// @param[in] JNIEnv * env : l'environnement JNI
/// @param[in] jobject & modificateur : l'objet contenant les modifications a apporter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VisiteurModifierProprieteNoeud::VisiteurModifierProprieteNoeud( JNIEnv* env, jobject& modificateur)
{
	//On met le coef de friction dans la variable associé.
	jclass classe = env->GetObjectClass(modificateur);
	jmethodID obtenirCoefFriction = env->GetMethodID(classe, "obtenirCoefFriction", "()D");
	coefFriction_ = (float)(env->CallDoubleMethod(modificateur, obtenirCoefFriction));

	//On met le coef de rebond dans la variable associé.
	jmethodID obtenirCoefRebond = env->GetMethodID(classe, "obtenirCoefRebond", "()D");
	coefRebond_ = (float)(env->CallDoubleMethod(modificateur, obtenirCoefRebond));

	//On met le coef de bonusAccel dans la variable associé.
	jmethodID obtenirBonusAccel = env->GetMethodID(classe, "obtenirBonusAccel", "()D");
	bonusAccel_ = (float)(env->CallDoubleMethod(modificateur, obtenirBonusAccel));

	//On met la position dans la variable associé.
	jmethodID obtenirPositionX = env->GetMethodID(classe, "obtenirPositionX", "()D");
	position_[VX] = (float)(env->CallDoubleMethod(modificateur, obtenirPositionX));
	jmethodID obtenirPositionY = env->GetMethodID(classe, "obtenirPositionY", "()D");
	position_[VY] = (float)(env->CallDoubleMethod(modificateur, obtenirPositionY));

	//On met l'Echelle dans la variable associé.
	jmethodID obtenirEchelle = env->GetMethodID(classe, "obtenirEchelle", "()D");
	echelle_ = (float)(env->CallDoubleMethod(modificateur, obtenirEchelle));
	

	//On met la rotation dans la variable associé.
	jmethodID obtenirRotation = env->GetMethodID(classe, "obtenirRotation", "()I");
	rotation_ = env->CallIntMethod(modificateur, obtenirRotation);

	//On met la longueur de la zone d'edition dans la variable associé.
	jmethodID obtenirZoneEditionX = env->GetMethodID(classe, "obtenirZoneEditionX", "()D");
	longueurZoneEdition_ = (float)(env->CallDoubleMethod(modificateur, obtenirZoneEditionX));

	//On met la hauteur de la zone d'edition dans la variable associé.
	jmethodID obtenirZoneEditionY = env->GetMethodID(classe, "obtenirZoneEditionY", "()D");
	hauteurZoneEdition_ = (float)(env->CallDoubleMethod(modificateur, obtenirZoneEditionY));

	unSeulSelect_ = FacadeModele::getInstance()->obtenirNbNoeudSelect() == 1;

	// Coef de rebond des bandes
	jmethodID obtenirCoefRebondBandes = env->GetMethodID(classe, "obtenirCoefRebondBandes", "(I)D");
	for (int i=0;i<8;++i)
	{
		coefRebondBandes_[i] = (float)(env->CallDoubleMethod(modificateur, obtenirCoefRebondBandes,i));
	}
	
}


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
	for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
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
	if(noeud->estSelectionne())
	{
		if(coefRebond_!=-1)
			noeud->setReboundRatio(coefRebond_);		
		if(unSeulSelect_)
		{
			Vecteur3 oldPos = noeud->obtenirPositionRelative();
			noeud->assignerPositionRelative(position_);
			float oldAngle = noeud->obtenirAngle();
			noeud->assignerAngle((float)rotation_);
			Vecteur3 oldEchelle; noeud->obtenirEchelleCourante(oldEchelle);
			noeud->modifierEchelleCourante(Vecteur3(echelle_*10, 1.0, 1.0));
			noeud->majPosCoins();

			// Si on arrive pas à assigner les nouvelles positions on annule les modifications et l'indique à l'usager
			if(!FacadeModele::getInstance()->ajusterElementEnCollision(noeud,20))
			{
				noeud->assignerPositionRelative(oldPos);
				noeud->assignerAngle(oldAngle);
				noeud->modifierEchelleCourante(oldEchelle);
				noeud->majPosCoins();
				utilitaire::afficherErreur("Nouvelles propriétés du Muret ne sont pas valides");
			}

		}
	}
	if(noeud->getReboundRatio()==-1)
		int test = 1;

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
		noeud->assignerCoefRebond(i,coefRebondBandes_[i]);
	}
	noeud->modifierCoefFriction(coefFriction_);

	ZoneEdition* zone;
	if(noeud->GetTerrain() && (zone = &noeud->GetTerrain()->getZoneEdition()) )
	{
		// On déclare un pointeur sur un float qui servira a contenir la longueur et hauteur max de la table
		float boiteEnglobantTable[2];
		// On obtient la longueur et hauteur max
		noeud->calculerHautLongMax(boiteEnglobantTable);

		// Si la longueur a assigner est plus petite que la longueur maximale de la table, on cap a la longueur de la table
		if(longueurZoneEdition_<(boiteEnglobantTable[1]))
		{
			zone->modifierLimiteExtLongueur(boiteEnglobantTable[1]);
		}
		// si plus grand ou egale
		else
		{
			zone->modifierLimiteExtLongueur(longueurZoneEdition_);
		}

		// Si la largeur a assigner est plus petite que la largeur maximale de la table, on cap a la largeur de la table
		if(hauteurZoneEdition_<(boiteEnglobantTable[0]))
		{
			zone->modifierLimiteExtLargeur(boiteEnglobantTable[0]);
		}
		// si plus grand ou egale
		else
		{
			zone->modifierLimiteExtLargeur(hauteurZoneEdition_);
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
	if(noeud->estSelectionne())
	{
		// Assigner la position avant pour que le calcul de la longueur max des buts soit mise a jour
		Vecteur3 positionCourante = noeud->obtenirPositionAbsolue();
		
		if(noeud->GetTerrain() )
		{
			ZoneEdition* zone = &noeud->GetTerrain()->getZoneEdition();
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
			
				if(abs(position_[VY])>zone->obtenirLimiteExtLargeur())
					position_[VY] = signe[VY]*zone->obtenirLimiteExtLargeur();
				if(abs(position_[VY])<zone->obtenirLimiteIntLargeur())
					position_[VY] = signe[VY]*zone->obtenirLimiteIntLargeur();
			}
			else if(noeud->obtenirTypePosNoeud() == POSITION_MILIEU_GAUCHE || noeud->obtenirTypePosNoeud() == POSITION_MILIEU_DROITE)
			{
				if(abs(position_[VX])>zone->obtenirLimiteExtLongueur())
					position_[VX] = signe[VX]*zone->obtenirLimiteExtLongueur();
				if(abs(position_[VX])<zone->obtenirLimiteIntLongueur())
					position_[VX] = signe[VX]*zone->obtenirLimiteIntLongueur();
			}
			else if(noeud->obtenirTypePosNoeud() == POSITION_HAUT_GAUCHE || noeud->obtenirTypePosNoeud() == POSITION_HAUT_DROITE || noeud->obtenirTypePosNoeud() == POSITION_BAS_DROITE || noeud->obtenirTypePosNoeud() == POSITION_BAS_GAUCHE)
			{
				if(abs(position_[VY])>zone->obtenirLimiteExtLargeur())
					position_[VY] = signe[VY]*zone->obtenirLimiteExtLargeur();
				if(abs(position_[VY])<zone->obtenirLimiteIntLargeur())
					position_[VY] = signe[VY]*zone->obtenirLimiteIntLargeur();
				if(abs(position_[VX])>zone->obtenirLimiteExtLongueur())
					position_[VX] = signe[VX]*zone->obtenirLimiteExtLongueur();
				if(abs(position_[VX])<zone->obtenirLimiteIntLongueur())
					position_[VX] = signe[VX]*zone->obtenirLimiteIntLongueur();
			}
		}
		// Le visiteur permet de mettre a jour les buts en meme temps
		Vecteur3 deplacement = position_-positionCourante;
		deplacement[VX]*=-1;
		VisiteurDeplacement visiteur(deplacement);
		noeud->acceptVisitor(visiteur);

		if(!FacadeModele::getInstance()->ajusterElementSurTableEnCollision())
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
	if(noeud->estSelectionne() && bonusAccel_!=-1)
		noeud->modifierBonusAccel(bonusAccel_);
}


void VisiteurModifierProprieteNoeud::visiterNoeudNeutre( NoeudAbstrait* noeud )
{
	if(unSeulSelect_ && noeud->estSelectionne())
	{
		/// On fait le deplacement contenu dans position_ par rapport à l'origine
		/*Vecteur3 deplacement = ((position_.convertir<3>())-(noeud->obtenirPositionAbsolue()));
		VisiteurDeplacement visiteurDeplacement(deplacement,true);
		noeud->acceptVisitor(visiteurDeplacement);*/
		Vecteur3 oldPos = noeud->obtenirPositionRelative();
		noeud->assignerPositionRelative(position_);

		float oldAngle = noeud->obtenirAngle();
		/// On applique la nouvelle rotation
		VisiteurRotation rotationAFaire((float)rotation_,position_);
		noeud->acceptVisitor(rotationAFaire);

		Vecteur3 oldEchelle; noeud->obtenirEchelleCourante(oldEchelle);


		if(noeud->obtenirType() == "portail")
			noeud->modifierEchelleCourante(Vecteur3(echelle_, echelle_, 1));
		else
			noeud->modifierEchelleCourante(Vecteur3(echelle_, echelle_, echelle_));


		noeud->updateMatrice();

		/// On regle les nouvelles collision créé
		if(!FacadeModele::getInstance()->ajusterElementEnCollision(noeud,20))
		{
			noeud->assignerPositionRelative(oldPos);
			noeud->assignerAngle(oldAngle);
			noeud->modifierEchelleCourante(oldEchelle);
			noeud->updateMatrice();
			utilitaire::afficherErreur("Nouvelles propriétés du noeud ne sont pas valides");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



