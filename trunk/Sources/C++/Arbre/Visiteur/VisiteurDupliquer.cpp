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
#include "RazerGameTree.h"
#include "Terrain.h"
#include "ControlPointMutableAbstract.h"
#include "NodeControlPoint.h"
#include "NodeWallEdition.h"

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
    Terrain* terrain = noeud->getField();
    NodeWallEdition *wall = dynamic_cast<NodeWallEdition *>(noeud);
    if(terrain && wall && wall->isAnyPointSelected())
	{
        // assume ici qu'un muret relatif ne peut etre selectionné
		NodeWallEdition* nouveauNoeud = (NodeWallEdition*)(arbre_->ajouterNouveauNoeud(RAZER_KEY_TABLE, noeud->getKey()));
		if(nouveauNoeud)
		{
            NodeControlPoint* coin1 = new NodeControlPoint();
            NodeControlPoint* coin2 = new NodeControlPoint();
            auto pos1 = noeud->obtenirCoin1();
            auto pos2 = noeud->obtenirCoin2();
            pos1[VX] += 5;
            pos2[VX] -= 5;
            coin1->setPosition(pos1);
            coin2->setPosition(pos2);
            nouveauNoeud->addControlPoint(coin1);
            nouveauNoeud->addControlPoint(coin2);
            nouveauNoeud->forceFullUpdate();
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
	unsigned int nbrEnfant = noeud->childCount();
	for (unsigned int i=0; i<nbrEnfant; ++i)
	{
		noeud->getChild(i)->acceptVisitor(*this);
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
    Terrain* terrain = noeud->getField();
	if(terrain && noeud -> IsSelected())
	{
		NoeudAbstrait* nouveauNoeud = arbre_->ajouterNouveauNoeud(RAZER_KEY_TABLE, noeud->getKey());
		if(nouveauNoeud != 0)
		{
            Vecteur2 position = noeud->getPosition().convertir<2>() + Vecteur2(noeud->getRadius(),noeud->getRadius());
            nouveauNoeud->setPosition(position);
            nouveauNoeud->forceFullUpdate();
			//Vecteur2 position;
// 			bool posValide = terrain->IsNodeAtValidEditionPosition(nouveauNoeud);
// 			for(int i = 1; i <= 2; ++i)
// 			{
// 				position = noeud->getPosition().convertir<2>() + Vecteur2(2*i*noeud->getRadius(), 2*i*noeud->getRadius());
// 				nouveauNoeud->setPosition(position.convertir<3>());
// 				posValide = terrain->IsNodeAtValidEditionPosition(nouveauNoeud);
// 
// 				if(posValide)
// 					break;
// 
// 				position = noeud->getPosition().convertir<2>() + Vecteur2(-2*i*noeud->getRadius(), 2*i*noeud->getRadius());
// 				nouveauNoeud->setPosition(position.convertir<3>());
// 				posValide = terrain->IsNodeAtValidEditionPosition(nouveauNoeud);
// 
// 				if(posValide)
// 					break;
// 
// 				position = noeud->getPosition().convertir<2>() + Vecteur2(-2*i*noeud->getRadius(), -2*i*noeud->getRadius());
// 				nouveauNoeud->setPosition(position.convertir<3>());
// 				posValide = terrain->IsNodeAtValidEditionPosition(nouveauNoeud);
// 
// 				if(posValide)
// 					break;
// 
// 				position = noeud->getPosition().convertir<2>() + Vecteur2(2*i*noeud->getRadius(), -2*i*noeud->getRadius());
// 				nouveauNoeud->setPosition(position.convertir<3>());
// 				posValide = terrain->IsNodeAtValidEditionPosition(nouveauNoeud);
// 
// 				if(posValide)
// 					break;
// 			}

// 			if(!posValide)
// 			{
//                 nouveauNoeud->deleteThis();
// 			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurDupliquer::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDupliquer::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    /// on 
//     if(noeud->IsSelected() && noeud->canBeVisitedAndRemoveFlag())
//     {
//         noeud->flagSelectedAssociatedPoints();
// 
//         NoeudAbstrait* obj = dynamic_cast<NoeudAbstrait*>(noeud->getLinkedObject());
//         if(obj)
//         {
//             obj->acceptVisitor(*this);
//         }
//     }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



