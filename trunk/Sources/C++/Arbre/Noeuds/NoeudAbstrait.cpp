////////////////////////////////////////////////
/// @file   NoeudAbstrait.cpp
/// @author DGI-2990
/// @date   2007-01-24
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////

#if WIN32
#include "GestionnaireModeles.h"
#include "DecodeString.h"
#include "Singleton.h"
#include "XMLUtils.h"

#endif

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif

#include "NoeudAbstrait.h"
#include "NoeudComposite.h"
#include "VisiteurCollision.h"
#include "ExceptionJeu.h"
#include "Utilitaire.h"
#include "Terrain.h"
#include "BonusModifierAbstract.h"



GLuint NoeudAbstrait::mIdGlCounter = 1;

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAbstrait::NoeudAbstrait( const std::string& type /*= std::string( "" ) */ )
///
/// Ne fait qu'initFromXml les variables membres de la classe.
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
    mType(type) ,
    mPosition(0) ,
    mFlags(1<<NODEFLAGS_VISIBLE|1<<NODEFLAGS_CAN_BE_SELECTED|1<<NODEFLAGS_RECORDABLE|1<<NODEFLAGS_CAN_BE_DELETED),
    mParent(0) , 
    mAngle(0) ,
    mGlId(mIdGlCounter++),
    mField(NULL),
    mWorld(NULL)
{
	mScale[VX] = 1;
	mScale[VY] = 1;
	mScale[VZ] = 1;

#if WIN32
    mModePolygones = GL_FILL;
    mGlTypeId = GestionnaireModeles::obtenirInstance()->obtenirTypeIdFromName(mType);
#endif
	// On charge la matrice identitee lors de la construction
	// On garde en memoire la matrice d'origine (devrait etre NULL)
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, mTransformationMatrix);
	// On remet en memoire la matrice d'origine
	glPopMatrix();

	updateRadius();

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
    setField(NULL);

    // clean remaining modifiers in the node
    for(auto it = mModifiers.begin(); it != mModifiers.end(); ++it)
    {
        delete *it;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudAbstrait::treeDepth(  )
///
/// Cette fonction calcule la profondeur de l'arbre incluant le noeud
/// courant ainsi que tous ses enfants.
///
/// @return unsigned int : Cette fonction retourne toujours 1 pour un noeud sans enfant.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudAbstrait::treeDepth() const
{
   return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::empty(  )
///
/// Cette fonction vide le noeud de tous ses enfants.
///
/// Cette fonction ne fait rien pour un noeud sans enfant, elle ne fait
/// donc rien dans cette implantation par défaut de la classe de base.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::empty()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::erase( const NoeudAbstrait* noeud )
///
/// Cette fonction efface le noeud s'il fait partie des enfants de
/// ce noeud.
///
/// Cette fonction ne fait rien pour un noeud sans enfant, elle ne fait
/// donc rien dans cette implantation par défaut de la classe de base.
///
/// @param[in] const NoeudAbstrait * noeud : Le noeud à erase.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::erase( const NoeudAbstrait* noeud )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::deleteThis()
///
/// correctly delete a node and removes it form its parent.
/// do not call in destructor because the parent might be initialising the
// destruction.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::deleteThis()
{
    // On enleve les noeud selectionné et tous ces enfants.
    // Si on ne veut pas enlever les enfants il faudrait modifier
    // la méthode erase() pour que les enfants soit relié au parent
    auto terrain = getField();
    if(!getParent() || !getParent()->erase(this))
    {
        /// le parent a peut etre retirer le terrain ce qui pourrait invalider la libération de la mémoire
        setField(terrain);
        empty();
        delete this;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudAbstrait::find( const std::string& typeNoeud )
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
const NoeudAbstrait* NoeudAbstrait::find( const std::string& typeNoeud ) const
{
   if (typeNoeud == mType)
      return this;
   else
      return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudAbstrait::find( const std::string& typeNoeud )
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
NoeudAbstrait* NoeudAbstrait::find( const std::string& typeNoeud )
{
   if (typeNoeud == mType)
      return this;
   else
      return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudAbstrait::find( unsigned int indice )
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
const NoeudAbstrait* NoeudAbstrait::find( unsigned int indice ) const
{
   return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudAbstrait::find( unsigned int indice )
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
NoeudAbstrait* NoeudAbstrait::find( unsigned int indice )
{
   return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::add( NoeudAbstrait* enfant )
///
/// Elle retourne toujours faux et ne fait rien, car ce type de noeud
/// abstrait ne peut pas avoir d'enfant.
///
/// @param[in] NoeudAbstrait * enfant : Le noeud à add.
///
/// @return bool : Vrai si l'ajout a bien été effectué, faux autrement.
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::add(NoeudAbstrait* enfant)
{
   return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudAbstrait::childCount(  )
///
/// Cette fonction retourne le nombre d'enfants de ce noeud.
///
/// Elle retourne toujours 0, car ce type de noeud abstrait ne peut pas
/// avoir d'enfant.
///
/// @return unsigned int : Vrai si l'ajout a bien été effectué, faux autrement.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudAbstrait::childCount() const
{
   return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::toggleSelection(  )
///
/// Cette fonction inverse l'état de sélection de ce noeud.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::toggleSelection()
{
    setSelection(!IsSelected());
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::selectAll(  )
///
/// Cette fonction sélectionne le noeud et ses enfants.
///
/// Elle ne fait que sélectionner le noeud pour cette classe, car ce
/// type de noeud abstrait ne peut pas avoir d'enfants.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::selectAll()
{
   setSelection(true);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::deselectAll(  )
///
/// Cette fonction désélectionne le noeud et ses enfants.
///
/// Elle ne fait que désélectionner le noeud pour cette classe, car ce
/// type de noeud abstrait ne peut pas avoir d'enfants.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::deselectAll()
{
    setSelection(false);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::selectionPresent(  )
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
bool NoeudAbstrait::selectionPresent() const
{
   return IsSelected();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::cyclePolygonMode( bool estForce )
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
void NoeudAbstrait::cyclePolygonMode( bool estForce )
{
#if WIN32
   if ( ( estForce ) || ( IsSelected() ) ) {
      if ( mModePolygones == GL_FILL )
         mModePolygones =  GL_LINE;
      else if ( mModePolygones == GL_LINE )
         mModePolygones = GL_POINT;
      else
         mModePolygones = GL_FILL;
   }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setPolygonMode( GLenum modePolygones )
///
/// Cette fonction assigne le mode de rendu des polygones du noeud.
///
/// @param[in] GLenum modePolygones : Le mode de polygone voulu, donc 
///										fil de fer ou plein, etc.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setPolygonMode( GLenum modePolygones )
{
#if WIN32
   // Le mode par défaut si on passe une mauvaise valeur est GL_FILL
   if ( (modePolygones != GL_FILL) &&
        (modePolygones != GL_LINE) &&
        (modePolygones != GL_POINT) ) {
      modePolygones = GL_FILL;
   }

   mModePolygones = modePolygones;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::render(  )
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
/// L'affichage comme tel est confié à la fonction renderReal(),
/// appelée par la fonction render().
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::render() const
{
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);
      
    // Assignation du mode d'affichage des polygones
    glPolygonMode( GL_FRONT_AND_BACK, mModePolygones );
      
    // Affichage concret
    renderReal();
      
    // Restauration
    glPopAttrib();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::renderReal()
///
/// Cette fonction effectue le véritable rendu de l'objet.  Elle est
/// appelée par la template method (dans le sens du patron de conception,
/// et non des template C++) render() de la classe de base.
///
/// @param[in] GLuint liste : liste d'affichage
/// @param[in] GLuint listeSelection : liste d'affichage sans texture
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::renderReal() const
{
    if(isVisible())
    {
#if !__APPLE__
        // Renders all the modifiers present on the node
        for(auto it = mModifiers.begin(); it != mModifiers.end(); ++it)
        {
            (*it)->render();
        }

        GLuint liste;	
        GestionnaireModeles::obtenirInstance()->obtenirListe(get3DModelKey(),liste);
        // Si aucune liste n'est trouvé, on sort de la fonction.
        if(liste==NULL)
            return;
#endif
        glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
        glPushMatrix();

        // Effectue les mise a l'echelle et les rotations
        glMultMatrixf(mTransformationMatrix);

        glPushAttrib( GL_ALL_ATTRIB_BITS );
        if(isHightlighted() || IsSelected())
        {
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_COPY_INVERTED);
        }

#if !__APPLE__
        GLint renderMode;
        glGetIntegerv(GL_RENDER_MODE,&renderMode);
        if(renderMode == GL_SELECT && canBeSelected())
        {
            // Push du id du noeud sur la pile de nom
            glPushName(mGlId);
            // Push du id du type du noeud sur la pile de nom
            glPushName(mGlTypeId);
            glCallList(liste); // Dessin de l'objet avec les textures
            glPopName();
            glPopName();
        }
        else
        {
            glCallList(liste); // Dessin de l'objet avec les textures
        }


#else
        auto field = getField();
        if(!field || !field->renderAppleNode(RazerGameUtilities::StringToKey(get3DModelKey())))
        {
            renderOpenGLES();
        }
        
#endif
        if(isHightlighted() || IsSelected())
        {
            glDisable(GL_COLOR_LOGIC_OP);
        }
        // Restauration des attributs
        glPopAttrib();

        glPopMatrix();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::tick( const float& dt )
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
void NoeudAbstrait::tick( const float& dt )
{

}
////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::getScale( Vecteur3& echelleCourante )
///
/// Retourne l'echelle courante du noeud.
///
/// @param[out] Vecteur3 & echelleCourante : Un vecteur ou est assigner l'echelle
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::getScale( Vecteur3& echelleCourante ) const
{
	echelleCourante[VX] = mScale[VX];
	echelleCourante[VY] = mScale[VY];
	echelleCourante[VZ] = mScale[VZ];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setScale( Vecteur3 echelleCourante )
///
/// Assigne la nouvelle echelle.
///
/// @param[in] Vecteur3 echelleCourante : Le vecteur contenant l'echelle a assigner
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setScale( const Vecteur3& echelleCourante )
{
	mScale[VX] = echelleCourante[VX];
	mScale[VY] = echelleCourante[VY];
	mScale[VZ] = echelleCourante[VZ];
    updateRadius();
    updateMatrice();
    if(!isWorldLocked())
    {
        updatePhysicBody();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setParent( NoeudComposite* parent )
///
/// Cette fonction assigne le parent du noeud afin qu'il soit possible
/// de remonter dans l'arbre.
///
/// @param[in] NoeudComposite * parent : Le parent du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setParent( NoeudComposite* parent )
{
	mParent = parent;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setAngle( float angle )
///
/// Assigne l'angle de rotation.
///
/// @param[in] float angle : nouvel angle de rotation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setAngle( float angle )
{
	mAngle = angle;
#if BOX2D_INTEGRATED  
    if(!isWorldLocked() && mPhysicBody)
    {
        mPhysicBody->SetTransform(mPhysicBody->GetPosition(),(float32)utilitaire::DEG_TO_RAD(mAngle));
    }
#endif
	updateMatrice();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudAbstrait::getAngle(  )
///
/// retourne l'angle de rotation.
///
/// @return float : l'angle en degree
///
////////////////////////////////////////////////////////////////////////
float NoeudAbstrait::getAngle() const
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

	glRotatef(mAngle, 0.0, 0.0, 1.0);
	glScalef(mScale[VX], mScale[VY], mScale[VZ]);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, mTransformationMatrix); // Savegarde de la matrice courante dans le noeud

	glPopMatrix();
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
#if WIN32
	float largeur, hauteur;
    Modele3D* pModel = getModel();
    if(pModel)
    {
        pModel->calculerBoiteEnglobante(minimum, maximum);
        largeur = mScale[VX]*(maximum[VX]-minimum[VX]);
        hauteur = mScale[VY]*(maximum[VY]-minimum[VY]);

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
#endif
	return PaireVect3(minimum, maximum);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudAbstrait::getRadius(  )
///
/// Retourne le rayon du noeud
///
///
/// @return float : Le rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
float NoeudAbstrait::getRadius() const
{
	return mRadius;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudAbstrait::updateRadius(  )
///
/// Recalcule le rayon
///
///
/// @return float : Le rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::updateRadius()
{
	mRadius = getDefaultRadius()*utilitaire::borneSuperieure(mScale[VX], mScale[VY]);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setCollisionVisitorAttributes( VisiteurCollision* v )
///
/// Permet d'assigner les attribut nécessaire à la collision.
///
/// @param[in] VisiteurCollision * v : Visiteur ayant besoin d'avoir ses paramètres assigne.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setCollisionVisitorAttributes( class VisiteurCollision* v )
{
	v->modifierTypeCollision(CERCLE);
	v->modifierRayonAVerifier(getRadius());
}


////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudAbstrait::createXmlNode(  )
///
/// Creation du noeud XML d'un noeud abstrait
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudAbstrait::createXmlNode()
{
	XmlElement* elementNoeud = XMLUtils::createNode(mType.c_str());
    XmlWriteNodePosition(elementNoeud);
    XMLUtils::writeArray(mScale.c_arr(),3,elementNoeud,"echelle");
    XMLUtils::writeAttribute(elementNoeud,"angle",mAngle);

    XMLUtils::writeAttribute(elementNoeud,"affiche",isVisible());
    XMLUtils::writeAttribute(elementNoeud,"selectionnable",canBeSelected());
    XMLUtils::writeAttribute(elementNoeud,"selected",IsSelected());

	return elementNoeud;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::XmlWriteNodePosition( XmlElement* elementNoeud )
///
/// /*Description*/
///
/// @param[in] XmlElement * elementNoeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::XmlWriteNodePosition( XmlElement* elementNoeud )
{
    XMLUtils::writeArray(mPosition.c_arr(),3,elementNoeud,"pos");
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::XmlReadNodePosition( const XmlElement* element )
///
/// /*Description*/
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::XmlReadNodePosition( Vecteur3& pos, const XmlElement* element )
{
    return XMLUtils::readArray(pos.c_arr(),3,element,"pos");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudAbstrait à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::initFromXml( const XmlElement* element )
{
	int intElem;
	float floatElem;
    Vecteur3 pos;
    if(!XmlReadNodePosition(pos,element))
		throw ExceptionJeu("%s: Error reading node's position", mType.c_str());
	setPosition(pos);
    if( !XMLUtils::readArray(mScale.c_arr(),3,element,"echelle") )
        throw ExceptionJeu("%s: Error reading node's scale", mType.c_str());

    if( !XMLUtils::readAttribute(element,"angle",floatElem) )
        throw ExceptionJeu("%s: Error reading node's angle", mType.c_str());
    setAngle(floatElem);
    if( !XMLUtils::readAttribute(element,"affiche",intElem) )
        throw ExceptionJeu("%s: Error reading node's visibility flag", mType.c_str());
    setVisible(intElem==1);

    if( !XMLUtils::readAttribute(element,"selectionnable",intElem) )
        throw ExceptionJeu("%s: Error reading node's selection flag", mType.c_str());
    setCanBeSelected(intElem==1);
    
    if( !XMLUtils::readAttribute(element,"selected",intElem) )
        intElem = 0; // backward compatibility
    setSelection(intElem==1);
	
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::setField( Terrain* val )
///
/// Modificateur de mField
///
/// @param[in] Terrain * val : nouvelle valeur du terrain
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setField( Terrain* val )
{
    if(mField)
    {
        mField->NodeSelectionNotification(this,false);
    }
	mField = val;
    if(mField)
    {
        mWorld = mField->GetWorld();
        mField->NodeSelectionNotification(this,IsSelected());
    }
    SetIsInGame(mField && mField->IsGameField());
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
    if(!isWorldLocked() && mPhysicBody)
    {
        b2Vec2 pos;
        utilitaire::VEC3_TO_B2VEC(mPosition,pos);
        mPhysicBody->SetTransform(pos,mPhysicBody->GetAngle());
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Modele3D* NoeudAbstrait::getModel()
///
/// /*Description*/
///
///
/// @return Modele3D*
///
////////////////////////////////////////////////////////////////////////
Modele3D* NoeudAbstrait::getModel() const
{
#if WIN32
    return GestionnaireModeles::obtenirInstance()->obtenirModele(get3DModelKey());
#else
    return NULL;
#endif
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
    auto world = getWorld();
    if(mPhysicBody && world)
    {
        world->DestroyBody(mPhysicBody);
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
    updateRadius();
    updateMatrice();
    if(!isWorldLocked())
    {
        updatePhysicBody();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::synchroniseTransformFromB2CallBack( void* , const struct b2Transform& )
///
/// /*Description*/
///
/// @param[in] void *
/// @param[in] const struct b2Transform &
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::synchroniseTransformFromB2CallBack( void* node, const b2Transform& transform)
{
#if BOX2D_INTEGRATED 
    if(node)
    {
        ((NoeudAbstrait*)node)->synchroniseTransformFromB2(transform);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::synchroniseTransformFromB2( const struct b2Transform& )
///
/// /*Description*/
///
/// @param[in] const struct b2Transform &
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::synchroniseTransformFromB2( const b2Transform& transform)
{
#if BOX2D_INTEGRATED 
    // TODO:: a verifier avec la position du parent
    Vecteur3 pos;
    utilitaire::B2VEC_TO_VEC3(pos,transform.p);
    setPosition(pos);
    setAngle(utilitaire::RAD_TO_DEG(transform.q.GetAngle()));
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

////////////////////////////////////////////////////////////////////////
///
/// @fn class ArbreRendu* NoeudAbstrait::GetTreeRoot()
///
/// Search for the tree root of the node
///
///
/// @return class ArbreRendu*
///
////////////////////////////////////////////////////////////////////////
const class ArbreRendu* NoeudAbstrait::GetTreeRoot() const
{
    if(mParent)
    {
        return mParent->GetTreeRoot();
    }
    // tree root not found
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::equals( NoeudAbstrait* )
///
/// fonction de comparaison de 2 noeuds
///
/// @param[in] NoeudAbstrait *
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::equals( NoeudAbstrait* n)
{
    return !!n &&
    mType == n->mType &&
    mModePolygones == n->mModePolygones &&
    // permet de donner une certaine tolerance
    (mPosition-n->mPosition).norme2() < 1 &&
    mFlags == n->mFlags &&
    mScale == n->mScale &&
    mRadius == n->mRadius;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::renderOpenGLES()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::renderOpenGLES() const
{
    const int segments = 10;
    static const float jump = 2*utilitaire::PI/(float)segments;
    const float radius = getRadius();
    const int count=segments*2;
    GLfloat vertices[count];
    int i = 0;
    for (float deg=0; i < count; i+=2, deg+=jump)
    {
        vertices[i] = (cos(deg)*radius);
        vertices[i+1] = (sin(deg)*radius);
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer (2, GL_FLOAT , 0, vertices); 
    glDrawArrays (GL_TRIANGLE_FAN, 0, segments);
    glDisableClientState(GL_VERTEX_ARRAY);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn inline void NoeudAbstrait::setSelection( bool selectionne )
///
/// Cette fonction permet d'assigner l'état d'être sélectionné ou non du noeud.
///
/// @param[in] bool selectionne : L'état sélectionné ou non.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::setSelection( bool selectionne )
{
    bool oldSelection = IsSelected();
    // Un objet non sélectionnable n'est jamais sélectionné.
    mFlags.SetFlag(selectionne && canBeSelected(),NODEFLAGS_SELECTED);
    auto terrain = getField();
    if(terrain && oldSelection != IsSelected())
    {
        terrain->NodeSelectionNotification(this,!oldSelection);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::activate( bool activate )
///
/// determines if this node is active in the simulation
///
/// @param[in] bool activate
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::activate( bool activate )
{
    mFlags.SetFlag(activate,NODEFLAGS_ACTIVE);
#if BOX2D_INTEGRATED  
    if(mPhysicBody)
    {
        mPhysicBody->SetActive(activate);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstrait::playTick( float temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstrait::playTick( float temps )
{
    // Some modifier might not need any tick so we clean
    // finished modifier first
    // ie. instant action or refreshing bonuses

    // remove all modifiers that have completed their jobs
    // Clean way to remove items in a set while iterating
    for (auto it = mModifiers.begin(); it != mModifiers.end(); ) 
    {
        if ((*it)->IsFinished()) 
        {
            delete *it;
            mModifiers.erase(it++);
        }
        else 
        {
            ++it;
        }
    }

    // Tick all the modifiers present on the node
    for(auto it = mModifiers.begin(); it != mModifiers.end(); ++it)
    {
        (*it)->Tick(temps);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAbstrait::isWorldLocked()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAbstrait::isWorldLocked() const
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        return world->IsLocked();
    }
#endif
    return false;
}





////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////
