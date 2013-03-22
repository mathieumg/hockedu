

//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjout.cpp
/// @author Gabriel Couvrette
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjout.h"
#include "FacadeModele.h"
#include "Terrain.h"
#include "HUDTexte.h"
#include "GestionnaireHUD.h"
#include "HUDElement.h"
#include "NoeudAbstrait.h"
#include "RazerGameTree.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjout::SourisEtatAjout()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjout::SourisEtatAjout(Terrain* pField,const std::string& nomNoeudAjout): 
    noeud_(NULL), nom_(nomNoeudAjout), hudTextPosInvalide(0),mField(pField)
{
    hudTextPosInvalide = new HUDTexte("Position invalide !", Vecteur4f(1,0,0,1),std::string("game_over_big_text"));
    showInvalidText(false);
    hudTextPosInvalide->modifierPosition(0.45f,0.5f);

    //GestionnaireHUD::obtenirInstance()->obtenirRacine(RACINE_EDITION)->add(hudTextPosInvalide);
    genererNoeud();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::~SourisEtatAjoutMuret(void)
///
/// Ce destructeur supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjout::~SourisEtatAjout(void)
{
    if(mField && noeud_)
    {
        mField->retirerNoeudTemp(noeud_);

        // etre sur que le noeud a ce quil faut pour bien ce clean
        noeud_->setField(mField);
        delete noeud_;
    }
    //GestionnaireHUD::obtenirInstance()->obtenirRacine(RACINE_EDITION)->supprimerElement(hudTextPosInvalide);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::genererNoeud(std::string objet, std::string parent)
///
/// Crée un nouveau noeud et l'ajoute au parent
///
/// @param[in]  objet   : Nom représentant l'objet à ajouter
///             parent  : parent auquel ajouter le nouveau noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::genererNoeud()
{
    if(mField)
    {
        noeud_ =  mField->getLogicTree()->creerNoeud(nom_);
        if(noeud_)
        {
            mField->ajouterNoeudTemp(noeud_);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in]  evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in]  evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(mField && evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
    {
        if(noeud_ != 0)
        {
            showInvalidText(false);
            if(mField->IsNodeAtValidEditionPosition(noeud_,false))
            {
                mField->transfererNoeud(noeud_);
                genererNoeud();
                // La generation d'un noeud peut entrainer le noeud a etre invalide si on ne peut le creer
                if(noeud_)
                {
                    Vecteur2i position=evenementSouris.obtenirPosition();
                    Vecteur3 positionVirtuelle;
                    FacadeModele::getInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
                    noeud_->setPosition(positionVirtuelle);
                }
            }
            else
            {
                showInvalidText(true);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est déplacée
///
/// @param[in]  evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if(noeud_ && mField)
    {
        Vecteur2i position=evenementSouris.obtenirPosition();
        Vecteur3 positionVirtuelle;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
        noeud_->setPosition(positionVirtuelle);
        mField->IsNodeAtValidEditionPosition(noeud_,true);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::obtenirNomEtatSouris()
///
/// Retourne le nom associé avec l'état de la souris
///
/// @param[in]  void
///
/// @return (int )NomEtatSouris : nom correspondant à l'état
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatAjout::obtenirNomEtatSouris()
{
    if(nom_ == RazerGameUtilities::NOM_MAILLET)
        return ETAT_SOURIS_AJOUTER_MAILLET;
    if(nom_ == RazerGameUtilities::NOM_ACCELERATEUR)
        return ETAT_SOURIS_AJOUTER_ACCELERATEUR;
    if(nom_ == RazerGameUtilities::NOM_PORTAIL)
        return ETAT_SOURIS_AJOUTER_PORTAIL;
    if(nom_ == RazerGameUtilities::NOM_RONDELLE)
        return ETAT_SOURIS_AJOUTER_RONDELLE;
    if(nom_ == RazerGameUtilities::NOM_MURET)
        return ETAT_SOURIS_AJOUTER_MURET;
    if(nom_ == RazerGameUtilities::NAME_BONUS)
        return ETAT_SOURIS_AJOUTER_BONUS;
    return ETAT_SOURIS_INCONNU;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::showInvalidText( bool show )
///
/// /*Description*/
///
/// @param[in] bool show
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::showInvalidText( bool show ) const
{
    hudTextPosInvalide->modifierVisibilite(show);
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


