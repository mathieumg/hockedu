///////////////////////////////////////////////////////////////////////////////
/// @file NodeTableControlPoint.cpp
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

#include "NodeTableControlPoint.h"
#include "Utilitaire.h"
#include "ZoneEdition.h"
#include "Terrain.h"
#include "NoeudTable.h"
#include "XMLUtils.h"
#include "VisiteurNoeud.h"
#include "NoeudBut.h"


const float NodeTableControlPoint::DEFAULT_RADIUS = 10;

CreateListDelegateImplementation(TableControlPoint)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NodeTableControlPoint::DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeTableControlPoint::NodeTableControlPoint( const std::string& typeNoeud, 
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
NodeTableControlPoint::NodeTableControlPoint( float coordX, float coordY, TypePosPoint typePosNoeud)
	: NoeudComposite(RAZER_KEY_TABLE_CONTROL_POINT) , longueurCote_(2.0f), typePosNoeud_(typePosNoeud),mCanBeVisited(true)
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
/// @fn NodeTableControlPoint::~NodeTableControlPoint()
///
/// Destructeur de la classe
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeTableControlPoint::~NodeTableControlPoint()
{
    move3DModel(positionInitiale_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::renderReal() const
///
/// Fait le reel rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::renderReal() const
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
void NodeTableControlPoint::renderOpenGLES() const
{
    glColor4f(1.0f,0.0f,1.0f,1.0f);
    Super::renderOpenGLES();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::tick( const float& temps)
///
/// Permet l'animation du point.
///
/// @param[in] float temps :  la duree de l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::tick( const float& temps )
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
/// @fn NodeTableControlPoint::acceptVisitor(VisiteurNoeud& v)
///
/// Permet d'accueillir un visiteur pour faire un opération qui 
///	vient de l'extérieur.
///
/// @param[in] VisiteurNoeud & v : le visiteur contenant l'operation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudPoint(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::obtenirTypePosNoeud() const
///
/// Retourne le type de position du noeud selon le enum, i.e. HG = hautGauche_
///
///
/// @return int : le type de position du noeud.
///
////////////////////////////////////////////////////////////////////////
TypePosPoint NodeTableControlPoint::obtenirTypePosNoeud() const
{
	return typePosNoeud_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::getPosition() const  
///
/// Retourne la position absolue du noeud.
///
///
/// @return Vecteur3 : position du noeud.
///
////////////////////////////////////////////////////////////////////////
const Vecteur3& NodeTableControlPoint::getPosition() const 
{
	return mPosition;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::obtenirPointSym() const
///
/// Retourne un pointeur sur le noeud symétrique.
///
///
/// @return NodeTableControlPoint* : pointeur sur un noeud point symétrique.
///
////////////////////////////////////////////////////////////////////////
NodeTableControlPoint* NodeTableControlPoint::obtenirPointSym() const
{
	return pointSym_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::modifierPointSym( NodeTableControlPoint* pointSym )
///
/// Modifie le pointeur sur le noeud symétrique.
///
/// @param[in] NodeTableControlPoint * pointSym : le point symetrique au point courant.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::modifierPointSym( NodeTableControlPoint* pointSym )
{
	pointSym_=pointSym;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeTableControlPoint::validerDeplacement( Vecteur3 pos, Vecteur2 deplace, int axe )
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
bool NodeTableControlPoint::validerDeplacement( const Vecteur3& pos, const Vecteur2& deplace, int axe )
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
                if(utilitaire::ABSOLUTETABARNAK(cible[axe]) < valeurLimiteInt)
                    return false;

                float valeurLimiteExt = (axe == VX) ? zone->obtenirLimiteExtX() : zone->obtenirLimiteExtY();
                if(utilitaire::ABSOLUTETABARNAK(cible[axe]) > valeurLimiteExt)
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
/// @fn Vecteur3 NodeTableControlPoint::calculerDeplacementMax( Vecteur3 posAbsActuel, Vecteur3 deplacement )
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
Vecteur3 NodeTableControlPoint::calculerDeplacementMax( Vecteur3 posAbsActuel, Vecteur3 deplacement )
{
	return Vecteur3(0,0,0);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NodeTableControlPoint::createXmlNode()
///
/// Creation du noeud XML d'un point
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NodeTableControlPoint::createXmlNode()
{
    XmlElement* elementNoeud = XMLUtils::createNode(mNodeName.c_str());
    XMLUtils::writeAttribute( elementNoeud, ETIQUETTE_KEY, (int)mNodeKey );
    XmlWriteNodePosition(elementNoeud);
    XMLUtils::writeAttribute<int>(elementNoeud,"typePosNoeud",typePosNoeud_);
    XMLUtils::writeAttribute(elementNoeud,"selection",IsSelected());

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeTableControlPoint::initFromXml( const XmlElement* element )
///
/// Initialisation du NodeTableControlPoint à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeTableControlPoint::initFromXml( const XmlElement* element )
{
    // faire l'initialisaiton des attribut concernant le point en premier pour que la suite puisse les utiliser
    Vecteur3 pos;
    if( !XmlReadNodePosition(pos,element) )
        throw ExceptionJeu("%s: Error reading node's position", mNodeName.c_str());
    setPosition(pos);
    int intElem;
    if(!XMLUtils::readAttribute(element,"typePosNoeud",intElem))
        throw ExceptionJeu("Error reading attribute typePosNoeud");
    typePosNoeud_ = TypePosPoint(intElem);


    /// Those node need a goal node under them
    if(typePosNoeud_ == POSITION_MILIEU_DROITE || typePosNoeud_ == POSITION_MILIEU_GAUCHE)
    {
        auto child = XMLUtils::FirstChildElement(element);
        int k;
        bool read = XMLUtils::readAttribute( child, ETIQUETTE_KEY, k );
        if(!read || k != RAZER_KEY_GOAL)
        {
            throw ExceptionJeu("XML node not representing a goal after a table control point");
        }
        NoeudBut* but = dynamic_cast<NoeudBut*>(getChild(0));
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
/// @fn const GroupeTripleAdresseFloat* NodeTableControlPoint::obtenirListePointsAChanger() const
///
/// Accesseur de la liste de point du modele de la table associe a ce point d'ancrage
///
///
/// @return GroupeTripleAdresseFloat
///
////////////////////////////////////////////////////////////////////////
const GroupeTripleAdresseFloat* NodeTableControlPoint::obtenirListePointsAChanger() const
{
	ListeIndexPoints::iterator iter = NoeudTable::obtenirListeIndexPointsModeleTable().find(typePosNoeud_);
	if(iter != NoeudTable::obtenirListeIndexPointsModeleTable().end())
		return &iter->second;
	return 0;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeTableControlPoint::setPosition( const Vecteur3& positionRelative )
///
/// Assigne la position relative du noeud et deplace le modele 3D de la table
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::setPosition( const Vecteur3& positionRelative )
{
    move3DModel(positionRelative);
    // assigner la position du point en premier pour que la table puisse l'utiliser à sa mise a jour
    NoeudAbstrait::setPosition(positionRelative);
    PositionSubject::signalObservers();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeTableControlPoint::move3DModel( const Vecteur3& targetPosition )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & targetPosition
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::move3DModel( const Vecteur3& targetPosition )
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


void NodeTableControlPoint::updatePhysicBody()
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
/// @fn void NodeTableControlPoint::flagSelectedAssociatedPoints()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeTableControlPoint::flagSelectedAssociatedPoints() const
{
    NodeTableControlPoint* point = obtenirPointSym();
    if(point)
    {
        point->flagVisitationIfSelected();
    }
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

