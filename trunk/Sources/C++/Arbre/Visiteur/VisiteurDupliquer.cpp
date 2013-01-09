//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurCollision.cpp
/// @author Samuel Ledoux, Charles Étienne Lalonde
/// @date 2012-02-13
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "VisiteurDupliquer.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "ArbreRenduINF2990.h"
#include "FacadeModele.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::VisiteurDupliquer( NoeudAbstrait* arbre )
///
/// Constructeur par paramêtre
///
/// @param[in] NoeudAbstrait * arbre : racine de l'arbre à vérifier
///
/// @return Aucune
///
////////////////////////////////////////////////////////////////////////
VisiteurDupliquer::VisiteurDupliquer( ArbreRenduINF2990* arbre ):arbre_(arbre)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Visitation d'un noeud Abstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	dupliquerNoeud(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visitation d'un noeud Composite
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudComposite( NoeudComposite* noeud )
{
	//dupliquerNoeud(noeud);
	visiterEnfants(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudMuret( NoeudMuret* noeud )
///
/// Visitation d'un noeud Muret
///
/// @param[in] NoeudMuret * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudMuret( NoeudMuret* noeud )
{
	if(noeud -> estSelectionne())
	{
		NoeudMuret* nouveauNoeud = dynamic_cast<NoeudMuret*>(arbre_->ajouterNouveauNoeud(ArbreRenduINF2990::NOM_TABLE, noeud->obtenirType()));
		if(nouveauNoeud != 0)
		{
			for (int i=1; i<10; i++)
			{
				Vecteur3 positionClic1_(noeud->obtenirCoin1()+Vecteur3(0.0,i*5.0)), positionVirtuelle(noeud->obtenirCoin2()+Vecteur3(0.0,i*5.0));
				nouveauNoeud->assignerPositionCoin(1,positionClic1_);
				nouveauNoeud->assignerPositionCoin(2,positionVirtuelle);
				double deltaX = positionVirtuelle[VX]-positionClic1_[VX];
				double deltaY = positionVirtuelle[VY]-positionClic1_[VY];
				double angle = (atan2(deltaY, deltaX)*180.0/(double)M_PI);
				double rayon = sqrt(deltaX*deltaX+deltaY*deltaY);
				if(rayon>1)
				{
					nouveauNoeud->assignerPositionCoin(2, positionVirtuelle);
					nouveauNoeud->modifierEchelleCourante(Vecteur3(rayon, 1, 1));
					nouveauNoeud->assignerAngle((int)angle);
					nouveauNoeud->updateMatrice();
				}
				if(FacadeModele::obtenirInstance()->validerPositionNoeud(nouveauNoeud))
				{
					
					return;
				}
			}
			arbre_->effacer(nouveauNoeud);
		}
	}
}
////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudBut( NoeudBut* noeud )
///
/// Visitation d'un noeud But
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudBut( NoeudBut* noeud )
{

}
////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Visitation d'un noeud Maillet
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	dupliquerNoeud(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Visitation d'un noeud Portail
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudPortail( NoeudPortail* noeud )
{
	dupliquerNoeud(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Visitation d'un noeud Rondelle
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudRondelle( NoeudRondelle* noeud )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudTable( NoeudTable* noeud )
///
/// Visitation d'un noeud Table
///
/// @param[in] NoeudTable * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudTable( NoeudTable* noeud )
{
	visiterEnfants(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visitation d'un noeud Point
///
/// @param[in] NoeudPoint * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudPoint( NoeudPoint* noeud )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visitation d'un noeud Accélérateur
///
/// @param[in] NoeudAccelerateur * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	dupliquerNoeud (noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurDupliquer::visiterEnfants( NoeudComposite* noeud )
///
/// Visitation des enfants d'un noeud composite
///
/// @param[in] NoeudComposite * noeud : Noeud parent
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterEnfants( NoeudComposite* noeud )
{
	unsigned int nbrEnfant = noeud->obtenirNombreEnfants();
	for (unsigned int i=0; i<nbrEnfant; ++i)
	{
		noeud->chercher(i)->accueillirVisiteurNoeud(*this);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDupliquer::dupliquerNoeud( NoeudAbstrait* noeud )
///
/// Algortihme de duplication d'un noeud
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::dupliquerNoeud( NoeudAbstrait* noeud )
{
	if(noeud -> estSelectionne())
	{
		NoeudAbstrait* nouveauNoeud = arbre_->ajouterNouveauNoeud(ArbreRenduINF2990::NOM_TABLE, noeud->obtenirType());
		if(nouveauNoeud != 0)
		{

			Vecteur2 position;
			bool posValide = FacadeModele::obtenirInstance()->validerPositionNoeud(nouveauNoeud);
			for(int i = 1; i <= 2; ++i)
			{
				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(2*i*noeud->obtenirRayon(), 2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::obtenirInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;

				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(-2*i*noeud->obtenirRayon(), 2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::obtenirInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;

				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(-2*i*noeud->obtenirRayon(), -2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::obtenirInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;

				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(2*i*noeud->obtenirRayon(), -2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::obtenirInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;
			}

			if(!posValide)
			{
				arbre_->effacer(nouveauNoeud);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



