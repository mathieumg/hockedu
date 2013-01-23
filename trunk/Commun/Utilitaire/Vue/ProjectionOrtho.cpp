////////////////////////////////////////////////////////////////////////////////////
/// @file ProjectionOrtho.cpp
/// @author DGI
/// @date 2006-12-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "glew.h"
#include "ProjectionOrtho.h"
#include "LignePointillee.h"
#include "Utilitaire.h"
#include <GL/glu.h>

namespace vue {

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn ProjectionOrtho::ProjectionOrtho(int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture, float zAvant,  float zArriere, float zoomInMax, float zoomOutMax, float incrementZoom, float xMinFenetre, float xMaxFenetre, float yMinFenetre, float yMaxFenetre)
	///
	/// Constructeur d'une projection orthogonale.  Ne fait qu'assigner les
	/// variables membres et ajuste ensuite le rapport d'aspect.
	///
	/// @param[in] xMinCloture   : coordonnée minimale en @a x de la clôture.
	/// @param[in] xMaxCloture   : coordonnée maximale en @a x de la clôture.
	/// @param[in] yMinCloture   : coordonnée minimale en @a y de la clôture.
	/// @param[in] yMaxCloture   : coordonnée maximale en @a y de la clôture.
	/// @param[in] zAvant        : distance du plan avant (en @a z).
	/// @param[in] zArriere      : distance du plan arrière (en @a z).
	/// @param[in] zoomInMax     : facteur de zoom in maximal.
	/// @param[in] zoomOutMax    : facteur de zoom out maximal.
	/// @param[in] incrementZoom : distance du plan arrière (en @a z).
	/// @param[in] xMinFenetre   : coordonnée minimale en @a x de la fenêtre
	///                            virtuelle.
	/// @param[in] xMaxFenetre   : coordonnée maximale en @a x de la fenêtre
	///                            virtuelle.
	/// @param[in] yMinFenetre   : coordonnée minimale en @a y de la fenêtre
	///                            virtuelle.
	/// @param[in] yMaxFenetre   : coordonnée maximale en @a y de la fenêtre
	///                            virtuelle.
	/// 
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////
	ProjectionOrtho::ProjectionOrtho(int xMinCloture, int xMaxCloture,
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
		//ajusterRapportAspect();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::zoomerIn()
	///
	/// Permet de faire un zoom in selon l'incrément de zoom.
	/// 
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::zoomerIn(float increment) 
	{
		float incrementZoom = increment;
		if(increment == 0)
		{
			incrementZoom = incrementZoom_;
		}
		//Ajouter valeur arbitraire

		float	deltaX = (xMaxFenetre_- xMinFenetre_),
				deltaY = (yMaxFenetre_- yMinFenetre_);
		if( ( deltaX/*/incrementZoom*/ > zoomInMax_) &&  ( deltaY/*/incrementZoom*/ > zoomInMax_ ) )
		{
			xMinFenetre_ += incrementZoom*deltaX*0.05f;
			xMaxFenetre_ -= incrementZoom*deltaX*0.05f;
			yMinFenetre_ += incrementZoom*deltaY*0.05f;
			yMaxFenetre_ -= incrementZoom*deltaY*0.05f;
			/*xMinFenetre_ /= incrementZoom;
			xMaxFenetre_ /= incrementZoom;
			yMinFenetre_ /= incrementZoom;
			yMaxFenetre_ /= incrementZoom;*/
		}
		ajusterRapportAspect(); // Ajoute
		mettreAJourProjection();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::zoomerOut()
	///
	/// Permet de faire un zoom out selon l'incrément de zoom.
	///
	/// @return Aucune.
	///
	//////////////////////////////////////////////////////////////////////// 
	void ProjectionOrtho::zoomerOut(float increment) 
	{
		float incrementZoom = increment;
		float	deltaX = (xMaxFenetre_- xMinFenetre_),
				deltaY = (yMaxFenetre_- yMinFenetre_);
		if(increment == 0)
		{
			incrementZoom = incrementZoom_;
		}
		if(( /*(xMaxFenetre_+ incrementZoom)*/deltaX < zoomOutMax_)  && (/*yMaxFenetre_+ incrementZoom*/deltaY < zoomOutMax_) )
		{
			/*xMinFenetre_ *= incrementZoom;
			xMaxFenetre_ *= incrementZoom;
			yMinFenetre_ *= incrementZoom;
			yMaxFenetre_ *= incrementZoom;*/
			xMinFenetre_ -= incrementZoom*deltaX*0.05f;
			xMaxFenetre_ += incrementZoom*deltaX*0.05f;
			yMinFenetre_ -= incrementZoom*deltaY*0.05f;
			yMaxFenetre_ += incrementZoom*deltaY*0.05f;
		}
		ajusterRapportAspect(); // Ajoute
		mettreAJourProjection();
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::redimensionnerFenetre( const Vecteur2i& coinMin, const Vecteur2i& coinMax )
	///
	/// Permet d'ajuster les coordonnées de la fenêtre virtuelle en fonction
	/// d'un redimensionnement de la fenêtre.
	///
	/// L'agrandissement de la fenêtre virtuelle est proportionnel à
	/// l'agrandissement de la clotûre afin que les objets gardent la même
	/// grandeur apparente lorsque la fenêtre est redimensionnée.
	///
	/// @param[in]  coinMin : Coin contenant les coordonnées minimales de la
	///                       nouvelle clôture
	/// @param[in]  coinMax : Coin contenant les coordonnées maximales de la
	///                       nouvelle clôture
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::redimensionnerFenetre( const Vecteur2i& coinMin,
		const Vecteur2i& coinMax )
	{
		if(coinMax[VX] > 10 && coinMax[VY] > 10)
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
			else if(yMaxCloture_ < xMaxCloture_)
			{
				yMinFenetre_ = xMinFenetre_ * aspectRatio;
				yMaxFenetre_ = xMaxFenetre_ * aspectRatio;
			}
			else if ( xMaxCloture_ < yMaxCloture_)
			{
				xMinFenetre_ = yMinFenetre_ / aspectRatio;
				xMaxFenetre_ = yMaxFenetre_ / aspectRatio;
			}
			mettreAJourProjection();
		}
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::appliquer() const
	///
	/// Cette fonction permet d'appliquer la fenêtre virtuelle en appelant les
	/// fonctions d'OpenGL selon le type de projection et les propriétés de la
	/// fenêtre.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::appliquer() const
	{
		glOrtho( xMinFenetre_, xMaxFenetre_,
			yMinFenetre_, yMaxFenetre_,
			zAvant_, zArriere_ );
		
		
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::zoomerIn( const Vecteur2i& coin1, const Vecteur2i& coin2)
	///
	/// Permet de faire un zoom in sur un rectangle déterminé par deux coins.
	/// Ainsi la région délimitée par le rectangle deviendra la fenêtre
	/// virtuelle.  La fenêtre résultante est ajustée pour respecter le rapport
	/// d'aspect.
	///
	/// @param[in]  coin1 : Le premier coin du rectangle.
	/// @param[in]  coin2 : Le deuxième coin du rectangle.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::zoomerIn( const Vecteur2i& coin1, const Vecteur2i& coin2 ) 
	{

		Vecteur2 coin1Virtuelle = convertirClotureAVirtuelle(coin1), coin2Virtuelle = convertirClotureAVirtuelle(coin2);
		float	nouveauXMin = min(coin1Virtuelle[VX],coin2Virtuelle[VX]),
				nouveauXMax = max(coin1Virtuelle[VX],coin2Virtuelle[VX]),
				nouveauYMin = min(coin1Virtuelle[VY],coin2Virtuelle[VY]),
				nouveauYMax = max(coin1Virtuelle[VY],coin2Virtuelle[VY]);

		float longueurRectangleX = nouveauXMax - nouveauXMin;
		float centreX = longueurRectangleX/2.0f+nouveauXMin;
		float longueurRectangleY = nouveauYMax - nouveauYMin;
		float centreY = longueurRectangleY/2.0f+nouveauYMin;
		
		if(centreX < -250 || centreX > 250 || centreY <-200 || centreY > 200)
			return;


		if( longueurRectangleX < zoomInMax_)
		{
			longueurRectangleX = zoomInMax_;
		}
		xMinFenetre_ = centreX-longueurRectangleX/2.0f;
		xMaxFenetre_ = centreX+longueurRectangleX/2.0f;

		
		if( longueurRectangleY < zoomInMax_)
		{
			longueurRectangleY = zoomInMax_;
		}
		yMinFenetre_ = centreY-longueurRectangleY/2.0f;
		yMaxFenetre_ = centreY+longueurRectangleY/2.0f;

		//Ajustement du rapport en augmentant le côté le plus court, ainsi on est certain de voir tout ce qui a été sélectionné par le rectangle élastique
		ajusterRapportAspect(false);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::zoomerOut( const Vecteur2i& coin1, const Vecteur2i& coin2)
	///
	/// Permet de faire un zoom out sur un rectangle délimité par deux coins.
	/// Ainsi la fenêtre virtuelle avant le zoom sera placée à l'interieur de
	/// la région délimité par le rectangle.  La fenêtre résultante est ajustée
	/// pour respecter le rapport d'aspect.
	///
	/// @param[in]  coin1 : Le premier coin du rectangle.
	/// @param[in]  coin2 : Le deuxième coin du rectangle.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::zoomerOut( const Vecteur2i& coin1, const Vecteur2i& coin2 ) 
	{
 		Vecteur2 coin1Virtuelle = convertirClotureAVirtuelle(coin1), coin2Virtuelle = convertirClotureAVirtuelle(coin2);
		float	nouveauXMin = min(coin1Virtuelle[VX],coin2Virtuelle[VX]),
			nouveauXMax = max(coin1Virtuelle[VX],coin2Virtuelle[VX]),
			nouveauYMin = min(coin1Virtuelle[VY],coin2Virtuelle[VY]),
			nouveauYMax = max(coin1Virtuelle[VY],coin2Virtuelle[VY]);

		Vecteur2 lDepart = obtenirDimensionFenetre();
		Vecteur2 lFin;
		float lDessinerX = nouveauXMax - nouveauXMin;
		lFin[VX] = lDepart[VX]*lDepart[VX]/lDessinerX;
		float centreX = (xMinFenetre_+xMaxFenetre_)/2.0f;
		float lDessinerY = nouveauYMax - nouveauYMin;
		lFin[VY] = lDepart[VY]*lDepart[VY]/lDessinerY;
		float centreY = (yMinFenetre_+yMaxFenetre_)/2.0f;


		if( lFin[VX] > zoomOutMax_)
		{
			lFin[VX] = zoomOutMax_;
		}
		float variation = (lFin[VX]-lDepart[VX])/2.0f;
		xMinFenetre_ = centreX-(lFin[VX]/2.0f);
		xMaxFenetre_ = centreX+(lFin[VX]/2.0f);

		
		if( lFin[VY] > zoomOutMax_)
		{
			lFin[VY] = zoomOutMax_;
		}
		variation = (lFin[VY]-lDepart[VY])/2.0f;
		yMinFenetre_ = centreY-(lFin[VY]/2.0f);
		yMaxFenetre_ = centreY+(lFin[VY]/2.0f);

		ajusterRapportAspect();

		Vecteur2i pointCentre((int)((coin1[VX]+coin2[VX])/2.0f),(int)((coin1[VY]+coin2[VY])/2.0f));
		centrerSurPoint(pointCentre);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::translater( float deplacementX, float deplacementY )
	///
	/// Permet de déplacer la fenêtre virtuelle en @a x et en @a y.  Les
	/// déplacement doivent être exprimés en proportion de la fenêtre virtuelle.
	///
	/// @param[in]  deplacementX : le déplacement en @a x.
	/// @param[in]  deplacementY : le déplacement en @a y.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::translater( float deplacementX, float deplacementY )
	{
		if( (deplacementX < 0 || xMaxFenetre_-deplacementX > -150) && (deplacementX > 0 || xMinFenetre_-deplacementX < 150))
		{
			xMinFenetre_ -= deplacementX;
			xMaxFenetre_ -= deplacementX;
		}
		if( yMaxFenetre_+deplacementY > -100 && yMinFenetre_+deplacementY < 100)
		{
			yMaxFenetre_ += deplacementY;
			yMinFenetre_ += deplacementY;
		}
		mettreAJourProjection();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::translater(const Vecteur2i& deplacement)
	///
	/// Permet de translater la fenêtre virtuelle en fonction d'un déplacement
	/// en coordonnées de clôture.
	///
	/// @param[in]  deplacement : Le déplacement en coordonnées de clôture.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::translater( const Vecteur2i& deplacement )
	{
		////////// ATTENTION au changement de logique, CentrerSurPoint est dépendant à son comportement actuel
		Vecteur2 deplacementVirtuel = convertirDistanceClotureAVirtuelle(deplacement);
		translater(deplacementVirtuel[VX],deplacementVirtuel[VY]);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::centrerSurPoint( const Vecteur2i& pointCentre )
	///
	/// Permet de centrer la fenêtre virtuelle sur un point de l'écran. Le
	/// point de l'écran est convertit en coordonnées virtuelles et il devient
	/// le centre de la fenêtre virtuelle.
	///
	/// @param[in]  pointCentre : Le point sur lequel on doit centrer.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::centrerSurPoint( const Vecteur2i& pointCentre ) 
	{
		Vecteur2i lCloture = obtenirDimensionCloture();
		Vecteur2i newCentre(pointCentre[VX]-(lCloture[VX]>>1), pointCentre[VY]-(lCloture[VY]>>1));
		Vecteur2 centre = convertirDistanceClotureAVirtuelle(newCentre);
		
		translater(centre[VX],centre[VY]);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::ajusterRapportAspect()
	///
	/// Permet d'ajuster les coordonnées de la fenêtre virtuelle en fonction
	/// de la clôture de façon à ce que le rapport d'aspect soit respecté.
	///
	/// @param[in]  diminuerLongueur : Indique si on veut que l'ajustement soit en diminuant le côté plus long
	///									Par défaut à vrai pour conserver l'ancien comportement
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::ajusterRapportAspect( bool diminuerLongueur /*= true */ )
	{
		Vecteur2i dimCloture = obtenirDimensionCloture();
		Vecteur2 dimFenetre = obtenirDimensionFenetre();
		float rapportXYCloture = dimCloture[0] / (float)dimCloture[1],
			rapportXYFenetre = dimFenetre[VX] / dimFenetre[VY];

		if( (rapportXYFenetre > rapportXYCloture) == diminuerLongueur )
		{
			float nouvelleLongueurXFenetre = rapportXYCloture*dimFenetre[VY];
			float centreX = (xMaxFenetre_ + xMinFenetre_)/2.0f;
			xMinFenetre_ = centreX-nouvelleLongueurXFenetre/2.0f;
			xMaxFenetre_ = centreX+nouvelleLongueurXFenetre/2.0f;
		}
		else
		{
			float nouveauYFenetre = dimFenetre[VX]/rapportXYCloture;
			float centreY = (yMaxFenetre_ + yMinFenetre_)/2.0f;
			yMinFenetre_ = centreY-nouveauYFenetre/2.0f;
			yMaxFenetre_ = centreY+nouveauYFenetre/2.0f;
		}
		mettreAJourProjection();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void ProjectionOrtho::convertirClotureAVirtuelle( int i, int j, float& x, float& y ) const 
	///
	/// Permet de calculer les coordonnées dans l'espace virtuel d'un point
	/// specifié en coordonnées d'affichage.  Utilise le calcul des rapports.
	///
	/// @param[in]  i : coordonnée @a i du point dans l'espace d'affichage.
	/// @param[in]  j : coordonnée @a j du point dans l'espace d'affichage.
	/// @param[out] x : coordonnée @a x du point dans l'espace virtuel.
	/// @param[out] y : coordonnée @a y du point dans l'espace virtuel.
	/// 
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::convertirClotureAVirtuelle( int i,
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
	/// @fn Vecteur2 vue::ProjectionOrtho::convertirClotureAVirtuelle( const Vecteur2i& v ) const
	///
	/// Convertis des coordonnées clôture en coordonnées virtuelles.
	///
	/// @param[in] const Vecteur2i & v : Les coordonnées à convertir.
	///
	/// @return Vecteur2 : Les coordonnées virtuelles
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 ProjectionOrtho::convertirClotureAVirtuelle(const Vecteur2i& v ) const
	{
		Vecteur2 LongueurFenetreVirtuelle = obtenirDimensionFenetre(), LongueurCloture=obtenirDimensionCloture();
		return Vecteur2(xMinFenetre_ + ( v[VX] / LongueurCloture[VX] )*LongueurFenetreVirtuelle[VX], yMaxFenetre_ - ( v[VY] / LongueurCloture[VY] )*LongueurFenetreVirtuelle[VY] );
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur2 vue::ProjectionOrtho::convertirDistanceClotureAVirtuelle( const Vecteur2i& v ) const
	///
	/// Convertis des distances clôtures en distances virtuelles.
	///
	/// @param[in] const Vecteur2i & v : Les distances à convertir.
	///
	/// @return Vecteur2 : Les distances virtuelles
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 ProjectionOrtho::convertirDistanceClotureAVirtuelle( const Vecteur2i& v ) const
	{
		Vecteur2 LongueurFenetreVirtuelle = obtenirDimensionFenetre(), LongueurCloture=obtenirDimensionCloture();
		return Vecteur2( v[VX] / LongueurCloture[VX] * LongueurFenetreVirtuelle[VX],  v[VY] / LongueurCloture[VY] * LongueurFenetreVirtuelle[VY] );
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void vue::ProjectionOrtho::afficherBordureVirtuelles(  )
	///
	/// Affiche une croix par dessus la zone d'édition.
	///
	///
	/// @return Aucune
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::afficherBordureVirtuelles() const
	{
		glDisable(GL_TEXTURE_2D); 
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex3d(0.0,yMaxFenetre_,100.0);
		glVertex3d(0.0,yMinFenetre_,100.0);
		glVertex3d(xMaxFenetre_,0.0,100.0);
		glVertex3d(xMinFenetre_,0.0,100.0);
		glEnd();
		glDisable(GL_COLOR_LOGIC_OP);

	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void ProjectionOrtho::centrerAZero()
	///
	/// Centre la fenêtre virtuelle au centre du terrain.
	///
	/// @return Aucune
	///
	////////////////////////////////////////////////////////////////////////
	void ProjectionOrtho::centrerAZero()
	{
		float largeur = xMaxFenetre_ - xMinFenetre_;
		float hauteur = yMaxFenetre_ - yMinFenetre_;
		xMaxFenetre_ = largeur/2;
		xMinFenetre_ = -largeur/2;
		yMaxFenetre_ = hauteur/2;
		yMinFenetre_ = -hauteur/2;
	}

}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
