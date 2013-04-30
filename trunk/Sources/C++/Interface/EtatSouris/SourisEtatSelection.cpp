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

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatSelection::SourisEtatSelection(  )
///
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatSelection::SourisEtatSelection(  )
{
	estEnfoncee_ = false;
	positionPrecedente_ = NULL;
	positionEnfoncee_ = NULL;
	drag_ = false;
	ctrlEnfoncee_ = false;
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

/// Comportement lorsqu'une touche du clavier est enfonc�e.
void SourisEtatSelection::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ctrlEnfoncee_ = true;
	}
}

/// Comportement lorsqu'une touche du clavier est rel�ch�e.
void SourisEtatSelection::toucheRelachee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ctrlEnfoncee_ = false;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatSelection::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfonc�
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatSelection::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	if(!estEnfoncee_ && evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		estEnfoncee_ = true;
		positionEnfoncee_ = evenementSouris.obtenirPosition();
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatSelection::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e
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
        FacadeModele::getInstance()->modifierVariableZoomElastique(false);
        Vecteur3 avant,apres;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(positionEnfoncee_[0],positionEnfoncee_[1],avant);
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[0],evenementSouris.obtenirPosition()[1],apres);
        FacadeModele::getInstance()->getEditionField()->selectNodes((Vecteur2&)avant,(Vecteur2&)apres,ctrlEnfoncee_);
        estEnfoncee_ = false;
        return;

        if(!ctrlEnfoncee_)
            FacadeModele::getInstance()->selectionArbre(false);

//		if(drag_)
		{
			// DRAG
//             FacadeModele::getInstance()->modifierVariableZoomElastique(false);
// 
//             Vecteur3 posCurVirt, posPrecedentVirt;
//             FacadeModele::getInstance()->convertirClotureAVirtuelle(positionEnfoncee_[VX], positionEnfoncee_[VY], posCurVirt);
//             FacadeModele::getInstance()->convertirClotureAVirtuelle(positionPrecedente_[VX], positionPrecedente_[VY], posPrecedentVirt);
// 
// 			glMatrixMode( GL_PROJECTION );
// 			glLoadIdentity();
// 			GLint viewport[4];
// 			glGetIntegerv(GL_VIEWPORT,viewport);
// 			gluPickMatrix(lastx,viewport[3]-lasty,50,50,viewport);
// 
// 
// 			VisiteurSelection visiteur(positionEnfonce, positionPrecedente);
// 			FacadeModele::getInstance()->visiterArbre(&visiteur);
// 			visiteur.faireSelection();
		}
//		else
		{
			Vecteur2i centreSelection(positionPrecedente_);
			Vecteur2i tailleSelection(1,1);
			// On s'assure de desactiver le rectangle elastique
			FacadeModele::getInstance()->modifierVariableZoomElastique(false);
			
			if(drag_)
			{

				Vecteur2i d( (positionEnfoncee_-positionPrecedente_) );
				centreSelection = positionPrecedente_+(d/2);
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
			
			ConteneurIdNoeuds liste;

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

				if(nbNoms == 2 && (drag_ || zMin<zMinimum ))
				{
					// Si drag est a true, on ne ce souci meme pas du min en z
					if(!drag_)
					{
						zMinimum = zMin;
						liste.clear();
					}
					ajouter = true;
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

						// j == 0 on assigne le premier element
						//(j&1 ? liste.back().second : liste.back().first ) = (*ptr);
					}
					ptr++;
				}
			}
            auto field = FacadeModele::getInstance()->getEditionField();

            const std::set<NoeudAbstrait*>& selection = field->getSelectedNodes();
            /// old copy
            std::set<NoeudAbstrait*> oldSelection = selection;

            int selected = 0;
			ConteneurIdNoeuds::const_iterator iter = liste.begin();
			for(; iter != liste.end(); iter++)
			{
				VisiteurSelectionOpenGL visiteur(&iter->second,ctrlEnfoncee_);
				NoeudGroupe* groupe = field->getTable()->obtenirGroupe(iter->first);
				if(groupe)
				{
					groupe->acceptVisitor(visiteur);
				}
				else
					FacadeModele::getInstance()->acceptVisitor(visiteur);

                selected += visiteur.getNbSelected();
			}

            bool pushUndo = false;
            if(oldSelection.size() == selection.size())
            {
                STL_ITERATE(selection,it)
                {
                    if(find(oldSelection.begin(),oldSelection.end(),*it) == oldSelection.end())
                    {
                        pushUndo = true;
                        break;
                    }
                }
            }
            else
            {
                pushUndo = true;
            }

            if(pushUndo)
            {
                field->pushUndoState();
            }


			//glMatrixMode( GL_PROJECTION );
		//	glPopMatrix();
		//	glMatrixMode( GL_MODELVIEW );


// 			VisiteurSelection visiteur(positionClic, positionClic);
// 			FacadeModele::getInstance()->visiterArbre(&visiteur);
			//visiteur.faireSelection();
		}

	}
	estEnfoncee_ = false;
	drag_ = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatSelection::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplac�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatSelection::sourisDeplacee( EvenementSouris& evenementSouris )
{
	
	// Mise � jour de la position
	positionPrecedente_ = evenementSouris.obtenirPosition();
	if(drag_)
	{
		FacadeModele::getInstance()->modifierVariableZoomElastique( estEnfoncee_,positionEnfoncee_,positionPrecedente_ );
	}
	
	if(estEnfoncee_ && (abs(positionEnfoncee_[VX]-positionPrecedente_[VX])>5 || abs(positionEnfoncee_[VY]-positionPrecedente_[VY])>5))
	{
		drag_ = true;
		rectangleElastiqueAncienCoin_ = positionEnfoncee_;
	}
		
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NomEtatSouris SourisEtatSelection::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant
///
///
/// @return L'�tat courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatSelection::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_SELECTION;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



