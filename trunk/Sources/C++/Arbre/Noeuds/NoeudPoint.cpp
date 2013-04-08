///////////////////////////////////////////////////////////////////////////////
/// @file NoeudPoint.cpp
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-02-7
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#if BOX2D_INTEGRATED
#include <Box2D/Box2D.h>
#endif

#include "NoeudPoint.h"
#include "Utilitaire.h"
#include "ZoneEdition.h"
#include "Terrain.h"
#include "NoeudTable.h"
#include "XMLUtils.h"
#include "VisiteurNoeud.h"
#include "NoeudBut.h"


const float NoeudPoint::DEFAULT_RADIUS = 10;

CreateListDelegateImplementation(TableControlPoint)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudPoint::DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudPoint::NoeudPoint( const std::string& typeNoeud, 
/// float coordX, float coordY, int typePosNoeud )
///
/// Constructeur assignant les paramètres donner aux attributs de la 
/// classe.
///
/// @param[in] const std::string & typeNoeud : le type de noeud.
/// @param[in] float coordX : la coordonne en X du noeud.
/// @param[in] float coordY : la coordonne en Y du noeud.
/// @param[in] int typePosNoeud : un int représentant le enum du 
///									type de position du noeud, i.e. HG.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudPoint::NoeudPoint( const std::string& typeNoeud, float coordX, float coordY, TypePosPoint typePosNoeud)
	: NoeudComposite(RAZER_KEY_TABLE_CONTROL_POINT,typeNoeud) , longueurCote_(2.0f), typePosNoeud_(typePosNoeud),mCanBeVisited(true)
{
    /// les noeuds points ne peuvent etre supprimer
    mFlags.SetFlag(false,NODEFLAGS_CAN_BE_DELETED);

    /// affichage du noeud avec le modele du control point
    //setDefaultNodeKey(RAZER_KEY_CONTROL_POINT);


    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);
    // Il ne faut aps utiliser le modificateur de position relative, car il ne faut pas affecter le modele 3D a la construction des points
    NoeudAbstrait::setPosition(Vecteur3(coordX,coordY, 0));
	modifierPositionInitiale(mPosition);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::~NoeudPoint()
///
/// Destructeur de la classe
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudPoint::~NoeudPoint()
{
    move3DModel(positionInitiale_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::renderReal() const
///
/// Fait le reel rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::renderReal() const
{
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);

    // Assignation du mode d'affichage des polygones
    glPolygonMode( GL_FRONT_AND_BACK, mModePolygones );
    glTranslatef(0,0,25.f);
    // Affichage concret
    NoeudAbstrait::renderReal();

    // Restauration
    glPopAttrib();
    glPopMatrix();

    DrawChild();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::renderOpenGLES()
///
/// Fonction appeler dans afficher concret pour faire le
/// rendu OpenGL, uniquement utilisé sous APPLE.
/// utiliser les liste d'affichage pour windows
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::renderOpenGLES() const
{
    glColor4f(1.0f,0.0f,1.0f,1.0f);
    Super::renderOpenGLES();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::tick( const float& temps)
///
/// Permet l'animation du point.
///
/// @param[in] float temps :  la duree de l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::tick( const float& temps )
{
	//mAngle = (float)((int)(mAngle+temps*500.0f)%360);
	//updateMatrice();

// #if WIN32
// 	glPushMatrix();
// 	glLoadMatrixd(matrice_); // Chargement de la matrice du noeud
// 	glRotated(90, 1.0, 0.0, 0.0);
// 	glGetDoublev(GL_MODELVIEW_MATRIX, matrice_); // Savegarde de la matrice courante dans le noeud
// 	glPopMatrix(); // Recuperation de la matrice d'origine
// #endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::acceptVisitor(VisiteurNoeud& v)
///
/// Permet d'accueillir un visiteur pour faire un opération qui 
///	vient de l'extérieur.
///
/// @param[in] VisiteurNoeud & v : le visiteur contenant l'operation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudPoint(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::obtenirTypePosNoeud() const
///
/// Retourne le type de position du noeud selon le enum, i.e. HG = hautGauche_
///
///
/// @return int : le type de position du noeud.
///
////////////////////////////////////////////////////////////////////////
TypePosPoint NoeudPoint::obtenirTypePosNoeud() const
{
	return typePosNoeud_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::getPosition() const  
///
/// Retourne la position absolue du noeud.
///
///
/// @return Vecteur3 : position du noeud.
///
////////////////////////////////////////////////////////////////////////
const Vecteur3& NoeudPoint::getPosition() const 
{
	return mPosition;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::obtenirPointSym() const
///
/// Retourne un pointeur sur le noeud symétrique.
///
///
/// @return NoeudPoint* : pointeur sur un noeud point symétrique.
///
////////////////////////////////////////////////////////////////////////
NoeudPoint* NoeudPoint::obtenirPointSym() const
{
	return pointSym_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::modifierPointSym( NoeudPoint* pointSym )
///
/// Modifie le pointeur sur le noeud symétrique.
///
/// @param[in] NoeudPoint * pointSym : le point symetrique au point courant.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::modifierPointSym( NoeudPoint* pointSym )
{
	pointSym_=pointSym;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::validerDeplacement( Vecteur3 pos, Vecteur2 deplace, int axe )
///
/// Valide le déplacement selon la limite in ou out et selon l'axe ( x ou y )
///
/// @param[in] Vecteur3 pos : la position a valider.
/// @param[in] Vecteur3 deplace : le vecteur de deplacement desire.
/// @param[in] int axe : l'axe a verifier.
///
/// @return bool : si le deplacement est valide ou non.
///
////////////////////////////////////////////////////////////////////////
bool NoeudPoint::validerDeplacement( const Vecteur3& pos, const Vecteur2& deplace, int axe )
{
    if(!IsInGame())
    {
        Terrain* t = getField();
        if( t )
        {
            ZoneEdition* zone = t->getZoneEdition();
            if(zone)
            {
                Vecteur3 deplace2(deplace[VX],deplace[VY],0);
                Vecteur3 cible=pos+deplace2;
                float valeurLimiteInt = (axe == VX) ? zone->obtenirLimiteIntX() : zone->obtenirLimiteIntY();
                if(utilitaire::ABS(cible[axe]) < valeurLimiteInt)
                    return false;

                float valeurLimiteExt = (axe == VX) ? zone->obtenirLimiteExtX() : zone->obtenirLimiteExtY();
                if(utilitaire::ABS(cible[axe]) > valeurLimiteExt)
                    return false;

                if(pos[axe]*cible[axe]<0)
                    return false;
            }
        }
    }
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudPoint::calculerDeplacementMax( Vecteur3 posAbsActuel, Vecteur3 deplacement )
///
/// Calcule et retourne le deplacement maximal d'un point de contrôle dans la direction du vecteur de déplacement
/// Le point retourner est donc un 
///
/// @param[in] Vecteur3 posAbsActuel
/// @param[in] Vecteur3 deplacement
///
/// @return Vecteur3
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudPoint::calculerDeplacementMax( Vecteur3 posAbsActuel, Vecteur3 deplacement )
{
	return Vecteur3(0,0,0);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudPoint::createXmlNode()
///
/// Creation du noeud XML d'un point
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudPoint::createXmlNode()
{
    XmlElement* elementNoeud = XMLUtils::createNode(mType.c_str());

    XmlWriteNodePosition(elementNoeud);
    XMLUtils::writeAttribute<int>(elementNoeud,"typePosNoeud",typePosNoeud_);
    XMLUtils::writeAttribute(elementNoeud,"selection",IsSelected());

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudPoint::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudPoint à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudPoint::initFromXml( const XmlElement* element )
{
    // faire l'initialisaiton des attribut concernant le point en premier pour que la suite puisse les utiliser
    Vecteur3 pos;
    if( !XmlReadNodePosition(pos,element) )
        throw ExceptionJeu("%s: Error reading node's position", mType.c_str());
    setPosition(pos);
    int intElem;
    if(!XMLUtils::readAttribute(element,"typePosNoeud",intElem))
        throw ExceptionJeu("Error reading attribute typePosNoeud");
    typePosNoeud_ = TypePosPoint(intElem);


    /// Those node need a goal node under them
    if(typePosNoeud_ == POSITION_MILIEU_DROITE || typePosNoeud_ == POSITION_MILIEU_GAUCHE)
    {
        auto child = XMLUtils::FirstChildElement(element);
        auto name = XMLUtils::GetNodeTag(child);
        if(name != RazerGameUtilities::NOM_BUT)
        {
            throw ExceptionJeu("XML node not representing a goal after a table control point");
        }
        NoeudBut* but = dynamic_cast<NoeudBut*>(find(0));
        checkf(but,"but manquant sur le point, regardez constructeur table");
        if(but)
        {
            but->initFromXml(child);
        }
    }
    auto terrain = getField();
    setVisible(!terrain || !terrain->IsGameField());

    bool selected;
    if(XMLUtils::readAttribute(element,"selection",selected))
    {
        setSelection(selected);
    }

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const GroupeTripleAdresseFloat* NoeudPoint::obtenirListePointsAChanger() const
///
/// Accesseur de la liste de point du modele de la table associe a ce point d'ancrage
///
///
/// @return GroupeTripleAdresseFloat
///
////////////////////////////////////////////////////////////////////////
const GroupeTripleAdresseFloat* NoeudPoint::obtenirListePointsAChanger() const
{
	ListeIndexPoints::iterator iter = NoeudTable::obtenirListeIndexPointsModeleTable().find(typePosNoeud_);
	if(iter != NoeudTable::obtenirListeIndexPointsModeleTable().end())
		return &iter->second;
	return 0;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPoint::setPosition( const Vecteur3& positionRelative )
///
/// Assigne la position relative du noeud et deplace le modele 3D de la table
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::setPosition( const Vecteur3& positionRelative )
{
    move3DModel(positionRelative);
    // assigner la position du point en premier pour que la table puisse l'utiliser à sa mise a jour
    NoeudAbstrait::setPosition(positionRelative);
    PositionSubject::signalObservers();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPoint::move3DModel( const Vecteur3& targetPosition )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & targetPosition
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::move3DModel( const Vecteur3& targetPosition )
{
#if WIN32  
    // onlny edit model if the table is present
    auto field = getField();
    if( field && field->getTable() && !field->getIsSimulation() && field->getResizeTableModel())
    {
        const GroupeTripleAdresseFloat* liste = obtenirListePointsAChanger();
        Vecteur3 deplacement(targetPosition-mPosition);
        if(liste)
        {
            for(unsigned int i=0; i<liste->size(); i++)
            {
                *(liste->get(i)[VX]) += (float)deplacement[VX];
                *(liste->get(i)[VY]) += (float)deplacement[VY];
            }
        }
        else
        {
            NoeudTable::queueTableModelMove(typePosNoeud_,deplacement);
        }
    }
#endif
}


void NoeudPoint::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        if(!IsInGame())
        {
            b2BodyDef myBodyDef;
            myBodyDef.type = b2_dynamicBody;; //this will be a static body
            const Vecteur3& pos = getPosition();
            b2Vec2 posB2;
            utilitaire::VEC3_TO_B2VEC(pos,posB2);
            myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
            myBodyDef.angle = 0; //set the starting angle

            mPhysicBody = world->CreateBody(&myBodyDef);
            b2CircleShape circleShape;
            circleShape.m_p.Set(0, 0); //position, relative to body position
            circleShape.m_radius = (float32)getRadius()*utilitaire::ratioWorldToBox2D; //radius

            b2FixtureDef myFixtureDef;
            myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
            myFixtureDef.density = 1;
            RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_CONTROL_POINT,IsInGame());

            mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
            mPhysicBody->SetUserData(this);
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPoint::flagSelectedAssociatedPoints()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::flagSelectedAssociatedPoints() const
{
    NoeudPoint* point = obtenirPointSym();
    if(point)
    {
        point->flagVisitationIfSelected();
    }
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

