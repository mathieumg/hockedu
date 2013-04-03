////////////////////////////////////////////////////////////////////////////////////
/// @file ProjectionPerspective.cpp
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#if WIN32
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include <GL/glu.h>

#elif __APPLE__
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "glu.h"
#endif

#include "ProjectionPerspective.h"
#include "LignePointillee.h"
#include "Utilitaire.h"
#include <math.h>
#include <iostream>

namespace vue {

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn ProjectionPerspective::ProjectionPerspective(int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture, float zAvant,  float zArriere, float zoomInMax, float zoomOutMax, float incrementZoom, float xMinFenetre, float xMaxFenetre, float yMinFenetre, float yMaxFenetre)
	///
	/// Constructeur d'une projection perspective.  Ne fait qu'assigner les
	/// variables membres et ajuste ensuite le rapport d'aspect.
	///
	/// @param[in] xMinCloture   : coordonn�e minimale en @a x de la cl�ture.
	/// @param[in] xMaxCloture   : coordonn�e maximale en @a x de la cl�ture.
	/// @param[in] yMinCloture   : coordonn�e minimale en @a y de la cl�ture.
	/// @param[in] yMaxCloture   : coordonn�e maximale en @a y de la cl�ture.
	/// @param[in] zAvant        : distance du plan avant (en @a z).
	/// @param[in] zArriere      : distance du plan arri�re (en @a z).
	/// @param[in] zoomInMax     : facteur de zoom in maximal.
	/// @param[in] zoomOutMax    : facteur de zoom out maximal.
	/// @param[in] incrementZoom : distance du plan arri�re (en @a z).
	/// @param[in] xMinFenetre   : coordonn�e minimale en @a x de la fen�tre
	///                            virtuelle.
	/// @param[in] xMaxFenetre   : coordonn�e maximale en @a x de la fen�tre
	///                            virtuelle.
	/// @param[in] yMinFenetre   : coordonn�e minimale en @a y de la fen�tre
	///                            virtuelle.
	/// @param[in] yMaxFenetre   : coordonn�e maximale en @a y de la fen�tre
	///                            virtuelle.
	/// 
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////
	ProjectionPerspective::ProjectionPerspective(int xMinCloture, int xMaxCloture,
		int yMinCloture, int yMaxCloture,
		float zAvant,  float zArriere,
		float zoomInMax, float zoomOutMax,
		float incrementZoom,
		float xMinFenetre, float xMaxFenetre,
		float yMinFenetre, float yMaxFenetre) :
	Projection(xMinCloture, xMaxCloture, yMinCloture, yMaxCloture,
		zAvant, zArriere,
		zoomInMax, zoomOutMax, incrementZoom, false) ,
		xMinFenetre_(xMinFenetre),
		xMaxFenetre_(xMaxFenetre),
		yMinFenetre_(yMinFenetre),
		yMaxFenetre_(yMaxFenetre)
	{
		angleOuverture_ = 360.0f*(float)atan((yMaxFenetre-yMinFenetre)/(2.0f*zAvant))/3.141592f;
		ajusterRapportAspect();
		mettreAJourProjection();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::zoomerIn()
	///
	/// Permet de faire un zoom in selon l'incr�ment de zoom.
	/// 
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::zoomerIn(float increment) 
	{
		/*float incrementZoom = increment;
		if(increment == 0)
		{
			incrementZoom = incrementZoom_;
		}
		//Ajouter valeur arbitraire

		float	deltaX = (xMaxFenetre_- xMinFenetre_),
				deltaY = (yMaxFenetre_- yMinFenetre_);
		if( ( deltaX > zoomInMax_) &&  ( deltaY > zoomInMax_ ) )
		{
			xMinFenetre_ += incrementZoom*deltaX*0.05;
			xMaxFenetre_ -= incrementZoom*deltaX*0.05;
			yMinFenetre_ += incrementZoom*deltaY*0.05;
			yMaxFenetre_ -= incrementZoom*deltaY*0.05;
			
		}
		ajusterRapportAspect(); // Ajoute
		mettreAJourProjection();*/
		/*float incrementZoom = increment;
		xMinFenetre_ += incrementZoom*0.05;
		xMaxFenetre_ -= incrementZoom*0.05;
		yMinFenetre_ += incrementZoom*0.05;
		yMaxFenetre_ -= incrementZoom*0.05;*/


		zAvant_-=increment;
		
		mettreAJourProjection();


	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::zoomerOut()
	///
	/// Permet de faire un zoom out selon l'incr�ment de zoom.
	///
	/// @return Aucune.
	///
	//////////////////////////////////////////////////////////////////////// 
	void ProjectionPerspective::zoomerOut(float increment) 
	{
		/*float incrementZoom = increment;
		float	deltaX = (xMaxFenetre_- xMinFenetre_),
				deltaY = (yMaxFenetre_- yMinFenetre_);
		if(increment == 0)
		{
			incrementZoom = incrementZoom_;
		}
		if(( deltaX < zoomOutMax_)  && (deltaY < zoomOutMax_) )
		{
			
			xMinFenetre_ -= incrementZoom*deltaX*0.05;
			xMaxFenetre_ += incrementZoom*deltaX*0.05;
			yMinFenetre_ -= incrementZoom*deltaY*0.05;
			yMaxFenetre_ += incrementZoom*deltaY*0.05;
		}
		ajusterRapportAspect(); // Ajoute
		mettreAJourProjection();*/

		/*float incrementZoom = increment;
		xMinFenetre_ -= incrementZoom*0.05;
		xMaxFenetre_ += incrementZoom*0.05;
		yMinFenetre_ -= incrementZoom*0.05;
		yMaxFenetre_ += incrementZoom*0.05;*/

		zAvant_+=increment;
		
		mettreAJourProjection();
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::redimensionnerFenetre( const Vecteur2i& coinMin, const Vecteur2i& coinMax )
	///
	/// Permet d'ajuster les coordonn�es de la fen�tre virtuelle en fonction
	/// d'un redimensionnement de la fen�tre.
	///
	/// L'agrandissement de la fen�tre virtuelle est proportionnel �
	/// l'agrandissement de la clot�re afin que les objets gardent la m�me
	/// grandeur apparente lorsque la fen�tre est redimensionn�e.
	///
	/// @param[in]  coinMin : Coin contenant les coordonn�es minimales de la
	///                       nouvelle cl�ture
	/// @param[in]  coinMax : Coin contenant les coordonn�es maximales de la
	///                       nouvelle cl�ture
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::redimensionnerFenetre( const Vecteur2i& coinMin,
		const Vecteur2i& coinMax )
	{
		/*if(coinMax[VX] > 10 && coinMax[VY] > 10)
		{
			float aspectRatio = (float)xMaxCloture_/(float)yMaxCloture_;
			xMinCloture_ = coinMin[VX];
			yMinCloture_ = coinMin[VY];
			xMaxCloture_ = coinMax[VX];
			yMaxCloture_ = coinMax[VY];

			mettreAJourCloture();

			if(yMaxCloture_ > xMaxCloture_)
			{
				yMinFenetre_ = xMinFenetre_ / aspectRatio;
				yMaxFenetre_ = xMaxFenetre_ / aspectRatio;
			}
			else if ( xMaxCloture_ > yMaxCloture_)
			{
				xMinFenetre_ = yMinFenetre_ * aspectRatio;
				xMaxFenetre_ = yMaxFenetre_ * aspectRatio;
			}
// 			else if(yMaxCloture_ < xMaxCloture_)
// 			{
// 				yMinFenetre_ = xMinFenetre_ * aspectRatio;
// 				yMaxFenetre_ = xMaxFenetre_ * aspectRatio;
// 			}
// 			else if ( xMaxCloture_ < yMaxCloture_)
// 			{
// 				xMinFenetre_ = yMinFenetre_ / aspectRatio;
// 				xMaxFenetre_ = yMaxFenetre_ / aspectRatio;
// 			}
			
			ajusterRapportAspect();
			mettreAJourProjection();
			
		}*/
		
// 		float aspect = (xMaxFenetre_-xMinFenetre_)/(yMaxFenetre_-yMinFenetre_);
// 		xMinFenetre_-=10;
// 		xMaxFenetre_+=10;
// 		yMinFenetre_-=10;
// 		yMaxFenetre_+=10;
		

		xMinCloture_ = coinMin[VX];
		yMinCloture_ = coinMin[VY];
		xMaxCloture_ = coinMax[VX];
		yMaxCloture_ = coinMax[VY];
		mettreAJourCloture();
		ajusterRapportAspect();
		mettreAJourProjection();

		//std::cout << "Cloture: " << xMaxCloture_ << std::endl << yMaxCloture_ << std::endl;
		//std::cout << "Fenetre: " << xMinFenetre_ << " " << xMaxFenetre_ << std::endl << yMinFenetre_ << " " << yMaxFenetre_ << std::endl << std::endl;

	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::appliquer() const
	///
	/// Cette fonction permet d'appliquer la fen�tre virtuelle en appelant les
	/// fonctions d'OpenGL selon le type de projection et les propri�t�s de la
	/// fen�tre.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::appliquer() const
	{
		
		gluPerspective(angleOuverture_, (xMaxFenetre_-xMinFenetre_)/(yMaxFenetre_-yMinFenetre_),1.0, zArriere_ );
		
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::zoomerIn( const Vecteur2i& coin1, const Vecteur2i& coin2)
	///
	/// Permet de faire un zoom in sur un rectangle d�termin� par deux coins.
	/// Ainsi la r�gion d�limit�e par le rectangle deviendra la fen�tre
	/// virtuelle.  La fen�tre r�sultante est ajust�e pour respecter le rapport
	/// d'aspect.
	///
	/// @param[in]  coin1 : Le premier coin du rectangle.
	/// @param[in]  coin2 : Le deuxi�me coin du rectangle.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::zoomerIn( const Vecteur2i& coin1, const Vecteur2i& coin2 ) 
	{
		// A IMPLEMENTER
		/*

		Vecteur2 coin1Virtuelle = convertirClotureAVirtuelle(coin1), coin2Virtuelle = convertirClotureAVirtuelle(coin2);
		float	nouveauXMin = min(coin1Virtuelle[VX],coin2Virtuelle[VX]),
				nouveauXMax = max(coin1Virtuelle[VX],coin2Virtuelle[VX]),
				nouveauYMin = min(coin1Virtuelle[VY],coin2Virtuelle[VY]),
				nouveauYMax = max(coin1Virtuelle[VY],coin2Virtuelle[VY]);

		float longueurRectangleX = nouveauXMax - nouveauXMin;
		float centreX = longueurRectangleX/2.0+nouveauXMin;
		float longueurRectangleY = nouveauYMax - nouveauYMin;
		float centreY = longueurRectangleY/2.0+nouveauYMin;
		
		if(centreX < -ZoneEdition_old::LIMITE_EXT_LONGUEUR-50 || centreX > ZoneEdition_old::LIMITE_EXT_LONGUEUR+50 || centreY <-ZoneEdition_old::LIMITE_EXT_LARGEUR-50 || centreY > ZoneEdition_old::LIMITE_EXT_LARGEUR+50)
			return;


		if( longueurRectangleX < zoomInMax_)
		{
			longueurRectangleX = zoomInMax_;
		}
		xMinFenetre_ = centreX-longueurRectangleX/2.0;
		xMaxFenetre_ = centreX+longueurRectangleX/2.0;

		
		if( longueurRectangleY < zoomInMax_)
		{
			longueurRectangleY = zoomInMax_;
		}
		yMinFenetre_ = centreY-longueurRectangleY/2.0;
		yMaxFenetre_ = centreY+longueurRectangleY/2.0;
		*/
		//Ajustement du rapport en augmentant le c�t� le plus court, ainsi on est certain de voir tout ce qui a �t� s�lectionn� par le rectangle �lastique
		ajusterRapportAspect(false);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::zoomerOut( const Vecteur2i& coin1, const Vecteur2i& coin2)
	///
	/// Permet de faire un zoom out sur un rectangle d�limit� par deux coins.
	/// Ainsi la fen�tre virtuelle avant le zoom sera plac�e � l'interieur de
	/// la r�gion d�limit� par le rectangle.  La fen�tre r�sultante est ajust�e
	/// pour respecter le rapport d'aspect.
	///
	/// @param[in]  coin1 : Le premier coin du rectangle.
	/// @param[in]  coin2 : Le deuxi�me coin du rectangle.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::zoomerOut( const Vecteur2i& coin1, const Vecteur2i& coin2 ) 
	{
		// A IMPLEMENTER
 		/*Vecteur2 coin1Virtuelle = convertirClotureAVirtuelle(coin1), coin2Virtuelle = convertirClotureAVirtuelle(coin2);
		float	nouveauXMin = min(coin1Virtuelle[VX],coin2Virtuelle[VX]),
			nouveauXMax = max(coin1Virtuelle[VX],coin2Virtuelle[VX]),
			nouveauYMin = min(coin1Virtuelle[VY],coin2Virtuelle[VY]),
			nouveauYMax = max(coin1Virtuelle[VY],coin2Virtuelle[VY]);

		Vecteur2 lDepart = obtenirDimensionFenetre();
		Vecteur2 lFin;
		float lDessinerX = nouveauXMax - nouveauXMin;
		lFin[VX] = lDepart[VX]*lDepart[VX]/lDessinerX;
		float centreX = (xMinFenetre_+xMaxFenetre_)/2.0;
		float lDessinerY = nouveauYMax - nouveauYMin;
		lFin[VY] = lDepart[VY]*lDepart[VY]/lDessinerY;
		float centreY = (yMinFenetre_+yMaxFenetre_)/2.0;


		if( lFin[VX] > zoomOutMax_)
		{
			lFin[VX] = zoomOutMax_;
		}
		float variation = (lFin[VX]-lDepart[VX])/2.0;
		xMinFenetre_ = centreX-(lFin[VX]/2.0);
		xMaxFenetre_ = centreX+(lFin[VX]/2.0);

		
		if( lFin[VY] > zoomOutMax_)
		{
			lFin[VY] = zoomOutMax_;
		}
		variation = (lFin[VY]-lDepart[VY])/2.0;
		yMinFenetre_ = centreY-(lFin[VY]/2.0);
		yMaxFenetre_ = centreY+(lFin[VY]/2.0);
		*/
		ajusterRapportAspect();
		/*
		Vecteur2i pointCentre((int)((coin1[VX]+coin2[VX])/2.0),(int)((coin1[VY]+coin2[VY])/2.0));
		centrerSurPoint(pointCentre);*/
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::translater( float deplacementX, float deplacementY )
	///
	/// Permet de d�placer la fen�tre virtuelle en @a x et en @a y.  Les
	/// d�placement doivent �tre exprim�s en proportion de la fen�tre virtuelle.
	///
	/// @param[in]  deplacementX : le d�placement en @a x.
	/// @param[in]  deplacementY : le d�placement en @a y.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::translater( float deplacementX, float deplacementY )
	{
		/*if( (deplacementX < 0 || xMaxFenetre_-deplacementX > - ZoneEdition_old::LIMITE_EXT_LONGUEUR+50) && (deplacementX > 0 || xMinFenetre_-deplacementX < ZoneEdition_old::LIMITE_EXT_LONGUEUR-50))
		{
			xMinFenetre_ -= deplacementX;
			xMaxFenetre_ -= deplacementX;
		}
		if( yMaxFenetre_+deplacementY > - ZoneEdition_old::LIMITE_EXT_LARGEUR+50 && yMinFenetre_+deplacementY < ZoneEdition_old::LIMITE_EXT_LARGEUR-50)
		{
			yMaxFenetre_ += deplacementY;
			yMinFenetre_ += deplacementY;
		}*/
		


		mettreAJourProjection();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::translater(const Vecteur2i& deplacement)
	///
	/// Permet de translater la fen�tre virtuelle en fonction d'un d�placement
	/// en coordonn�es de cl�ture.
	///
	/// @param[in]  deplacement : Le d�placement en coordonn�es de cl�ture.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::translater( const Vecteur2i& deplacement )
	{
		////////// ATTENTION au changement de logique, CentrerSurPoint est d�pendant � son comportement actuel
		Vecteur2 deplacementVirtuel = convertirDistanceClotureAVirtuelle(deplacement);
		translater(deplacementVirtuel[VX],deplacementVirtuel[VY]);
		mettreAJourProjection();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::centrerSurPoint( const Vecteur2i& pointCentre )
	///
	/// Permet de centrer la fen�tre virtuelle sur un point de l'�cran. Le
	/// point de l'�cran est convertit en coordonn�es virtuelles et il devient
	/// le centre de la fen�tre virtuelle.
	///
	/// @param[in]  pointCentre : Le point sur lequel on doit centrer.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::centrerSurPoint( const Vecteur2i& pointCentre ) 
	{
		/*Vecteur2i lCloture = obtenirDimensionCloture();
		Vecteur2i newCentre(pointCentre[VX]-(lCloture[VX]>>1), pointCentre[VY]-(lCloture[VY]>>1));
		Vecteur2 centre = convertirDistanceClotureAVirtuelle(newCentre);
		
		translater(centre[VX],centre[VY]);*/
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::ajusterRapportAspect()
	///
	/// Permet d'ajuster les coordonn�es de la fen�tre virtuelle en fonction
	/// de la cl�ture de fa�on � ce que le rapport d'aspect soit respect�.
	///
	/// @param[in]  diminuerLongueur : Indique si on veut que l'ajustement soit en diminuant le c�t� plus long
	///									Par d�faut � vrai pour conserver l'ancien comportement
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::ajusterRapportAspect( bool diminuerLongueur /*= true */ )
	{
		Vecteur2i dimCloture = obtenirDimensionCloture();
		Vecteur2 dimFenetre = obtenirDimensionFenetre();

		ajusterRapportAspect(dimCloture, dimFenetre, diminuerLongueur);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::ajusterRapportAspect( Vecteur2i dimentionCloture, Vecteur2 dimentionFenetre, bool diminuerLongueur /*= true */ )
	///
	/// Permet d'ajuster les coordonn�es de la fen�tre virtuelle en fonction
	/// de la cl�ture de fa�on � ce que le rapport d'aspect soit respect�.
	///
	/// @param[in]	dimentionCloture :	dimention de la cloture
	/// @param[in]	dimentionFenetre :	dimention fenetre
	/// @param[in]  diminuerLongueur : Indique si on veut que l'ajustement soit en diminuant le c�t� plus long
	///									Par d�faut � vrai pour conserver l'ancien comportement
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::ajusterRapportAspect( Vecteur2i dimentionCloture, Vecteur2 dimentionFenetre, bool diminuerLongueur /*= true */ )
	{


		float rapportXYCloture = (float)dimentionCloture[0] / (float)dimentionCloture[1],
			rapportXYFenetre = dimentionFenetre[VX] / dimentionFenetre[VY];

		if( (rapportXYFenetre > rapportXYCloture) == diminuerLongueur )
		{
			float nouvelleLongueurXFenetre = rapportXYCloture*dimentionFenetre[VY];
			float centreX = (xMaxFenetre_ + xMinFenetre_)/2.0f;
			xMinFenetre_ = centreX-nouvelleLongueurXFenetre/2.0f;
			xMaxFenetre_ = centreX+nouvelleLongueurXFenetre/2.0f;
		}
		else
		{
			float nouveauYFenetre = dimentionFenetre[VX]/rapportXYCloture;
			float centreY = (yMaxFenetre_ + yMinFenetre_)/2.0f;
			yMinFenetre_ = centreY-nouveauYFenetre/2.0f;
			yMaxFenetre_ = centreY+nouveauYFenetre/2.0f;
			//angleOuverture_ = 360.0*atan((yMaxFenetre_-yMinFenetre_)/(2.0*zAvant_))/3.141592;

		}


		mettreAJourProjection();

	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionPerspective::convertirClotureAVirtuelle( int i, int j, float& x, float& y ) const 
	///
	/// Permet de calculer les coordonn�es dans l'espace virtuel d'un point
	/// specifi� en coordonn�es d'affichage.  Utilise le calcul des rapports.
	///
	/// @param[in]  i : coordonn�e @a i du point dans l'espace d'affichage.
	/// @param[in]  j : coordonn�e @a j du point dans l'espace d'affichage.
	/// @param[out] x : coordonn�e @a x du point dans l'espace virtuel.
	/// @param[out] y : coordonn�e @a y du point dans l'espace virtuel.
	/// 
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::convertirClotureAVirtuelle( int i,
		int j,
		float& x,
		float& y ) const
	{
		Vecteur2 v = convertirClotureAVirtuelle(Vecteur2i(i,j));
		x = v[VX];
		y = v[VY];
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur2 vue::ProjectionPerspective::convertirClotureAVirtuelle( const Vecteur2i& v ) const
	///
	/// Convertis des coordonn�es cl�ture en coordonn�es virtuelles.
	///
	/// @param[in] const Vecteur2i & v : Les coordonn�es � convertir.
	///
	/// @return Vecteur2 : Les coordonn�es virtuelles
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 ProjectionPerspective::convertirClotureAVirtuelle(const Vecteur2i& v ) const
	{
		Vecteur2 LongueurFenetreVirtuelle = obtenirDimensionFenetre(), LongueurCloture=obtenirDimensionCloture();
		return Vecteur2(xMinFenetre_ + ( v[VX] / LongueurCloture[VX] )*LongueurFenetreVirtuelle[VX], yMaxFenetre_ - ( v[VY] / LongueurCloture[VY] )*LongueurFenetreVirtuelle[VY] );
		
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur2 vue::ProjectionPerspective::convertirDistanceClotureAVirtuelle( const Vecteur2i& v ) const
	///
	/// Convertis des distances cl�tures en distances virtuelles.
	///
	/// @param[in] const Vecteur2i & v : Les distances � convertir.
	///
	/// @return Vecteur2 : Les distances virtuelles
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 ProjectionPerspective::convertirDistanceClotureAVirtuelle( const Vecteur2i& v ) const
	{
		Vecteur2 LongueurFenetreVirtuelle = obtenirDimensionFenetre(), LongueurCloture=obtenirDimensionCloture();
		return Vecteur2( v[VX] / LongueurCloture[VX] * LongueurFenetreVirtuelle[VX],  v[VY] / LongueurCloture[VY] * LongueurFenetreVirtuelle[VY] );
		
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void vue::ProjectionPerspective::afficherBordureVirtuelles(  )
	///
	/// Affiche une croix par dessus la zone d'�dition.
	///
	///
	/// @return Aucune
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::afficherBordureVirtuelles() const
	{
#if WIN32
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex3f(0.0,yMaxFenetre_,100.0);
		glVertex3f(0.0,yMinFenetre_,100.0);
		glVertex3f(xMaxFenetre_,0.0,100.0);
		glVertex3f(xMinFenetre_,0.0,100.0);
		glEnd();
		glDisable(GL_COLOR_LOGIC_OP);
#endif

	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void ProjectionPerspective::centrerAZero()
	///
	/// Centre la fen�tre virtuelle au centre du terrain.
	///
	/// @return Aucune
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionPerspective::centrerAZero()
	{
		/*float largeur = xMaxFenetre_ - xMinFenetre_;
		float hauteur = yMaxFenetre_ - yMinFenetre_;
		xMaxFenetre_ = largeur/2;
		xMinFenetre_ = -largeur/2;
		yMaxFenetre_ = hauteur/2;
		yMinFenetre_ = -hauteur/2;*/
		mettreAJourProjection();
	}

}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
