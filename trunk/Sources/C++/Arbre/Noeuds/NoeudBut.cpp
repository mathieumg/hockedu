///////////////////////////////////////////////////////////////////////////////
/// @file NoeudBut.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudBut.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "VisiteurCollision.h"
#include "GestionnaireModeles.h"
#include "NoeudPoint.h"
#include "FacadeModele.h"
#include "XMLUtils.h"
#include "Utilitaire.h"

//const Vecteur3 NoeudBut::DEFAULT_SIZE = Vecteur3(1,1,1);
const Vecteur3 NoeudBut::DEFAULT_SIZE = Vecteur3(30, 10, 5);

CreateListDelegateImplementation(Goal)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = NoeudBut::DEFAULT_SIZE / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}

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
   : NoeudComposite(typeNoeud), joueur_(joueur), coinHaut_(coinHaut), coinBas_(coinBas), mBottomAngle(0),mTopAngle(0), mPuckCatcher(NULL),mCachedPuckRadius(20.f)
{
    if(pParent)
    {
        pParent->ajouter(this);
    }

	mGoalLength = DEFAULT_SIZE[VX];
	//echelleCourante_ = Vecteur3(30, 5, 5);
    updateLongueur();
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
    // doit etre appelé ici, car l'instance NoeudBut n'existe plus lorsque nous atteignons le destructeur de NoeudAbstrait
    clearPhysicsBody();
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
        Vecteur3 posBas = mBottomPosition - positionPoint;
        Vecteur3 posHaut = mTopPosition - positionPoint;

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
/// @fn void NoeudBut::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud qui contient la transformation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::acceptVisitor( VisiteurNoeud& v )
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
/// @fn void NoeudBut::updateLongueur( float facteurModificationEchelle )
///
/// Calcul et retourne le longueur du but.
///
/// @param[in] float facteurModificationEchelle :
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::updateLongueur(float facteurModificationEchelle)
{
	echelleCourante_[VX]*=facteurModificationEchelle;

	Vecteur3 pos = obtenirPositionAbsolue();

	Vecteur3 deltaHaut=(coinHaut_->obtenirPositionAbsolue()-pos)*0.85f;
	float longueurHaut = deltaHaut.norme(); // Valeur pour ne pas que les but empiete sur les bandes
	Vecteur3 deltaBas=(coinBas_->obtenirPositionAbsolue()-pos)*0.85f;
	float longueurBas = deltaBas.norme();	// Valeur pour ne pas que les but empiete sur les bandes
	float longueur=echelleCourante_[VX]*DEFAULT_SIZE[VX];
	if(longueur>longueurHaut)
	{
		longueur=longueurHaut;
		echelleCourante_[VX] = longueur/DEFAULT_SIZE[VX];
	}
	if(longueur>longueurBas)
	{
		longueur=longueurBas;
		echelleCourante_[VX] = longueur/DEFAULT_SIZE[VX];
	}

	float ratioHaut = longueur/longueurHaut;
	float ratioBas = longueur/longueurBas;

    float	
        deltaXhaut = deltaHaut[VX]*ratioHaut,
        deltaXbas = deltaBas[VX]*ratioBas,
        deltaYhaut = deltaHaut[VY]*ratioHaut,
        deltaYbas = deltaBas[VY]*ratioBas;

	mTopPosition[VX] = pos[VX]+deltaXhaut;
	mTopPosition[VY] = pos[VY]+deltaYhaut;
	mBottomPosition[VX] = pos[VX]+deltaXbas;
	mBottomPosition[VY] = pos[VY]+deltaYbas;
	mGoalLength = longueur;

    mBottomAngle = utilitaire::RAD_TO_DEG(atan2(deltaYbas, deltaXbas)), 
    mTopAngle = utilitaire::RAD_TO_DEG(atan2(deltaYhaut, deltaXhaut));
    

	updateMatrice();
    updatePhysicBody();
    PositionSubject::signalObservers();
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
/// @fn float NoeudBut::obtenirRayon(  )
///
/// obtention du rayon
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float NoeudBut::obtenirRayon()
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
	v->modifierRayonAVerifier(mGoalLength);
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

    XMLUtils::ecrireAttribute<float>(elementNoeud,"longueurBut",mGoalLength);
    XMLUtils::ecrireAttribute<int>(elementNoeud,"joueur",joueur_);
    XMLUtils::ecrireVecteur3Dxml(&mTopPosition,elementNoeud,"coinHaut");
    XMLUtils::ecrireVecteur3Dxml(&mBottomPosition,elementNoeud,"coinBas");
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
	auto floatElem = mGoalLength;
    if(!XMLUtils::LireAttribute(element,"longueurBut",floatElem))
		return false;
	mGoalLength = floatElem;

	auto intElem = joueur_;
    if(!XMLUtils::LireAttribute(element,"joueur",intElem))
		return false;
	joueur_ = intElem;

    if( !XMLUtils::lectureVecteur3Dxml(&mTopPosition,element,"coinHaut") )
		return false;
    if( !XMLUtils::lectureVecteur3Dxml(&mBottomPosition,element,"coinBas") )
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudBut::obtenirHauteurBut()
///
/// Permet de connaitre la hauteur des buts pour savoir si la rondelle peut y passer
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float NoeudBut::obtenirHauteurBut()
{
	return mTopPosition[VY] - mBottomPosition[VY];
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
#if BOX2D_INTEGRATED

    clearPhysicsBody();

    float halfLength = getLength()/2.f*utilitaire::ratioWorldToBox2D;//(float)(coin2-coin1).norme()/2.f;

    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody; //this will be a dynamic body
    myBodyDef.position.Set(0, 0); //set the starting position
    myBodyDef.angle = 0; //set the starting angle

    mPhysicBody = getWorld()->CreateBody(&myBodyDef);
    Vecteur3 anchorPointPos = obtenirPositionAbsolue();
    b2Vec2 anchorPointPosB2, topPosB2,BottomPosB2 ;
    utilitaire::VEC3_TO_B2VEC(anchorPointPos,anchorPointPosB2);
    utilitaire::VEC3_TO_B2VEC(mTopPosition,topPosB2);
    utilitaire::VEC3_TO_B2VEC(mBottomPosition,BottomPosB2);
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

    updatePuckCatcher(mCachedPuckRadius);
    //     mPhysicBody->SetUserData(this);
    //     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::updatePuckCatcher( float puckRadius )
///
/// /*Description*/
///
/// @param[in] float puckRadius
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::updatePuckCatcher( float puckRadius )
{
#if BOX2D_INTEGRATED  
    if(!mPuckCatcher || mCachedPuckRadius != puckRadius)
    {
        if(mPuckCatcher)
        {
            getWorld()->DestroyBody(mPuckCatcher);
            mPuckCatcher = NULL;
        }

        float shiftValue = ( obtenirJoueur() == 1 ? -1 : 1 ) * puckRadius * 2;
        Vecteur3 anchorPointPosShifted = obtenirPositionAbsolue();
        Vecteur3 topPosShifted = mTopPosition;
        Vecteur3 bottomPosShifted = mBottomPosition;
        anchorPointPosShifted[VX] += shiftValue;
        topPosShifted[VX] += shiftValue;
        bottomPosShifted[VX] += shiftValue;

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.position.Set(0, 0); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPuckCatcher = getWorld()->CreateBody(&myBodyDef);

        b2Vec2 anchorPointPosShiftedB2, topPosB2,bottomPosB2, topPosShiftedB2, bottomPosShiftedB2;
        utilitaire::VEC3_TO_B2VEC(anchorPointPosShifted,anchorPointPosShiftedB2);
        utilitaire::VEC3_TO_B2VEC(mTopPosition,topPosB2);
        utilitaire::VEC3_TO_B2VEC(mBottomPosition,bottomPosB2);
        utilitaire::VEC3_TO_B2VEC(topPosShifted,topPosShiftedB2);
        utilitaire::VEC3_TO_B2VEC(bottomPosShifted,bottomPosShiftedB2);
        b2EdgeShape shape;

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shapeHaut above
        myFixtureDef.density = 1;
        myFixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
        myFixtureDef.filter.maskBits = CATEGORY_PUCK;
        myFixtureDef.filter.groupIndex = 0;

        shape.Set(topPosB2,topPosShiftedB2);
        mPuckCatcher->CreateFixture(&myFixtureDef); //add a fixture to the body
        shape.Set(topPosShiftedB2,anchorPointPosShiftedB2);
        mPuckCatcher->CreateFixture(&myFixtureDef); //add a fixture to the body
        shape.Set(anchorPointPosShiftedB2,bottomPosShiftedB2);
        mPuckCatcher->CreateFixture(&myFixtureDef); //add a fixture to the body
        shape.Set(bottomPosShiftedB2,bottomPosB2);
        mPuckCatcher->CreateFixture(&myFixtureDef); //add a fixture to the body

        mPuckCatcher->SetUserData(this);

        mCachedPuckRadius = puckRadius;
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudBut::clearPhysicsBody()
///
/// Libere la memoire de l'objet Box2D
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudBut::clearPhysicsBody()
{
    NoeudAbstrait::clearPhysicsBody();
#if BOX2D_INTEGRATED  
    if(mPuckCatcher)
    {
        getWorld()->DestroyBody(mPuckCatcher);
        mPuckCatcher = NULL;
    }
#endif
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
