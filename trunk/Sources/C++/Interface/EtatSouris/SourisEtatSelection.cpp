//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatSelection.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
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
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
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
	FacadeModele::obtenirInstance()->modifierVariableZoomElastique(false);
}

/// Comportement lorsqu'une touche du clavier est enfoncée.
void SourisEtatSelection::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ctrlEnfoncee_ = true;
	}
}

/// Comportement lorsqu'une touche du clavier est relâchée.
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
/// Comportement lorsqu'un bouton de la souris est enfoncé
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
		if(!ctrlEnfoncee_)
			FacadeModele::obtenirInstance()->selectionArbre(false);

		//if(drag_)
		{
			// DRAG
			//FacadeModele::obtenirInstance()->modifierVariableZoomElastique(false);

			//Vecteur3 posCurVirt, posPrecedentVirt;
			//FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(positionEnfoncee_[VX], positionEnfoncee_[VY], posCurVirt);
			//FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(positionPrecedente_[VX], positionPrecedente_[VY], posPrecedentVirt);

// 			glMatrixMode( GL_PROJECTION );
// 			glLoadIdentity();
// 			GLint viewport[4];
// 			glGetIntegerv(GL_VIEWPORT,viewport);
// 			gluPickMatrix(lastx,viewport[3]-lasty,50,50,viewport);


// 			VisiteurSelection visiteur(positionEnfonce, positionPrecedente);
// 			FacadeModele::obtenirInstance()->visiterArbre(&visiteur);
//			visiteur.faireSelection();
		}
		//else
		{
			Vecteur2i centreSelection(positionPrecedente_);
			Vecteur2i tailleSelection(1,1);
			// On s'assure de desactiver le rectangle elastique
			FacadeModele::obtenirInstance()->modifierVariableZoomElastique(false);
			
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

			FacadeModele::obtenirInstance()->obtenirVue()->appliquerProjection();

			const GLsizei taille = 1000;
			GLuint tampon[taille];			
			
			//glInitNames();
			glSelectBuffer( taille, tampon );
			glRenderMode(GL_SELECT);

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			FacadeModele::obtenirInstance()->obtenirVue()->appliquerCamera();
			FacadeModele::obtenirInstance()->afficherBase();

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
			
			ConteneurIdNoeuds::const_iterator iter = liste.begin();
			for(; iter != liste.end(); iter++)
			{
				VisiteurSelectionOpenGL visiteur(&iter->second,ctrlEnfoncee_);
				NoeudGroupe* groupe = FacadeModele::obtenirInstance()->obtenirTerrain()->obtenirTable()->obtenirGroupe(iter->first);
				if(groupe)
				{
					groupe->accueillirVisiteurNoeud(visiteur);
				}
				else
					FacadeModele::obtenirInstance()->visiterArbre(&visiteur);
			}



			//glMatrixMode( GL_PROJECTION );
		//	glPopMatrix();
		//	glMatrixMode( GL_MODELVIEW );


// 			VisiteurSelection visiteur(positionClic, positionClic);
// 			FacadeModele::obtenirInstance()->visiterArbre(&visiteur);
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
	positionPrecedente_ = evenementSouris.obtenirPosition();
	if(drag_)
	{
		FacadeModele::obtenirInstance()->modifierVariableZoomElastique( estEnfoncee_,positionEnfoncee_,positionPrecedente_ );
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

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



