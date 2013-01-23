///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NoeudMuretRelatif.h"
#include "ArbreRenduINF2990.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n1, NoeudPoint* n2 ):
NoeudMuret(ArbreRenduINF2990::NOM_MURET_RELATIF,false)
{
	coin1_[VX] = &n1->positionRelative_[VX];
	coin1_[VY] = &n1->positionRelative_[VY];
	coin1_[VZ] = &n1->positionRelative_[VZ];
	coin2_[VX] = &n2->positionRelative_[VX];
	coin2_[VY] = &n2->positionRelative_[VY];
	coin2_[VZ] = &n2->positionRelative_[VZ];

    n1->attach(*this);
    n2->attach(*this);

	animer(0);
	assignerEstSelectionnable(false);
	assignerEstEnregistrable(false);
	mettreAJourEchelleRotation();
	//assignerAffiche(false); // Mis en commentaire pour l'ajout d'un modele 3D
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n, NoeudBut* but, bool haut )
///
/// constructeur par paramètre
///
/// @param[in] NoeudPoint * n : point relié
/// @param[in] NoeudBut * but : But relié
/// @param[in] bool haut : indique si on ce lie à la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n, NoeudBut* but, bool haut ):
NoeudMuret(ArbreRenduINF2990::NOM_MURET_RELATIF, false)
{
	coin1_[VX] = &n->positionRelative_[VX];
	coin1_[VY] = &n->positionRelative_[VY];
	coin1_[VZ] = &n->positionRelative_[VZ];
	if(haut)
	{
		coin2_[VX] = &but->mTopPosition[VX];
		coin2_[VY] = &but->mTopPosition[VY];
		coin2_[VZ] = &but->mTopPosition[VZ];
	}
	else
	{
		coin2_[VX] = &but->mBottomPosition[VX];
		coin2_[VY] = &but->mBottomPosition[VY];
		coin2_[VZ] = &but->mBottomPosition[VZ];
	}
    n->attach(*this);
    but->attach(*this);

	animer(0);
	assignerEstSelectionnable(false);
	assignerEstEnregistrable(false);

	mettreAJourEchelleRotation();
	//assignerAffiche(false);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( NoeudBut* but, NoeudPoint* n, bool haut )
///
/// constructeur par paramètre
///
/// @param[in] NoeudPoint * n : point relié
/// @param[in] NoeudBut * but : But relié
/// @param[in] bool haut : indique si on ce lie à la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( NoeudBut* but, NoeudPoint* n, bool haut ):
NoeudMuret(ArbreRenduINF2990::NOM_MURET_RELATIF, false)
{
	coin2_[VX] = &n->positionRelative_[VX];
	coin2_[VY] = &n->positionRelative_[VY];
	coin2_[VZ] = &n->positionRelative_[VZ];
	if(haut)
	{
		coin1_[VX] = &but->mTopPosition[VX];
		coin1_[VY] = &but->mTopPosition[VY];
		coin1_[VZ] = &but->mTopPosition[VZ];
	}
	else
	{
		coin1_[VX] = &but->mBottomPosition[VX];
		coin1_[VY] = &but->mBottomPosition[VY];
		coin1_[VZ] = &but->mBottomPosition[VZ];
	}

    n->attach(*this);
    but->attach(*this);

	animer(0);
	assignerEstSelectionnable(false);
	assignerEstEnregistrable(false);

	mettreAJourEchelleRotation();
	//assignerAffiche(false);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::~NoeudMuretRelatif( void )
///
/// Destructeur des murets relatifs.
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::~NoeudMuretRelatif(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudMuretRelatif::obtenirCoin1()
///
/// Accesseur du coin1.
///
///
/// @return Vecteur3 : la position du coin1.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuretRelatif::obtenirCoin1()
{
	return Vecteur3(*coin1_[VX],*coin1_[VY],0);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudMuretRelatif::obtenirCoin2()
///
/// Accesseur du coin2.
///
///
/// @return Vecteur3 : la position du coin2.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuretRelatif::obtenirCoin2()
{
	return Vecteur3(*coin2_[VX],*coin2_[VY],0);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuretRelatif::afficherConcret()
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuretRelatif::afficherConcret() const
{
	// Fais rien, ce muret est invisible...non non non, il l'est maintenant!
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	NoeudComposite::afficherConcret(); // On ne call pas de liste pour ne pas les afficher

	glPopAttrib();
	glPopMatrix();

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuretRelatif::animer( const float& temps )
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] float temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuretRelatif::animer( const float& temps )
{
	// Appel à la version de la classe de base pour l'animation des enfants.
	NoeudComposite::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudMuretRelatif::obtenirRayon(  )
///
/// Méthode temporaire, les murets ne devrait plus avoir de rayon puisque ce sont des segments.
///
///
/// @return float : le rayon.
///
////////////////////////////////////////////////////////////////////////
float NoeudMuretRelatif::obtenirRayon() const
{
	return 0;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudMuretRelatif::mettreAJourEchelleRotation()
///
/// Methode pour initialiser la mise a l'echelle et l'angle de rotation ainsi que mettre a jour la matrice de transformation pour l'affichage des modeles 3D
///
/// @return float : le rayon.
///
////////////////////////////////////////////////////////////////////////
void NoeudMuretRelatif::mettreAJourEchelleRotation()
{
	Vecteur3 vecteurEntre(*coin2_[VX]-*coin1_[VX], *coin2_[VY]-*coin1_[VY], 0);
	assignerPositionRelative(Vecteur3(*coin1_[VX]+vecteurEntre[VX]/2.0f, *coin1_[VY]+vecteurEntre[VY]/2.0f));
	float distance = vecteurEntre.norme();
	modifierEchelleCourante(Vecteur3(distance, 5, 5));
	float angle = utilitaire::RAD_TO_DEG(atan2(vecteurEntre[VY], vecteurEntre[VX]));
	assignerAngle(angle);
	updateMatrice();
    updatePhysicBody();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::updatePosition( class PositionSubject& pSubject )
///
/// /*Description*/
///
/// @param[in] class PositionSubject & pSubject
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
void NoeudMuretRelatif::updatePosition( PositionSubject& pSubject )
{
    mettreAJourEchelleRotation();
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////