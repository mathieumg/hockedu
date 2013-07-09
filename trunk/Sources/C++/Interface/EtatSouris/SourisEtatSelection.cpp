//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatSelection.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatSelection.h"
#include "FacadeModele.h"
#include "VisiteurSelection.h"
#include "glew.h"
#include <GL\glu.h>
#include "Vue.h"
#include "NoeudTable.h"
#include "NoeudGroupe.h"
#include "Terrain.h"
#include "VisiteurSelectionOpenGL.h"
#include "CodesEvenements.h"
#include "GestionnaireModeles.h"

const ToucheClavier KeepCurrentSelectionModifier = VJAK_CONTROL;
const ToucheClavier UnselectModifier = VJAK_SHIFT;

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatSelection::SourisEtatSelection(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatSelection::SourisEtatSelection(  ):mFlags(0)
{
    mInitialMousePos = NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatSelection::~SourisEtatSelection(void)
///
/// destructeur
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatSelection::~SourisEtatSelection(void)
{
    FacadeModele::getInstance()->modifierVariableZoomElastique(false);
}

/// Comportement lorsqu'une touche du clavier est enfoncée.
void SourisEtatSelection::toucheEnfoncee( EvenementClavier& evenementClavier )
{
}

/// Comportement lorsqu'une touche du clavier est relâchée.
void SourisEtatSelection::toucheRelachee( EvenementClavier& evenementClavier )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatSelection::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatSelection::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
    {
        mFlags.SetFlag(true,SELECTION_LEFT_CLICK_PRESSED);
        mInitialMousePos = evenementSouris.obtenirPosition();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatSelection::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatSelection::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
    {
        auto field = FacadeModele::getInstance()->getEditionField();
        const std::set<NoeudAbstrait*>& selection = field->getSelectedNodes();
        /// old copy
        std::set<NoeudAbstrait*> oldSelection = selection;

        bool keepSelection = EventManager::IsKeyPressed(KeepCurrentSelectionModifier);
        if(!keepSelection)
            FacadeModele::getInstance()->selectionArbre(false);

        ConteneurIdNoeuds liste;
        GetOpenGLSelectedList(liste);

        int selected = 0;
        ConteneurIdNoeuds::const_iterator iter = liste.begin();
        for(; iter != liste.end(); iter++)
        {
            VisiteurSelectionOpenGL visiteur(&iter->second,keepSelection);
            NoeudGroupe* groupe = field->getTable()->obtenirGroupe(iter->first);
            if(groupe)
            {
                groupe->acceptVisitor(visiteur);
            }
            else
                FacadeModele::getInstance()->acceptVisitor(visiteur);

            selected += visiteur.getNbSelected();
        }

        mFlags.SetFlag(false,SELECTION_UNDO_SENT);
        if(oldSelection.size() == selection.size())
        {
            STL_ITERATE(selection,it)
            {
                if(find(oldSelection.begin(),oldSelection.end(),*it) == oldSelection.end())
                {
                    mFlags.SetFlag(true,SELECTION_UNDO_SENT);
                    break;
                }
            }
        }
        else
        {
            mFlags.SetFlag(true,SELECTION_UNDO_SENT);
        }

        if(mFlags.IsFlagSet(SELECTION_UNDO_SENT))
        {
            field->pushUndoState();
        }

        mFlags.SetFlag(false,SELECTION_LEFT_CLICK_PRESSED);
        mFlags.SetFlag(false,SELECTION_MOUSE_DRAGGING);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatSelection::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatSelection::sourisDeplacee( EvenementSouris& evenementSouris )
{
    
    // Mise à jour de la position
    auto mousePos = evenementSouris.obtenirPosition();
    if(mFlags.IsFlagSet(SELECTION_MOUSE_DRAGGING))
    {
        FacadeModele::getInstance()->modifierVariableZoomElastique( mFlags.IsFlagSet(SELECTION_LEFT_CLICK_PRESSED),mInitialMousePos,mousePos );
    }
    
    if(mFlags.IsFlagSet(SELECTION_LEFT_CLICK_PRESSED) && (mInitialMousePos-mousePos).norme2() > 25)
    {
        mFlags.SetFlag(true,SELECTION_MOUSE_DRAGGING);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NomEtatSouris SourisEtatSelection::obtenirNomEtatSouris()
///
/// Retourne l'état courant
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatSelection::obtenirNomEtatSouris()
{
    return ETAT_SOURIS_SELECTION;
}

void SourisEtatSelection::doubleClickEvent( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
    {

        ConteneurIdNoeuds liste;
        GetOpenGLSelectedList(liste);

        auto field = FacadeModele::getInstance()->getEditionField();

        bool keepSelection = EventManager::IsKeyPressed(KeepCurrentSelectionModifier);
        bool unselect = EventManager::IsKeyPressed(UnselectModifier);

        // assume a left click has been done therefor changing the selection
        if(keepSelection)
        {
            if(mFlags.IsFlagSet(SELECTION_UNDO_SENT))
                field->undoModification();
        }
        else
        {
            field->setTableItemsSelection(false);
        }

        auto table = field->getTable();
        if(table)
        {
            STL_ITERATE(liste,it)
            {
                auto type = GestionnaireModeles::obtenirInstance()->obtenirNameFromTypeId(it->first);
                auto group = table->obtenirGroupe(type);
                if(group)
                {
                    for(unsigned int i=0; i<group->childCount(); ++i)
                    {
                        NoeudAbstrait* n = (group->find(i));
                        n->setSelection(!unselect);
                    }
                }
            }
        }
        field->pushUndoState();
        mFlags.SetFlag(true,SELECTION_UNDO_SENT);
        mFlags.SetFlag(false,SELECTION_LEFT_CLICK_PRESSED);
        mFlags.SetFlag(false,SELECTION_MOUSE_DRAGGING);
    }
}

void SourisEtatSelection::GetOpenGLSelectedList( ConteneurIdNoeuds &liste )
{
    const Vecteur2i& mousePos = EventManager::mMouseScreenPos;
    Vecteur2i centreSelection(mousePos);
    Vecteur2i tailleSelection(1,1);
    // On s'assure de desactiver le rectangle elastique
    FacadeModele::getInstance()->modifierVariableZoomElastique(false);

    if(mFlags.IsFlagSet(SELECTION_MOUSE_DRAGGING))
    {
        Vecteur2i d( (mInitialMousePos-mousePos) );
        centreSelection = mousePos+(d/2);
        tailleSelection[VX] = abs(d[VX]);
        tailleSelection[VY] = abs(d[VY]);
    }

    // Pre selelection
    glMatrixMode( GL_PROJECTION );
    //glPushMatrix();
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    gluPickMatrix(centreSelection[VX],viewport[3]-centreSelection[VY],tailleSelection[VX],tailleSelection[VY],viewport);

    FacadeModele::getInstance()->obtenirVue()->appliquerProjection();

    const GLsizei taille = 1000;
    GLuint tampon[taille];			

    //glInitNames();
    glSelectBuffer( taille, tampon );
    glRenderMode(GL_SELECT);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    FacadeModele::getInstance()->obtenirVue()->appliquerCamera();
    FacadeModele::getInstance()->getEditionField()->renderField();

    int nbObjets = glRenderMode(GL_RENDER);

    GLuint* ptr = tampon;


    float zMinimum = 100000;
    for(int i=0; i<nbObjets; i++)
    {
        bool ajouter = false;
        int nbNoms = (*ptr);
        ptr++;
        float zMin = (float)(*ptr)/0xFFFFFFFF;
        ptr++;
        float zMax = (float)(*ptr)/0xFFFFFFFF;
        ptr++;

        if(nbNoms == 2)
        {
            // Si drag est a true, on ne ce souci meme pas du min en z
            if(!mFlags.IsFlagSet(SELECTION_MOUSE_DRAGGING))
            {
                zMinimum = zMin;
                liste.clear();
                ajouter = true;
            }
            else if(zMin<zMinimum)
            {
                ajouter = true;
            }
        }
        unsigned int idConcret;
        for(int j=0; j<nbNoms; j++)
        {
            if(ajouter)
            {
                // j%2 == 1 donc j == 1 dans notre cas
                if(j&1)
                {
                    unsigned int typeId = (*ptr);
                    if(liste.find(typeId) == liste.end())
                    {
                        liste[typeId] = IdNoeuds();
                    }
                    liste[typeId].insert(idConcret);
                }
                else
                {
                    idConcret = (*ptr);
                }
            }
            ptr++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



