////////////////////////////////////////////////
/// @file   NoeudAbstrait.cpp
/// @author DGI-2990
/// @date   2007-01-24
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////

#include "NoeudAbstrait.h"
#include "NoeudComposite.h"
#include "VisiteurCollision.h"
#include "GestionnaireModeles.h"
#include "DecodeString.h"
#include "Singleton.h"
#include "XMLUtils.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"

GLuint NoeudAbstrait::compteurIdGl_ = 1;
b2World* NoeudAbstrait::mWorld = NULL;


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAbstrait::NoeudAbstrait( const std::string& type /*= std::string( "" ) */ )
///
/// Ne fait qu'initialiser les variables membres de la classe.
///
/// @param[in] const std::string & type : Le type du noeud.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait::NoeudAbstrait(
   const std::string& type //= std::string( "" )
   ) :
    mPhysicBody(NULL),
    type_(type) ,
    modePolygones_(GL_FILL) ,
    mPosition(0) ,
    affiche_(true) ,
    selectionne_(false) ,
    selectionnable_(true) ,
    enregistrable_(true) ,
    parent_(0) , 
    mAngle(0) ,
    surligne_(false),
    glId_(compteurIdGl_++),
    terrain_(NULL)
{
	echelleCourante_[VX] = 1;
	echelleCourante_[VY] = 1;
	echelleCourante_[VZ] = 1;

	// On charge la matrice identitee lors de la construction
	// On garde en memoire la matrice d'origine (devrait etre NULL)
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, matrice_);
	// On remet en memoire la matrice d'origine
	glPopMatrix();

    glTypeId_ = GestionnaireModeles::obtenirInstance()->obtenirTypeIdFromName(type_);

	updateRayon();

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAbstrait::~NoeudAbstrait(  )
///
/// Destructeur vide déclaré virtuel pour les classes dérivées.  La
/// libération des afficheurs n'est pas la responsabilité de cette
/// classe.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait::~NoeudAbstrait()
{
	clearPhysicsBody();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudAbstrait::calculerProfondeur(  )
///
/// Cette fonction calcule la profondeur de l'arbre incluant le noeud
/// courant ainsi que tous ses enfants.
///
/// @return unsigned int : Cette fonction retourne toujours 1 pour un noeud sans enfant.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudAbstrait::calculerProfondeur() const
{
   return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::vider(  )
///
/// Cette fonction vide le noeud de tous ses enfants.
///
/// Cette fonction ne fait rien pour un noeud sans enfant, elle ne fait
/// donc rien dans cette implantation par défaut de la classe de base.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::vider()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::effacer( const NoeudAbstrait* noeud )
///
/// Cette fonction efface le noeud s'il fait partie des enfants de
/// ce noeud.
///
/// Cette fonction ne fait rien pour un noeud sans enfant, elle ne fait
/// donc rien dans cette implantation par défaut de la classe de base.
///
/// @param[in] const NoeudAbstrait * noeud : Le noeud à effacer.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::effacer( const NoeudAbstrait* noeud )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudAbstrait::chercher( const std::string& typeNoeud )
///
/// Cette fonction cherche un noeud d'un type donné parmi le noeud
/// lui-même et ses enfants.
///
/// Elle retourne donc le noeud lui-même si son type est celui passé en
/// paramètre, ou 0 sinon.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud à trouver.
///
/// @return const NoeudAbstrait* : Le pointeur vers le noeud s'il est trouvé.
///
////////////////////////////////////////////////////////////////////////
const NoeudAbstrait* NoeudAbstrait::chercher( const std::string& typeNoeud ) const
{
   if (typeNoeud == type_)
      return this;
   else
      return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudAbstrait::chercher( const std::string& typeNoeud )
///
/// Cette fonction cherche un noeud d'un type donné parmi le noeud
/// lui-même et ses enfants.
///
/// Elle retourne donc le noeud lui-même si son type est celui passé en
/// paramètre, ou 0 sinon.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud à trouver.
///
/// @return NoeudAbstrait* : Le pointeur vers le noeud s'il est trouvé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* NoeudAbstrait::chercher( const std::string& typeNoeud )
{
   if (typeNoeud == type_)
      return this;
   else
      return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudAbstrait::chercher( unsigned int indice )
///
/// Cette fonction cherche le i-ème enfant d'un noeud.
///
/// Elle retourne toujours 0 pour la classe de base, car cette
/// dernière ne possède pas d'enfant.
///
/// @param[in] unsigned int indice : L'indice du noeud à trouver.
///
/// @return const NoeudAbstrait* : Le pointeur vers le noeud s'il est trouvé.
///
////////////////////////////////////////////////////////////////////////
const NoeudAbstrait* NoeudAbstrait::chercher( unsigned int indice ) const
{
   return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudAbstrait::chercher( unsigned int indice )
///
/// Cette fonction cherche le i-ème enfant d'un noeud.
///
/// Elle retourne toujours 0 pour la classe de base, car cette
/// dernière ne possède pas d'enfant.
///
/// @param[in] unsigned int indice : L'indice du noeud à trouver.
///
/// @return NoeudAbstrait* : Le pointeur vers le noeud s'il est trouvé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* NoeudAbstrait::chercher( unsigned int indice )
{
   return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::ajouter( NoeudAbstrait* enfant )
///
/// Elle retourne toujours faux et ne fait rien, car ce type de noeud
/// abstrait ne peut pas avoir d'enfant.
///
/// @param[in] NoeudAbstrait * enfant : Le noeud à ajouter.
///
/// @return bool : Vrai si l'ajout a bien été effectué, faux autrement.
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::ajouter(NoeudAbstrait* enfant)
{
   return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudAbstrait::obtenirNombreEnfants(  )
///
/// Cette fonction retourne le nombre d'enfants de ce noeud.
///
/// Elle retourne toujours 0, car ce type de noeud abstrait ne peut pas
/// avoir d'enfant.
///
/// @return unsigned int : Vrai si l'ajout a bien été effectué, faux autrement.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudAbstrait::obtenirNombreEnfants() const
{
   return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::inverserSelection(  )
///
/// Cette fonction inverse l'état de sélection de ce noeud.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::inverserSelection()
{
   selectionne_ = !selectionne_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::selectionnerTout(  )
///
/// Cette fonction sélectionne le noeud et ses enfants.
///
/// Elle ne fait que sélectionner le noeud pour cette classe, car ce
/// type de noeud abstrait ne peut pas avoir d'enfants.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::selectionnerTout()
{
   assignerSelection(true);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::deselectionnerTout(  )
///
/// Cette fonction désélectionne le noeud et ses enfants.
///
/// Elle ne fait que désélectionner le noeud pour cette classe, car ce
/// type de noeud abstrait ne peut pas avoir d'enfants.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::deselectionnerTout()
{
   selectionne_ = false;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::selectionExiste(  )
///
/// Cette fonction vérifie si le noeud ou un de ses enfants est
/// sélectionné.
///
/// Elle ne fait que regarder si le noeud est sélectionné, car ce type
/// de noeud abstrait ne peut pas avoir d'enfants.
///
///
/// @return bool : Vrai s'il existe un noeud sélectionné, faux autrement.
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::selectionExiste() const
{
   return selectionne_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::changerModePolygones( bool estForce )
///
/// Cette fonction change le mode de rendu des polygones du noeud s'il
/// est sélectionné ou si on le force.
///
/// @param[in] bool estForce : Vrai si on veut changer le mode même si le
///								noeud n'est pas sélectionné.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::changerModePolygones( bool estForce )
{
   if ( ( estForce ) || ( estSelectionne() ) ) {
      if ( modePolygones_ == GL_FILL )
         modePolygones_ =  GL_LINE;
      else if ( modePolygones_ == GL_LINE )
         modePolygones_ = GL_POINT;
      else
         modePolygones_ = GL_FILL;
   }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::assignerModePolygones( GLenum modePolygones )
///
/// Cette fonction assigne le mode de rendu des polygones du noeud.
///
/// @param[in] GLenum modePolygones : Le mode de polygone voulu, donc 
///										fil de fer ou plein, etc.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::assignerModePolygones( GLenum modePolygones )
{
   // Le mode par défaut si on passe une mauvaise valeur est GL_FILL
   if ( (modePolygones != GL_FILL) &&
        (modePolygones != GL_LINE) &&
        (modePolygones != GL_POINT) ) {
      modePolygones = GL_FILL;
   }

   modePolygones_ = modePolygones;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::afficher(  )
///
/// Cette fonction affiche le noeud comme tel.
///
/// Elle consiste en une template method (dans le sens du patron de
/// conception, et non les template C++) qui effectue ce qui est
/// généralement à faire pour l'affichage, c'est-à-dire:
/// - Mise en pile de la matrice de transformation
/// - Translation du noeud pour qu'il soit à sa position relative
/// - Utilisation du mode d'affichage des polygones
/// - ...
/// - Restauration de l'état.
///
/// L'affichage comme tel est confié à la fonction afficherConcret(),
/// appelée par la fonction afficher().
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::afficher() const
{
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);
      
    // Assignation du mode d'affichage des polygones
    glPolygonMode( GL_FRONT_AND_BACK, modePolygones_ );
      
    // Affichage concret
    afficherConcret();
      
    // Restauration
    glPopAttrib();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::afficherConcret()
///
/// Cette fonction effectue le véritable rendu de l'objet.  Elle est
/// appelée par la template method (dans le sens du patron de conception,
/// et non des template C++) afficher() de la classe de base.
///
/// @param[in] GLuint liste : liste d'affichage
/// @param[in] GLuint listeSelection : liste d'affichage sans texture
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::afficherConcret() const
{
    if(estAffiche())
    {
        GLuint liste;
        GestionnaireModeles::obtenirInstance()->obtenirListe(get3DModelKey(),liste);
        // Si aucune liste n'est trouvé, on sort de la fonction.
        if(liste==NULL)
            return;
        
        glTranslated(mPosition[0], mPosition[1], mPosition[2]);
        glPushMatrix();

        // Effectue les mise a l'echelle et les rotations
        glMultMatrixd(matrice_);

        glPushAttrib( GL_ALL_ATTRIB_BITS );
        if(surligne_ || selectionne_)
        {
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_COPY_INVERTED);
        }

        // Push du id du noeud sur la pile de nom
        glPushName(glId_);
        // Push du id du type du noeud sur la pile de nom
        glPushName(glTypeId_);

        // Efface le stencil et l'active
        glClearStencil(0);
        glClear( GL_STENCIL_BUFFER_BIT );
        glEnable( GL_STENCIL_TEST );
        // A chaque fois qu'un pixel est dessine a l'ecran, le stencil met le bit a 1
        glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
        glCallList(liste); // Dessin de l'objet avec les textures
        glPopName();
        glPopName();

        glDisable(GL_STENCIL_TEST);
        // Restauration des attributs
        glPopAttrib();

        glPopMatrix();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::animer( const float& dt )
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// Elle ne fait rien pour cette classe et vise à être surcharger par
/// les classes dérivées.
///
/// @param[in] float dt : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::animer( const float& dt )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::obtenirMatrice( GLdouble* matriceRetour )
///
/// Cette fonction met la matrice dans la matrice donne en parametre.
///
/// @param[in] GLdouble * matriceRetour : matrice de retour dans 
/// laquelle la matrice du noeud va etre copiee.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::obtenirMatrice(GLdouble* matriceRetour) const
{
	for(int i=0; i<16; i++)
		matriceRetour[i] = matrice_[i];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::modifierMatrice( GLdouble *matrice )
///
/// Cette fonction modifie la matrice de transformation du noeud.
///
/// @param[in] GLdouble * matrice : matrice a copier dans la matrice du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::modifierMatrice(GLdouble *matrice)
{
	for(int i=0; i<16; i++)
		matrice_[i] = matrice[i];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::obtenirEchelleCourante( Vecteur3& echelleCourante )
///
/// Retourne l'echelle courante du noeud.
///
/// @param[out] Vecteur3 & echelleCourante : Un vecteur ou est assigner l'echelle
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::obtenirEchelleCourante( Vecteur3& echelleCourante ) const
{
	echelleCourante[VX] = echelleCourante_[VX];
	echelleCourante[VY] = echelleCourante_[VY];
	echelleCourante[VZ] = echelleCourante_[VZ];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::modifierEchelleCourante( Vecteur3 echelleCourante )
///
/// Assigne la nouvelle echelle.
///
/// @param[in] Vecteur3 echelleCourante : Le vecteur contenant l'echelle a assigner
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::modifierEchelleCourante( const Vecteur3& echelleCourante )
{
	echelleCourante_[VX] = echelleCourante[VX];
	echelleCourante_[VY] = echelleCourante[VY];
	echelleCourante_[VZ] = echelleCourante[VZ];
	updateMatrice();
    updatePhysicBody();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::assignerParent( NoeudComposite* parent )
///
/// Cette fonction assigne le parent du noeud afin qu'il soit possible
/// de remonter dans l'arbre.
///
/// @param[in] NoeudComposite * parent : Le parent du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::assignerParent( NoeudComposite* parent )
{
	parent_ = parent;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::assignerAngle( int angle )
///
/// Assigne l'angle de rotation.
///
/// @param[in] float angle : nouvel angle de rotation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::assignerAngle( const float& angle )
{
	mAngle = angle;
#if BOX2D_INTEGRATED  
    if(mPhysicBody)
    {
        mPhysicBody->SetTransform(mPhysicBody->GetPosition(),(float32)utilitaire::DEG_TO_RAD(mAngle));
    }
#endif
	updateMatrice();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudAbstrait::obtenirAngle(  )
///
/// retourne l'angle de rotation.
///
/// @return float : l'angle en degree
///
////////////////////////////////////////////////////////////////////////
float NoeudAbstrait::obtenirAngle() const
{
	return mAngle;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::updateMatrice(  )
///
/// Met a jour la matrice de transformation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::updateMatrice()
{
	glPushMatrix();
	glLoadIdentity();

	glRotated(mAngle, 0.0, 0.0, 1.0);
	glScaled(echelleCourante_[VX], echelleCourante_[VY], echelleCourante_[VZ]);
	
	glGetDoublev(GL_MODELVIEW_MATRIX, matrice_); // Savegarde de la matrice courante dans le noeud

	glPopMatrix();
	updateRayon();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait::PaireVect3 NoeudAbstrait::obtenirZoneOccupee(  )
///
/// Retourne la zone occupee par le noeud (positions min et max en X et en Y).
///
///
/// @return NoeudAbstrait::PaireVect3 : paire de Vecteur3 contenant les mins et maxs et coordonnees virtuelles.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait::PaireVect3 NoeudAbstrait::obtenirZoneOccupee() const
{
	
	Vecteur3 minimum, maximum;
	float largeur, hauteur;
    Modele3D* pModel = obtenirModele();
    if(pModel)
    {
        pModel->calculerBoiteEnglobante(minimum, maximum);
        largeur = echelleCourante_[VX]*(maximum[VX]-minimum[VX]);
        hauteur = echelleCourante_[VY]*(maximum[VY]-minimum[VY]);

        // On multiplie par l'echelle maximale
        minimum[VX] = getPosition()[VX]-(largeur*cos(((int)mAngle%90)*(float)M_PI/180))/2;
        maximum[VX] = getPosition()[VX]+(largeur*cos(((int)mAngle%90)*(float)M_PI/180))/2;

        minimum[VY] = getPosition()[VY]-(largeur*sin(((int)mAngle%90)*(float)M_PI/180))/2;
        maximum[VY] = getPosition()[VY]+(largeur*sin(((int)mAngle%90)*(float)M_PI/180))/2;

        if(maximum[VY]-minimum[VY]<hauteur)
        {
            minimum[VY] = getPosition()[VY]-hauteur/2;
            maximum[VY] = getPosition()[VY]+hauteur/2;

        }
    }

	return PaireVect3(minimum, maximum);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudAbstrait::obtenirRayon(  )
///
/// Retourne le rayon du noeud
///
///
/// @return float : Le rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
float NoeudAbstrait::obtenirRayon() const
{
	return rayon_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudAbstrait::updateRayon(  )
///
/// Recalcule le rayon
///
///
/// @return float : Le rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::updateRayon()
{
	rayon_ = getDefaultRadius()*max(echelleCourante_[VX], echelleCourante_[VY]);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::assignerAttributVisiteurCollision( VisiteurCollision* v )
///
/// Permet d'assigner les attribut nécessaire à la collision.
///
/// @param[in] VisiteurCollision * v : Visiteur ayant besoin d'avoir ses paramètres assigne.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::assignerAttributVisiteurCollision( VisiteurCollision* v )
{
	v->modifierTypeCollision(CERCLE);
	v->modifierRayonAVerifier(obtenirRayon());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::modifierSurligner( bool estSurligner )
///
/// Modifie la surbrillance.
///
/// @param[in] bool estSurligner : true si le noeud est en collision.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::modifierSurligner( const bool& estSurligner )
{
	surligne_ = estSurligner;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::estSurligne(  )
///
/// Retourne true si le noeud est en collision.
///
///
/// @return bool : true si collision.
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::estSurligne()
{
	return surligne_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::possedeSelection(  )
///
/// Retourne true si le noeud est selectionne.
///
///
/// @return bool : true si la noeud est selectionne.
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::possedeSelection()
{
	return selectionne_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudAbstrait::creerNoeudXML(  )
///
/// Creation du noeud XML d'un noeud abstrait
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudAbstrait::creerNoeudXML()
{
	XmlElement* elementNoeud = XMLUtils::createNode(type_.c_str());
    XMLUtils::writeArray(mPosition.c_arr(),3,elementNoeud,"pos");
    XMLUtils::writeArray(echelleCourante_.c_arr(),3,elementNoeud,"echelle");
    XMLUtils::writeAttribute(elementNoeud,"angle",mAngle);
    XMLUtils::writeAttribute(elementNoeud,"affiche",affiche_);
    XMLUtils::writeAttribute(elementNoeud,"selectionnable",selectionnable_);

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::initialiser( const XmlElement* element )
///
/// Initialisation du NoeudAbstrait à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::initialiser( const XmlElement* element )
{
	int intElem;
	float floatElem;
	Vecteur3 pos;
    
	if( !XMLUtils::readArray(pos.c_arr(),3,element,"pos") )
		return false;
	setPosition(pos);
    if( !XMLUtils::readArray(echelleCourante_.c_arr(),3,element,"echelle") )
        return false;

    if( !XMLUtils::readAttribute(element,"angle",floatElem) )
        return false;
    assignerAngle(floatElem);

	if( !XMLUtils::readAttribute(element,"affiche",intElem) )
		return false;
    assignerAffiche(intElem==1);

    if( !XMLUtils::readAttribute(element,"selectionnable",intElem) )
		return false;
    assignerEstSelectionnable(intElem==1);
	
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::modifierTerrain( Terrain* val )
///
/// Modificateur de terrain_
///
/// @param[in] Terrain * val : nouvelle valeur du terrain
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::modifierTerrain( Terrain* val )
{
	terrain_ = val;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setPosition( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setPosition( const Vecteur3& positionRelative )
{
    mPosition = positionRelative;
#if BOX2D_INTEGRATED  
    if(mPhysicBody)
    {
        b2Vec2 pos;
        utilitaire::VEC3_TO_B2VEC(mPosition,pos);
        mPhysicBody->SetTransform(pos,mPhysicBody->GetAngle());
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Modele3D* NoeudAbstrait::obtenirModele()
///
/// /*Description*/
///
///
/// @return Modele3D*
///
////////////////////////////////////////////////////////////////////////
Modele3D* NoeudAbstrait::obtenirModele() const
{
    return GestionnaireModeles::obtenirInstance()->obtenirModele(get3DModelKey());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::clearPhysicsBody()
///
/// Release memory of the physics body
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::clearPhysicsBody()
{
#if BOX2D_INTEGRATED  
    if(mPhysicBody && getWorld())
    {
        getWorld()->DestroyBody(mPhysicBody);
    }
    mPhysicBody = NULL;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::forceFullUpdate()
///
/// Recreates everything needed for the game
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::forceFullUpdate()
{
    updateRayon();
    updateMatrice();
    updatePhysicBody();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::SynchroniseTransformFromB2CallBack( void* , const struct b2Transform& )
///
/// /*Description*/
///
/// @param[in] void *
/// @param[in] const struct b2Transform &
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::SynchroniseTransformFromB2CallBack( void* node, const b2Transform& transform)
{
#if BOX2D_INTEGRATED 
    if(node)
    {
        ((NoeudAbstrait*)node)->SynchroniseTransformFromB2(transform);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::SynchroniseTransformFromB2( const struct b2Transform& )
///
/// /*Description*/
///
/// @param[in] const struct b2Transform &
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::SynchroniseTransformFromB2( const b2Transform& transform)
{
#if BOX2D_INTEGRATED 
    // TODO:: a verifier avec la position du parent
    utilitaire::B2VEC_TO_VEC3(mPosition,transform.p);
    mAngle = utilitaire::RAD_TO_DEG(transform.q.GetAngle());
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::acceptVisitor( class VisiteurNoeud& v )
///
/// Accepte un visiteur sur le noeud
///
/// @param[in] class VisiteurNoeud & v
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::acceptVisitor( class VisiteurNoeud& v )
{
    v.visiterNoeudAbstrait(this);
}



////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////
