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
/// Écriture d'un noeud abstrait.
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	if(!noeud->isRecordable())
		return;

	// Créer le noeud 
	XmlElement* elementNoeud = noeud->createXmlNode();

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
/// Écriture d'un noeud composite.
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEcrireXML::visiterNoeudComposite( NoeudComposite* noeud )
{
    XmlElement* elementNoeud = parent_;

    // permet d'enregistrer les enfants d'un noeud qui n'est pas enregistrable
	if(noeud->isRecordable())
    {
        // Créer le noeud 
        elementNoeud = noeud->createXmlNode();

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

	XmlElement* ancienParent = parent_;
	parent_ = elementNoeud;


	for( unsigned int i = 0; i < noeud->childCount(); i++)
	{
		noeud->find(i)->acceptVisitor(*this);
	}


	parent_ = ancienParent;
	
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
/// Réinitialise la racine à 0.
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