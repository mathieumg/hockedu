///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMuret.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudMuret.h"
#include "Utilitaire.h"
#include "VisiteurCollision.h"
#include "FacadeModele.h"
#include "Modele3D.h"
#include "tinyxml.h"
#include "XMLUtils.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuret::NoeudMuret( const std::string& typeNoeud, bool estSurTable )
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud.
/// @param[in] bool estSurTable : si le muret est sur la table ou si c'est une bordure
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuret::NoeudMuret(const std::string& typeNoeud, bool estSurTable)
	: NoeudComposite(typeNoeud)
{
	if(estSurTable)
		FacadeModele::obtenirInstance()->ajouterElementSurTable(this);
	coefRebond_ = 0.75;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::~NoeudMuret()
///
/// Ce destructeur d�sallouee la liste d'affichage du muret.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuret::~NoeudMuret()
{
	FacadeModele::obtenirInstance()->supprimerElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::afficherConcret() const
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::afficherConcret() const
{
	// Sauvegarde de la matrice.
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0.76f, 0.64f, 0.31f);

	// Appel � la version de la classe de base pour l'affichage des enfants.
	NoeudComposite::afficherConcret();

	// Restauration de la matrice.
	glPopAttrib();
	glPopMatrix();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::animer( const float& temps)
{
	// Appel � la version de la classe de base pour l'animation des enfants.
	NoeudComposite::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::accueillirVisiteurNoeud( const VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::accueillirVisiteurNoeud( VisiteurNoeud& v )
{
	v.visiterNoeudMuret(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirCoin1()
///
/// Retourne la position du coin1.
///
/// @param[in] void
///
/// @return Vecteur3 positionCoin1 : vecteur contenant la position.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuret::obtenirCoin1()
{
	return positionCoin1_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirCoin2()
///
/// Retourne la position du coin2.
///
/// @param[in] void
///
/// @return Vecteur3 positionCoin2 : vecteur contenant la position.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuret::obtenirCoin2()
{
	return positionCoin2_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::majPosCoins(  )
///
/// Permet de recalculer la position des coin des muret suite a une rotation
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::majPosCoins()
{
	//double deltaX = (cos(utilitaire::DEG_TO_RAD(angle_)) *echelleCourante_[VX])/2.0;
	//double deltaY = (sin(utilitaire::DEG_TO_RAD(angle_)) *echelleCourante_[VX])/2.0;
	Vecteur3 deplacement( cos(utilitaire::DEG_TO_RAD(angle_) ), sin(utilitaire::DEG_TO_RAD(angle_) ) );
	deplacement*= echelleCourante_[VX];
	deplacement /= 2.0;
	positionCoin1_ = positionRelative_+deplacement;
	positionCoin2_ = positionRelative_-deplacement;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::trouverAngle(  )
///
/// Permet de calculer l'angle du muret selon ces coins
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::trouverAngle()
{
	Vecteur3 coin1Relatif(positionCoin1_-positionRelative_);
	coin1Relatif.normaliser();
	angle_ = (360.0+(utilitaire::RAD_TO_DEG(acos(coin1Relatif[VX]))*( (coin1Relatif[VY] < 0)?-1:1) ) );
	angle_ /= 360.0;
	angle_ -= (int)(angle_);
	angle_ *= 360.0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::assignerPositionCoin( int lequel, Vecteur3 position )
///
/// Modifie la position des coins
///
/// @param[in] int			lequel		: lequel des deux coins ( 1 ou 2)
///				Vecteur3	position	: position a assigner
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerPositionCoin( int lequel, Vecteur3 position )
{
	if(lequel==1)
	{
		positionCoin1_ = position;
	}
	else if(lequel==2)
	{
		positionCoin2_ = position;
	}


	double	deltaX = positionCoin2_[VX]-positionCoin1_[VX],
		deltaY = positionCoin2_[VY]-positionCoin1_[VY];

	positionRelative_ = Vecteur3(positionCoin1_[VX]+deltaX/2.0, positionCoin1_[VY]+deltaY/2.0, 0);
	trouverAngle();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirRayon() const
///
/// Retourne le rayon du noeud.
///
/// @param[in]	void
///
/// @return double : rayon du noeud
///
////////////////////////////////////////////////////////////////////////
double NoeudMuret::obtenirRayon() const
{
	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::assignerAttributVisiteurCollision( VisiteurCollision* v )
///
/// Permet d'assigner les attribut n�cessaire � la collision.
///
/// @param[in] VisiteurCollision * v : le visiteur de collision.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerAttributVisiteurCollision( VisiteurCollision* v )
{
	v->modifierTypeCollision(SEGMENT);
	v->modifierCoin1(positionCoin1_);
	v->modifierCoin2(positionCoin2_);
	v->modifierRayonAVerifier(obtenirRayon());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* NoeudMuret::creerNoeudXML()
///
/// /*Description*/
///
///
/// @return TiXmlElement*
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* NoeudMuret::creerNoeudXML()
{
	TiXmlElement* elementNoeud = NoeudComposite::creerNoeudXML();
	// Ajouter la position des coins des murets
	elementNoeud->SetDoubleAttribute("coefRebond",coefRebond_);
    XMLUtils::ecrireVecteur3Dxml(&positionCoin1_,elementNoeud,"coinA");
    XMLUtils::ecrireVecteur3Dxml(&positionCoin2_,elementNoeud,"coinB");
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudMuret::initialiser( const TiXmlElement* element )
///
/// Initialisation du NoeudMuret � partir d'un element XML
///
/// @param[in] const TiXmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudMuret::initialiser( const TiXmlElement* element )
{
	if(!NoeudComposite::initialiser(element))
		return false;
	double doubleElem;
	if( element->QueryDoubleAttribute("coefRebond", &doubleElem) != TIXML_SUCCESS )
		return false;
	coefRebond_ = doubleElem;

    if( !XMLUtils::lectureVecteur3Dxml(&positionCoin1_,element,"coinA") )
		return false;
    if( !XMLUtils::lectureVecteur3Dxml(&positionCoin2_,element,"coinB") )
		return false;
	trouverAngle();
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::assignerAngle( double angle )
///
/// Permet de modifier l'angle du muret et garde les coins a jour
///
/// @param[in] double angle : nouvel angle du muret
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerAngle( const double& angle )
{
	NoeudAbstrait::assignerAngle(angle);
	majPosCoins();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::modifierEchelleCourante( const Vecteur3& echelleCourante )
///
/// Mutateur des facteurs d'echelle et conserve les coins a jour
///
/// @param[in] const Vecteur3 & echelleCourante
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::modifierEchelleCourante( const Vecteur3& echelleCourante )
{
	NoeudAbstrait::modifierEchelleCourante(echelleCourante);
	majPosCoins();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::assignerPositionRelative( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerPositionRelative( const Vecteur3& positionRelative )
{
	NoeudAbstrait::assignerPositionRelative(positionRelative);
	majPosCoins();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
