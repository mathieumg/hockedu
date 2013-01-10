///////////////////////////////////////////////////////////////////////////////
/// @file NoeudBut.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudBut.h"
#include "ArbreRenduINF2990.h"
#include "VisiteurCollision.h"
#include "GestionnaireModeles.h"
#include "NoeudPoint.h"
#include "FacadeModele.h"
#include "XMLUtils.h"
#include <Box2D/Box2D.h>
#include "Utilitaire.h"

double NoeudBut::longueurBut_ = 1;


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudBut::NoeudBut( const std::string& typeNoeud, int joueur, NoeudPoint * coinHaut, NoeudPoint * coinBas )
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud.
/// @param[in] int joueur : Le numero du joueur.
/// @param[in] NoeudPoint * coinHaut : Un pointeur sur le coin en haut.
/// @param[in] NoeudPoint * coinBas : Un pointeur sur le coin en bas.
/// @param[in] NoeudComposite * pParent : parent du but
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudBut::NoeudBut(const std::string& typeNoeud, int joueur, NoeudPoint * coinHaut, NoeudPoint * coinBas, NoeudComposite* pParent)
   : NoeudComposite(typeNoeud), joueur_(joueur), coinHaut_(coinHaut), coinBas_(coinBas), mBottomAngle(0),mTopAngle(0), longueurButBase_(1)
{
    if(pParent)
    {
        pParent->ajouter(this);
    }

	longueurBut_ = longueurButBase_;
	echelleCourante_ = Vecteur3(30, 5, 5);
    updateLongueur();
// 	updateMatrice();
//     updatePhysicBody();
	//assignerEstSelectionnable(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudBut::~NoeudBut()
///
/// Ce destructeur ne fait rien puisque la destruction n'est pas de ses reponsabilités.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudBut::~NoeudBut()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::updatePhysicBody()
{
    clearPhysicsBody();

    float halfLength = (float)echelleCourante_[VX]/2.f;//(float)(coin2-coin1).norme()/2.f;

    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody; //this will be a dynamic body
    myBodyDef.position.Set(0, 0); //set the starting position
    myBodyDef.angle = 0; //set the starting angle

    mPhysicBody = getWorld()->CreateBody(&myBodyDef);
    Vecteur3 anchorPointPos = obtenirPositionAbsolue();
    b2Vec2 anchorPointPosB2, topPosB2,BottomPosB2 ;
    utilitaire::VEC3_TO_B2VEC(anchorPointPos,anchorPointPosB2);
    utilitaire::VEC3_TO_B2VEC(positionHaut_,topPosB2);
    utilitaire::VEC3_TO_B2VEC(positionBas_,BottomPosB2);
    b2EdgeShape shape;
    shape.Set(anchorPointPosB2,topPosB2);
    
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &shape; //this is a pointer to the shapeHaut above
    myFixtureDef.density = 1;
    myFixtureDef.filter.categoryBits = CATEGORY_NONE;
    myFixtureDef.filter.maskBits = CATEGORY_NONE;
    myFixtureDef.filter.groupIndex = 1;

    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
    shape.Set(anchorPointPosB2,BottomPosB2);
    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::afficherConcret() const
{
    GLuint liste = NULL;
    GestionnaireModeles::obtenirInstance()->obtenirListe(type_,liste);

    glTranslated(positionRelative_[0], positionRelative_[1], positionRelative_[2]);

    if(liste != 0 && estAffiche())
    {
        Vecteur3 positionPoint = parent_->obtenirPositionAbsolue();
        Vecteur3 posBas = positionBas_ - positionPoint;
        Vecteur3 posHaut = positionHaut_ - positionPoint;

        // Initialisation
        glPushMatrix();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);

        // Dessin de la partie ajustable en bas
        glPushMatrix();
        glTranslated(posBas[VX], posBas[VY], 0);
        glRotated(mBottomAngle, 0, 0, 1);
        glMultMatrixd(matrice_);
        if(joueur_==1)
        {
            glScalef(1, -1, 1);
        }
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        // Efface le stencil et l'active
        glClearStencil(0);
        glClear( GL_STENCIL_BUFFER_BIT );
        glEnable( GL_STENCIL_TEST );
        // A chaque fois qu'un pixel est dessine a l'ecran, le stencil met le bit a 1
        glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
        glCallList(liste);
        glDisable(GL_STENCIL_TEST);
        glPopAttrib();
        glPopMatrix();

        // Dessin de la partie ajustable en haut
        glPushMatrix();
        glTranslated(posHaut[VX], posHaut[VY], 0);
        glRotated(mTopAngle, 0, 0, 1);
        glMultMatrixd(matrice_);
        if(joueur_==2)
        {
            glScalef(1., -1., 1.);
        }
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        // Efface le stencil et l'active
        glClearStencil(0);
        glClear( GL_STENCIL_BUFFER_BIT );
        glEnable( GL_STENCIL_TEST );
        // A chaque fois qu'un pixel est dessine a l'ecran, le stencil met le bit a 1
        glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
        glCallList(liste); 

        glDisable(GL_STENCIL_TEST);

        glPopAttrib();
        glPopMatrix();


        // Fin
        glDisable(GL_CULL_FACE);
        glPopMatrix();
    }


}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::animer( const float& temps )
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] float temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::animer( const float& temps)
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::accueillirVisiteurNoeud( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud qui contient la transformation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::accueillirVisiteurNoeud( VisiteurNoeud& v )
{
	v.visiterNoeudBut(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int NoeudBut::obtenirJoueur(  )
///
/// Retourne le numero du joueuer, 1 ou 2.
///
///
/// @return int : Le numero du joueur.
///
////////////////////////////////////////////////////////////////////////
int NoeudBut::obtenirJoueur() const
{
	return joueur_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait::PaireVect3 NoeudBut::obtenirZoneOccupee(  )
///
/// Retourne la position occupee par le but.
///
///
/// @return NoeudAbstrait::PaireVect3 : position min et max occuppee.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait::PaireVect3 NoeudBut::obtenirZoneOccupee() const
{
	/// A COMPLETER
	return PaireVect3();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::updateLongueur( double facteurModificationEchelle )
///
/// Calcul et retourne le longueur du but.
///
/// @param[in] double facteurModificationEchelle :
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::updateLongueur(double facteurModificationEchelle)
{
	echelleCourante_[VX]*=facteurModificationEchelle;

	Vecteur3 pos = obtenirPositionAbsolue();

	Vecteur3 deltaHaut=(coinHaut_->obtenirPositionAbsolue()-pos)*0.85;
	double longueurHaut = deltaHaut.norme(); // Valeur pour ne pas que les but empiete sur les bandes
	Vecteur3 deltaBas=(coinBas_->obtenirPositionAbsolue()-pos)*0.85;
	double longueurBas = deltaBas.norme();	// Valeur pour ne pas que les but empiete sur les bandes
	double longueur=echelleCourante_[VX]*longueurButBase_;
	if(longueur>longueurHaut)
	{
		longueur=longueurHaut;
		echelleCourante_[VX] = longueur/longueurButBase_;
	}
	if(longueur>longueurBas)
	{
		longueur=longueurBas;
		echelleCourante_[VX] = longueur/longueurButBase_;
	}

	double ratioHaut = longueur/longueurHaut;
	double ratioBas = longueur/longueurBas;

    double	
        deltaXhaut = deltaHaut[VX]*ratioHaut,
        deltaXbas = deltaBas[VX]*ratioBas,
        deltaYhaut = deltaHaut[VY]*ratioHaut,
        deltaYbas = deltaBas[VY]*ratioBas;

	positionHaut_[VX] = pos[VX]+deltaXhaut;
	positionHaut_[VY] = pos[VY]+deltaYhaut;
	positionBas_[VX] = pos[VX]+deltaXbas;
	positionBas_[VY] = pos[VY]+deltaYbas;
	longueurBut_ = longueur;

    mBottomAngle = utilitaire::RAD_TO_DEG(atan2(deltaYbas, deltaXbas)), 
    mTopAngle = utilitaire::RAD_TO_DEG(atan2(deltaYhaut, deltaXhaut));
    

	updateMatrice();
    updatePhysicBody();
    signalModification();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::modifierButAdverse( NoeudBut* butAdverse )
///
/// Modifie le pointeur sur le but adverse.
///
/// @param[in] NoeudBut * butAdverse : un pointeur sur le but adverse.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::modifierButAdverse( NoeudBut* butAdverse )
{
	butAdverse_=butAdverse;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudBut* NoeudBut::obtenirButAdverse(  )
///
/// Accesseur du but adverse.
///
///
/// @return NoeudBut* :  un pointeur sur le but adverse.
///
////////////////////////////////////////////////////////////////////////
NoeudBut* NoeudBut::obtenirButAdverse()
{
	return butAdverse_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn double NoeudBut::obtenirRayon(  )
///
/// obtention du rayon
///
///
/// @return double
///
////////////////////////////////////////////////////////////////////////
double NoeudBut::obtenirRayon()
{
	return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::assignerAttributVisiteurCollision( VisiteurCollision* v )
///
/// permet d'indiquer au visiteur de collision les attribut specifique a ce noeud
///
/// @param[in] VisiteurCollision * v
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::assignerAttributVisiteurCollision( VisiteurCollision* v )
{
	v->modifierTypeCollision(SEGMENT);
	v->modifierCoin1(coinBas_->obtenirPositionAbsolue());
	v->modifierCoin2(coinHaut_->obtenirPositionAbsolue());
	v->modifierRayonAVerifier(longueurBut_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* NoeudBut::creerNoeudXML()
///
/// Creation du noeud XML d'un but
///
///
/// @return TiXmlElement*
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* NoeudBut::creerNoeudXML()
{
	TiXmlElement* elementNoeud = NoeudComposite::creerNoeudXML();

    XMLUtils::ecrireAttribute<double>(elementNoeud,"longueurBut",longueurBut_);
    XMLUtils::ecrireAttribute<int>(elementNoeud,"joueur",joueur_);
    XMLUtils::ecrireVecteur3Dxml(&positionHaut_,elementNoeud,"coinHaut");
    XMLUtils::ecrireVecteur3Dxml(&positionBas_,elementNoeud,"coinBas");
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudBut::initialiser( const TiXmlElement* element )
///
/// Initialisation du But à partir d'un element XML
///
/// @param[in] const TiXmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudBut::initialiser( const TiXmlElement* element )
{
	if(!NoeudComposite::initialiser(element))
		return false;
	double doubleElem;
    if(!XMLUtils::LireAttribute<double>(element,"longueurBut",doubleElem))
		return false;
	longueurBut_ = doubleElem;

	int intElem;
    if(!XMLUtils::LireAttribute<int>(element,"joueur",intElem))
		return false;
	joueur_ = intElem;

    if( !XMLUtils::lectureVecteur3Dxml(&positionHaut_,element,"coinHaut") )
		return false;
    if( !XMLUtils::lectureVecteur3Dxml(&positionBas_,element,"coinBas") )
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn double NoeudBut::obtenirHauteurBut()
///
/// Permet de connaitre la hauteur des buts pour savoir si la rondelle peut y passer
///
///
/// @return double
///
////////////////////////////////////////////////////////////////////////
double NoeudBut::obtenirHauteurBut()
{
	return positionHaut_[VY] - positionBas_[VY];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::forceFullUpdate()
///
/// Recreates everything needed for the game
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::forceFullUpdate()
{
    updateLongueur();
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
