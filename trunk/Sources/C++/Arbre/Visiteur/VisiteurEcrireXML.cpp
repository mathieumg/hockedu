//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEcrireXML.cpp
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurEcrireXML.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudPoint.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudAccelerateur.h"
#include "XMLUtils.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurEcrireXML::VisiteurEcrireXML()
///
/// Constructeur qui initialisela racine.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurEcrireXML::VisiteurEcrireXML(void)
{
	racine_ = 0;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurEcrireXML::~VisiteurEcrireXML()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurEcrireXML::~VisiteurEcrireXML(void)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// �criture d'un noeud abstrait.
///
/// @param[in] NoeudAbstrait * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	if(!noeud->estEnregistrable())
		return;

	// Cr�er le noeud 
	XmlElement* elementNoeud = noeud->creerNoeudXML();

	if(racine_ == 0)
	{
		racine_ = elementNoeud;
		parent_ = racine_;
	}
	else
	{
        XMLUtils::LinkEndChild(parent_,elementNoeud);
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudComposite( NoeudComposite* noeud )
///
/// �criture d'un noeud composite.
///
/// @param[in] NoeudComposite * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudComposite( NoeudComposite* noeud )
{
	if(!noeud->estEnregistrable())
		return;

	// Cr�er le noeud 
	XmlElement* elementNoeud = noeud->creerNoeudXML();//obtenirElement(noeud);

	if(racine_ == 0)
	{
		racine_ = elementNoeud;
		parent_ = racine_;
	}
	else
	{
        XMLUtils::LinkEndChild(parent_,elementNoeud);
	}

	XmlElement* ancienParent = parent_;
	parent_ = elementNoeud;
	for( unsigned int i = 0; i < noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
	}
	parent_ = ancienParent;
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// �criture d'un noeud repr�sentant un muret.
///
/// @param[in] NodeWallAbstract * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudMuret( NodeWallAbstract* noeud )
{
    visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudBut( NoeudBut* noeud )
///
/// �criture d'un noeud repr�sentant un but.
///
/// @param[in] NoeudBut * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudBut( NoeudBut* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// �criture d'un noeud repr�sentant un maillet.
///
/// @param[in] NoeudMaillet * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudPortail( NoeudPortail* noeud )
///
/// �criture d'un noeud repr�sentant un portail.
///
/// @param[in] NoeudPortail * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudPortail( NoeudPortail* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// �criture d'un noeud repr�sentant une rondelle.
///
/// @param[in] NoeudRondelle * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudTable( NoeudTable* noeud )
///
/// �criture d'un noeud repr�sentant une table.
///
/// @param[in] NoeudTable * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudTable( NoeudTable* noeud )
{
	visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::visiterNoeudPoint( NoeudPoint* noeud )
///
/// �criture d'un noeud repr�sentant un point. Ne fais rien puisque la table s'en occupe
///
/// @param[in] NoeudPoint * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudPoint( NoeudPoint* noeud )
{
	visiterNoeudComposite(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurEcrireXML::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// �criture d'un noeud repr�sentant un accel�rateur.
///
/// @param[in] NoeudAccelerateur * noeud : noeud � v�rifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	visiterNoeudAbstrait(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* VisiteurEcrireXML::obtenirRacine()
///
/// Retourne la racine du sous-arbre
///
///
/// @return XmlElement* : racine du sous-arbre courant
///
////////////////////////////////////////////////////////////////////////
XmlElement* VisiteurEcrireXML::obtenirRacine()
{
	return racine_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEcrireXML::reinitialiserRacine()
///
/// R�initialise la racine � 0.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::reinitialiserRacine()
{
	racine_ = 0;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////