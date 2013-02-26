//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutControlPointMutable.cpp
/// @author Mathieu Parent
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjoutControlPointMutable.h"
#include "FacadeModele.h"
#include "NodeControlPoint.h"
#include "SourisEtatAjout.h"
#include "Terrain.h"
#include "ControlPointMutableAbstract.h"

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutControlPointMutable::SourisEtatAjoutControlPointMutable()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage et initialise les attributs
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutControlPointMutable::SourisEtatAjoutControlPointMutable(Terrain* pField, const std::string& typeNoeud):
    SourisEtatAjout(pField,typeNoeud),mCurrentPoint(NULL)
{
    if(!createNextControlPoint())
    {
        checkf(0,"Incapacité de créer le noeud ou de selectionné un point de control");
        if(noeud_)
        {
            if(mField)
            {
                mField->retirerNoeudTemp(noeud_);
            }
            delete noeud_;
            noeud_ = NULL;
        }
        if(mCurrentPoint)
        {
            delete mCurrentPoint;
            mCurrentPoint = NULL;
        }
    }
}

    
////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutControlPointMutable::~SourisEtatAjoutControlPointMutable(void)
///
/// Ce destructeur supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutControlPointMutable::~SourisEtatAjoutControlPointMutable(void)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutControlPointMutable::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in]  evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutControlPointMutable::sourisEnfoncee( EvenementSouris& evenementSouris )
{ 

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutControlPointMutable::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in]  evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutControlPointMutable::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(noeud_ && mField && evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
    {
        if(!mField->IsNodeAtValidEditionPosition(noeud_,false))
        {   
            showInvalidText(true);
            return;
        }
        showInvalidText(false);


        if(!createNextControlPoint())
        {
            mField->transfererNoeud(noeud_);
            genererNoeud();
            if(!createNextControlPoint())
            {
                if(noeud_)
                {
                    mField->retirerNoeudTemp(noeud_);
                    delete noeud_;
                    noeud_ = NULL;
                }
                if(mCurrentPoint)
                {
                    delete mCurrentPoint;
                    mCurrentPoint = NULL;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutControlPointMutable::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est déplacée
///
/// @param[in]  evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutControlPointMutable::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if(noeud_ && mField && mCurrentPoint)
    {
        auto positionSouris = evenementSouris.obtenirPosition();
        Vecteur3 positionVirtuelle;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(positionSouris[VX],positionSouris[VY],positionVirtuelle);

        mCurrentPoint->setPosition(positionVirtuelle);

        // Appel pour faire le surlignement
        mField->IsNodeAtValidEditionPosition(noeud_,true);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutControlPointMutable::createNextControlPoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool SourisEtatAjoutControlPointMutable::createNextControlPoint()
{
    ControlPointMutableAbstract* controlNode = dynamic_cast<ControlPointMutableAbstract*>(noeud_);
    if(controlNode)
    {
        if(controlNode->getMaxControlPoints() == 0)
        {
            if(noeud_->GetTerrain())
            {
                noeud_->GetTerrain()->retirerNoeudTemp(noeud_);
                delete noeud_;
                noeud_  = NULL;
                return false;
            }
            delete noeud_; 
            return false;
        }
        if(controlNode->getNBControlPoint() < controlNode->getMaxControlPoints())
        {
            Vecteur3 pos;
            if(mCurrentPoint)
            {
                pos = mCurrentPoint->getPosition();
            }
            mCurrentPoint = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);
            mCurrentPoint->setPosition(pos);
            bool res = noeud_->ajouter(mCurrentPoint);
            checkf(res);
            return res;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutControlPointMutable::genererNoeud()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutControlPointMutable::genererNoeud()
{
    Super::genererNoeud();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutControlPointMutable::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// /*Description*/
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutControlPointMutable::toucheEnfoncee( EvenementClavier& evenementClavier )
{
    if(evenementClavier.obtenirTouche() == VJAK_ENTER)
    {
        ControlPointMutableAbstract* controlNode = dynamic_cast<ControlPointMutableAbstract*>(noeud_);
        if(controlNode && controlNode->getNBControlPoint() > 3 )
        {
            noeud_->detacherEnfant(mCurrentPoint);
            if(FacadeModele::getInstance()->validerPositionNoeud(noeud_))
            {
                delete mCurrentPoint;
                noeud_->GetTerrain()->transfererNoeud(noeud_);
                genererNoeud();
                if(!createNextControlPoint())
                {
                    if(noeud_)
                    {
                        if(noeud_->GetTerrain())
                        {
                            noeud_->GetTerrain()->retirerNoeudTemp(noeud_);
                        }
                        delete noeud_;
                        noeud_ = NULL;
                    }
                    if(mCurrentPoint)
                    {
                        delete mCurrentPoint;
                        mCurrentPoint = NULL;
                    }
                }
            }
            else
            {
                noeud_->ajouter(mCurrentPoint);
            }
        }
    }
}

#ifdef MIKE_DEBUG
PRAGMA_ENABLE_OPTIMIZATION
#endif


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


