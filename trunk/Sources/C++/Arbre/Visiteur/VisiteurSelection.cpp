//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurSelection.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurSelection.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "VisiteurCollision.h"
#include "Terrain.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurSelection::VisiteurSelection( Vecteur2 positionMin, Vecteur2 positionMax )
///
/// Constructeur par paramètres
///
/// @param[in] Vecteur2 positionMin : Position du premier clique de souris pour la sélection
/// @param[in] Vecteur2 positionMax : Position du deuxième clique de souris pour la sélection
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurSelection::VisiteurSelection( Vecteur2 positionMin, Vecteur2 positionMax )
{
	
	positionClicMin_[VX] = min(positionMin[VX], positionMax[VX]);
	positionClicMin_[VY] = min(positionMin[VY], positionMax[VY]);
	positionClicMax_[VX] = max(positionMin[VX], positionMax[VX]);
	positionClicMax_[VY] = max(positionMin[VY], positionMax[VY]);

	if(positionClicMin_==positionClicMax_)
		avecRectangle = false;
	else
		avecRectangle = true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurSelection::~VisiteurSelection()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurSelection::~VisiteurSelection(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurSelection::visiterNoeudAbstrait()
///
/// Visitation d'un noeud Abstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visitation d'un noeud composite.
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudComposite( NoeudComposite* noeud )
{
	for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visitation d'un noeud muret.
///
/// @param[in] NodeWallAbstract * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	if(noeud->estSelectionnable())
	{
		if(avecRectangle)
		{
			visiterNoeudAffichable(noeud);
		}
		else
		{
			VisiteurCollision visiteurCollision(positionClicMin_, false);
            noeud->GetTerrain()->acceptVisitor(visiteurCollision);
			if(visiteurCollision.collisionPresente())
			{
				ConteneurNoeuds listeNoeuds;
				visiteurCollision.obtenirListeCollision(listeNoeuds);
				// N'entre pas dans la boucle s'il n'y a pas de noeuds a selectionner
				for(int i=0; i<listeNoeuds.size(); i++)
				{
					
					aSelectionner_ = NoeudZBuf(listeNoeuds[i], 0.0f);
				}
				
			}
		}
	}
    visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudBut( NoeudBut* noeud )
///
/// Écriture d'un noeud représentant un but.
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudBut( NoeudBut* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Écriture d'un noeud représentant un maillet.
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudAffichable(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Écriture d'un noeud représentant un portail.
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudPortail( NoeudPortail* noeud )
{
	visiterNoeudAffichable(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Écriture d'un noeud représentant une rondelle.
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudAffichable(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudTable( NoeudTable* noeud )
///
/// Écriture d'un noeud représentant une table.
///
/// @param[in] NoeudTable * noeud : noeud è vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudTable( NoeudTable* noeud )
{
	visiterNoeudComposite(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visiteur de selection pour le noeudPoint
///
/// @param[in] NoeudPoint * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudPoint( NoeudPoint* noeud )
{
	visiterNoeudAffichable(noeud);
	visiterNoeudComposite(noeud);

}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudAffichable( NoeudAbstrait* noeud )
///
/// Algortihme de selection des noeuds
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudAffichable( NoeudAbstrait* noeud )
{
	
	// Quitte si le noeud n'est pas selectionnable
	if(!noeud->estSelectionnable())
		return;

	NoeudAbstrait::PaireVect3 zoneOccuppee = noeud->obtenirZoneOccupee();
	Vecteur3 pos = noeud->getPosition();

	
	bool selection = false;
	if(avecRectangle)
	{
		// Test pour voir si le centre du noeud est a l'interieur du rectangle
		if(	pos[VX]>positionClicMin_[VX] &&
			pos[VY]>positionClicMin_[VY] &&
			pos[VX]<positionClicMax_[VX] &&
			pos[VY]<positionClicMax_[VY])
		{
			noeud->selectionnerTout();
		}
	}
	else
	{
		
		// Test pour voir si le clic est sur le noeud

		//Utilisation du code du visiteur de collision pour une meilleure efficacite
		VisiteurCollision visiteurCollision(positionClicMin_);
		//FacadeModele::getInstance()->
		if(	zoneOccuppee.first[VX]<positionClicMin_[VX] && 
			zoneOccuppee.first[VY]<positionClicMin_[VY] && 
			zoneOccuppee.second[VX]>positionClicMax_[VX] && 
			zoneOccuppee.second[VY]>positionClicMax_[VY])
		{
			selection = true;
		}
	}
	if(selection)
	{
		// On effectue la selection du noeud

		// On verifie que le noeud est sur le dessus (seulement pour un clic)
		if(zoneOccuppee.second[VZ]>aSelectionner_.second || avecRectangle)
		{
			if(avecRectangle)
				noeud->selectionnerTout(); // On selectionne si le noeud n'est pas selectionne
			else
				aSelectionner_ = NoeudZBuf(noeud, zoneOccuppee.second[VZ]);
		}
	}
		
		
		
		
		
		/*
		// Test pour voir si le clic est sur le noeud

		//Utilisation du code du visiteur de collision pour une meilleure efficacite
		VisiteurCollision* visiteurCollision = new VisiteurCollision(positionClicMin_);
		FacadeModele::getInstance()->visiterArbre((VisiteurNoeud*)visiteurCollision);
		ConteneurNoeuds conteneur;
		visiteurCollision->obtenirListeCollision(conteneur);
		for(int i=1; i<conteneur.size(); i++)
		{
			float posZCourante = conteneur[i]->getPosition()[VZ];
			if(posZCourante>=aSelectionner_.second && conteneur[i]->estSelectionnable() || avecRectangle)
			{
				if(avecRectangle)
	 				noeud->selectionnerTout(); // On selectionne si le noeud n'est pas selectionne
		 		else
		 			aSelectionner_ = NoeudZBuf(noeud, posZCourante);
			}
		}

		delete visiteurCollision;
	}


	*/
// 	if(selection)
// 	{
// 		// On effectue la selection du noeud
// 
// 		// On verifie que le noeud est sur le dessus (seulement pour un clic)
// // 		if(zoneOccuppee.second[VZ]>aSelectionner_.second || avecRectangle)
// // 		{
// // 			if(avecRectangle)
// 				noeud->selectionnerTout(); // On selectionne si le noeud n'est pas selectionne
// // 			else
// // 				aSelectionner_ = NoeudZBuf(noeud, zoneOccuppee.second[VZ]);
// // 		}
// 	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visiteur de selection pour le noeudAccelerateur
///
/// @param[in] NoeudAccelerateur * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	visiterNoeudAffichable(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelection::faireSelection()
///
/// Effectue la selection sur le noeud leplus a lavant
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelection::faireSelection()
{
	if(aSelectionner_.first!=NULL)
	{
		NoeudAbstrait* aSel = aSelectionner_.first;
		if(aSel->estSelectionne() && !avecRectangle)
			aSel->deselectionnerTout(); // On deselectionne si le noeud est deja selectionne
		else
			aSel->selectionnerTout(); // On selectionne si le noeud n'est pas selectionne
	}
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


