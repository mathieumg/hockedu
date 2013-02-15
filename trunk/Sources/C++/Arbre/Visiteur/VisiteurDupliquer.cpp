//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurCollision.cpp
/// @author Samuel Ledoux, Charles Étienne Lalonde
/// @date 2012-02-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
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
#include "FacadeModele.h"
#include "RazerGameTree.h"


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
VisiteurDupliquer::VisiteurDupliquer( RazerGameTree* arbre ):arbre_(arbre)
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
/// @fn VisiteurDupliquer::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visitation d'un noeud Muret
///
/// @param[in] NodeWallAbstract * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	if(noeud -> estSelectionne())
	{
        // assume ici qu'un muret relatif ne peut etre selectionné
		NoeudMuret* nouveauNoeud = (NoeudMuret*)(arbre_->ajouterNouveauNoeud(RazerGameUtilities::NOM_TABLE, noeud->obtenirType()));
		if(nouveauNoeud != 0)
		{
			for (int i=1; i<10; i++)
			{
				Vecteur3 positionClic1_(noeud->obtenirCoin1()+Vecteur3(0.0f,i*5.0f)), positionVirtuelle(noeud->obtenirCoin2()+Vecteur3(0.0f,i*5.0f));
				nouveauNoeud->assignerPositionCoin(1,positionClic1_);
				nouveauNoeud->assignerPositionCoin(2,positionVirtuelle);
				float deltaX = positionVirtuelle[VX]-positionClic1_[VX];
				float deltaY = positionVirtuelle[VY]-positionClic1_[VY];
				float angle = (atan2(deltaY, deltaX)*180.0f/(float)M_PI);
				float rayon = sqrt(deltaX*deltaX+deltaY*deltaY);
				if(rayon>1)
				{
					nouveauNoeud->assignerPositionCoin(2, positionVirtuelle);
					nouveauNoeud->modifierEchelleCourante(Vecteur3(rayon, 1, 1));
					nouveauNoeud->assignerAngle(angle);
					nouveauNoeud->updateMatrice();
				}
				if(FacadeModele::getInstance()->validerPositionNoeud(nouveauNoeud))
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
		noeud->chercher(i)->acceptVisitor(*this);
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
		NoeudAbstrait* nouveauNoeud = arbre_->ajouterNouveauNoeud(RazerGameUtilities::NOM_TABLE, noeud->obtenirType());
		if(nouveauNoeud != 0)
		{

			Vecteur2 position;
			bool posValide = FacadeModele::getInstance()->validerPositionNoeud(nouveauNoeud);
			for(int i = 1; i <= 2; ++i)
			{
				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(2*i*noeud->obtenirRayon(), 2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::getInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;

				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(-2*i*noeud->obtenirRayon(), 2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::getInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;

				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(-2*i*noeud->obtenirRayon(), -2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::getInstance()->validerPositionNoeud(nouveauNoeud);

				if(posValide)
					break;

				position = noeud->obtenirPositionRelative().convertir<2>() + Vecteur2(2*i*noeud->obtenirRayon(), -2*i*noeud->obtenirRayon());
				nouveauNoeud->assignerPositionRelative(position.convertir<3>());
				posValide = FacadeModele::getInstance()->validerPositionNoeud(nouveauNoeud);

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



