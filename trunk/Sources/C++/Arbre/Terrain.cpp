///////////////////////////////////////////////////////////////////////////////
/// @file Terrain.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "RazerGameUtilities.h"

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#if BOX2D_PLAY
#include "Partie.h"
#include "SoundFMOD.h"
#include "Runnable.h"
#endif
#if BOX2D_DEBUG
#include "DebugRenderBox2D.h"
#endif

#ifndef __APPLE__
#include "../Reseau/Paquets/PaquetMaillet.h"
#include "../Reseau/GestionnaireReseau.h"
#include "GestionnaireHUD.h"
#include "HUDBonus.h"
#include "FacadeModele.h"

#define TransmitEvent(e) FacadeModele::transmitEvent(e)
#else
#define TransmitEvent(e)
#endif

#include "Terrain.h"
#include "RazerGameTree.h"
#include "ArbreNoeudLibre.h"
#include "ZoneEdition.h"
#include "NoeudTable.h"
#include "NoeudGroupe.h"
#include "ZoneEdition.h"
#include "AideCollision.h"
#include "NoeudBut.h"
#include "utilitaire.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "NoeudBut.h"
#include "NodeModelRender.h"
#include "NodeBonus.h"
#include "XMLUtils.h"
#include "NoeudAccelerateur.h"
#include "NoeudPortail.h"
#include "VisiteurDupliquer.h"
#include "VisiteurCollision.h"
#include "VisiteurDeplacement.h"
#include "VisiteurEcrireXML.h"
#include "ExceptionJeu.h"
#include "NodeWallEdition.h"
#include "NodeControlPoint.h"
#include "VisiteurFunction.h"
#include "VisitorGatherProperties.h"
#include "FieldModificationStrategyMove.h"
#include "FieldModificationStrategyRotate.h"
#include "FieldModificationStrategyScale.h"
#include "VisitorSetProperties.h"
#include "FieldModificationStrategyAddNode.h"
#include "FieldModificationStrategyAddWall.h"
#include "Visiteur/VisiteurSuppression.h"
#include "../Reseau/Paquets/PaquetGameEvent.h"
#include "../Reseau/RelayeurMessage.h"

const unsigned int MAX_PUCKS = 1;
const unsigned int MAX_MALLETS = 2;



////////////////////////////////////////////////////////////////////////
///
/// @fn  Terrain::Terrain()
///
/// Constructeur par paramètre.
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Terrain::Terrain(Partie* pGame): 
    mLogicTree(NULL), mNewNodeTree(NULL), mTable(NULL),mFieldName(""),mRenderTree(0),mGame(pGame),mZamboni(NULL),
    mLeftMallet(NULL),mRightMallet(NULL),mPuck(NULL), mIsInit(false), mModifStrategy(NULL),mDoingUndoRedo(false),mCurrentState(NULL), mBesoinMiseAuJeu(false)
{

    mRedoBuffer.reserve(UNDO_BUFFERSIZE);
#if BOX2D_INTEGRATED
    b2Vec2 gravity(0,0);
    mWorld = new b2World(gravity);

    mWorld->SetWarmStarting(false);
    mWorld->SetContinuousPhysics(true);
    mWorld->SetSubStepping(false);

#if BOX2D_PLAY
    if(IsGameField())
    {
        mWorld->SetContactListener(this);
    }
#endif
#if BOX2D_DEBUG
    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_shapeBit);
    mWorld->SetDebugDraw(DebugRenderBox2D::mInstance);
#endif
#endif

    /// Cree la zone edition apres le world, car celle-ci va en avoir besoin
    mEditionZone = NULL;
    if(!mGame)
    {
        mEditionZone = new ZoneEdition(this);

        // cant do undo/redo action by default
        TransmitEvent(CANNOT_UNDO);
        TransmitEvent(CANNOT_REDO);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  Terrain::~Terrain(  )
///
/// Destructeur, libération de la mémoire
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
Terrain::~Terrain()
{
#ifndef __APPLE__
    RunnableBreaker::signalObservers();
#endif
    libererMemoire();

    if(mEditionZone)
    {
        delete mEditionZone;
    }

#if BOX2D_INTEGRATED
    if(mWorld)
    {
        delete mWorld;
        mWorld = NULL;
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::libererMemoire()
///
/// Libère la mémoire du terrain et le retourne à son état de base
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::libererMemoire()
{
    mIsInit = false;
    // S'assurer de remettre tous les pointeurs a NULL car la comparaison est utiliser 
    // partout dans le terrain pour savoir si le pointeur est valide
    if(mLogicTree)
    {
        // Libération de l'arbre de rendu complet
        mLogicTree->empty();
        delete mLogicTree;
        mLogicTree = NULL;
    }
    if(mNewNodeTree)
    {
        // Ses enfants ne seront pas libérés, mais le terrain n'en n'est pas responsable
        mNewNodeTree->empty();
        delete mNewNodeTree;
        mNewNodeTree = NULL;
    }
    // Libère l'arbre de rendu servant à l'affichage seulement
    if(mRenderTree)
    {
        mRenderTree->empty();
        delete mRenderTree;
        mRenderTree = NULL;
    }
    // On ne libère pas la table, car elle est un enfant de l'arbre de rendu
    mTable = NULL;
    
    mFieldName = "";

    /// if we are currently applying undo/redo, we must not clear that memory
    if(!mDoingUndoRedo)
    {
        for(auto it=mRedoBuffer.begin(); it!= mRedoBuffer.end(); ++it)
        {
            delete (*it);
        }
        mRedoBuffer.clear();
        for(auto it=mUndoBuffer.begin(); it!= mUndoBuffer.end(); ++it)
        {
            delete (*it);
        }
        mUndoBuffer.clear();
        if(mCurrentState)
        {
            delete mCurrentState;
        }
        mCurrentState = NULL;

        // cant do undo/redo action by default
        TransmitEvent(CANNOT_UNDO);
        TransmitEvent(CANNOT_REDO);
    }

}




////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::renderField()
///
/// Permet d'effectuer le rendu des arbres du terrain
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::renderField()
{
    if(mLogicTree)
        mLogicTree->render();

#if WIN32
    GLint renderMode;
    glGetIntegerv(GL_RENDER_MODE,&renderMode);
    if(renderMode == GL_SELECT)
    {
        // dont draw anything else when selecting
        return;
    }
#endif

    if(mNewNodeTree)
        mNewNodeTree->render();
    if(getZoneEdition())
        getZoneEdition()->afficher();
    if(mRenderTree)
        mRenderTree->render();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiser( std::string nom )
///
/// Permet d'initialiser le terrain avec ces éléments de bases pour le terrain
/// Permet de reintialiser en meme temps
///
/// @param[in] std::string nom : nom du terrain est en fait le chemin pour la sauvegarde en XML
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::initialiser( std::string nom )
{
    // On s'assure que les éléments du terrain sont libéré avant de les écraser
    libererMemoire();
    // Doit rester la premiere chose que l'on fait dans cette méthode sinon on pourrait invalider des pointeurs
    ///////////////////////////////////////////////////////////////////////////

    // Assignation du nom du terrain
    mFieldName = nom;
    // Initialisation de la Zone d'edition
    if(getZoneEdition())
    {
        getZoneEdition()->reinitialiser();
    }

    initialiserArbreRendu();
    fullRebuild();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiserArbreRendu()
///
/// Methode pour initFromXml l'arbre de rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::initialiserArbreRendu()
{
    // Initialisation des arbres de rendus
    if(mLogicTree == NULL)
    {
        mLogicTree = new RazerGameTree(this,MAX_MALLETS,MAX_PUCKS);
    }
    else
    {
        mLogicTree->empty();
    }

    if(!mRenderTree)
    {
        mRenderTree = new ArbreRendu(this);
        NoeudAbstrait* piece = new NodeModelRender(RazerGameUtilities::NOM_HOUSE);
        mRenderTree->add(piece);
        if(IsGameField())
        {
            mZamboni = new NodeModelRender(RazerGameUtilities::NAME_ZAMBONI);
            mRenderTree->add(mZamboni);
        }
    }
    if(!mGame)
    {
        if(!mNewNodeTree)
        {
            mNewNodeTree = new ArbreNoeudLibre();
        }
        else
        {
            mNewNodeTree->empty();
        }
    }

    // Ajout d'une table de base au terrain
    mTable = new NoeudTable(RazerGameUtilities::NOM_TABLE);
    mLogicTree->add(mTable);

    // Permet de rediriger les bandes extérieur de la table vers le groupe  gMuret
    //mTable->reassignerParentBandeExt();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::initialiserXml( XmlElement* element )
///
/// Permet d'initFromXml le terrain avec ces éléments a partir d'un noeud XML
///
/// @param[in] XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::initialiserXml( const XmlElement* element, bool fromDocument /*= true */ )
{
    libererMemoire();
    
    // Initialisation des arbres de rendus
    if(!mRenderTree)
    {
        mRenderTree = new ArbreRendu(this);
        NoeudAbstrait* piece = new NodeModelRender(RazerGameUtilities::NOM_HOUSE);
        mRenderTree->add(piece);
        if(IsGameField())
        {
            mZamboni = new NodeModelRender(RazerGameUtilities::NAME_ZAMBONI);
            mRenderTree->add(mZamboni);
        }
    }
    if(!mGame)
    {
        if(!mNewNodeTree)
        {
            mNewNodeTree = new ArbreNoeudLibre();
        }
        else
        {
            mNewNodeTree->empty();
        }
    }
    mLogicTree = new RazerGameTree(this,MAX_MALLETS,MAX_PUCKS);

    const XmlElement* racine = element;
    /// si ce noeud xml vient d'un document, il faut retrouver la node Terrain
    if(fromDocument)
    {
        racine = XMLUtils::FirstChildElement(element,"Terrain");
        if(!racine)
            return false;
    }

    if(!XMLUtils::readAttribute(racine,"nom",mFieldName))
        return false;

    try
    {
        mLogicTree->initFromXml(racine);
        initNecessaryPointersForGame();
    }
    catch(ExceptionJeu& e)
    {
        // Erreur dans le fichier XML, on remet un terrain par defaut
        creerTerrainParDefaut(mFieldName);
        utilitaire::afficherErreur(e.what());
        return true;
    }
    catch(...)
    {
        // Erreur dans le fichier XML, on remet un terrain par defaut
        creerTerrainParDefaut(mFieldName);
        return true;
    }

    if(getZoneEdition() && !getZoneEdition()->initialisationXML(racine))
        return false;

    fullRebuild();

    /// premiere modif, save state first
    if(!IsGameField() && !mCurrentState)
    {
        mCurrentState = creerNoeudXML();
    }

    if( mSelectedNodes.size() == 0 )
    {
        // no more item selected
        TransmitEvent(THERE_ARE_NO_NODE_SELECTED);
    }
    else
    {
        // selection present
        TransmitEvent(THERE_ARE_NODES_SELECTED);
    }


    mIsInit = true;

    return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::creerTerrainParDefaut()
///
/// Création d'un terrain par défaut.
/// Celui-ci a les dimemsions de base, 2 maillets et une rondelle
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::creerTerrainParDefaut(const std::string& nom)
{
    initialiser(nom);
    NoeudAbstrait* maillet1 = getLogicTree()->creerNoeud(RazerGameUtilities::NOM_MAILLET);
    NoeudAbstrait* maillet2 = getLogicTree()->creerNoeud(RazerGameUtilities::NOM_MAILLET);
    NoeudAbstrait* rondelle = getLogicTree()->creerNoeud(RazerGameUtilities::NOM_RONDELLE);

    maillet1->setPosition(mTable->obtenirPoint(POSITION_MILIEU_GAUCHE)->getPosition()/2.0);
    maillet2->setPosition(mTable->obtenirPoint(POSITION_MILIEU_DROITE)->getPosition()/2.0);
    rondelle->setPosition(Vecteur3(0.0,0.0,0.0));
    
    mTable->add(maillet1);
    mTable->add(maillet2);
    mTable->add(rondelle);

    rondelle->setSelection(true);
    fullRebuild();
    try
    {
        initNecessaryPointersForGame();
    }
    catch(ExceptionJeu& e)
    {
        checkf(0,"%s", e.what());
    }

    /// premiere modif, save state first
    if(!IsGameField() && !mCurrentState)
    {
        mCurrentState = creerNoeudXML();
    }

    if( mSelectedNodes.size() == 0 )
    {
        // no more item selected
        TransmitEvent(THERE_ARE_NO_NODE_SELECTED);
    }
    else
    {
        // selection present
        TransmitEvent(THERE_ARE_NODES_SELECTED);
    }

    mIsInit = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void createRandomField( const std::string& )
///
/// Creation d'un terrain 
///
/// @param[in] const std::string &
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::createRandomField(const std::string& nom)
{
    creerTerrainParDefaut(nom);

    VisiteurDeplacement d1(Vecteur2((float)(rand()%50),(float)(rand()%50)));
    VisiteurDeplacement d2(Vecteur2((float)(rand()%50),(float)(rand()%50)));
    VisiteurDeplacement d3(Vecteur2((float)(rand()%50),(float)(-rand()%50)));

    // shuffle it a bit
    mTable->obtenirPoint(POSITION_MILIEU_DROITE)->acceptVisitor(d1);
    mTable->obtenirPoint(POSITION_HAUT_DROITE)->acceptVisitor(d2);
    mTable->obtenirPoint(POSITION_BAS_DROITE)->acceptVisitor(d3);

#define AddAccel(x,y,acc,e)                                                             \
    if(rand()&1)                                                                        \
    {                                                                                   \
    NoeudAccelerateur* a = new NoeudAccelerateur(RazerGameUtilities::NOM_ACCELERATEUR); \
    a->setPosition(Vecteur3((float)x,(float)y));                                        \
    a->modifierBonusAccel((float)acc);                                                  \
    a->setScale(Vecteur3((float)e,(float)e,1));                                         \
    mTable->add(a);                                                                     \
    }                                                                                 
    AddAccel(-40,0,1.7,1);
    AddAccel(45,0,0.8,1);
    AddAccel(50,50,1.5,1.2);
    AddAccel(-50,50,1.6,1);
    AddAccel(-50,-50,1.5,1);
    AddAccel(50,-50,1.5,1);
    AddAccel(115,0,1.3,1);
    AddAccel(-115,0,1.5,0.6);

#define AddPortal(x,y,e)                                                              \
    if(rand()&1)                                                                      \
    {                                                                                 \
    NoeudPortail* a = new NoeudPortail(RazerGameUtilities::NOM_PORTAIL);              \
    a->setPosition(Vecteur3((float)x,(float)y));                                      \
    a->setScale(Vecteur3((float)e,(float)e,1));                                       \
    mTable->add(a);                                                                   \
    }  
    AddPortal(-90,30,1);
    AddPortal(-95,-30,1);
    AddPortal(0,-50,0.55);
    AddPortal(80,-40,1.55);
    AddPortal(80,30,1);
    AddPortal(0,55,0.65);

#define AddWall(x1,y1,x2,y2,e,rebond)                                                           \
    if(rand()&1)                                                                                \
    {                                                                                           \
        if(IsGameField())                                                                       \
        {                                                                                       \
            NoeudMuret* muret = new NoeudMuret(RazerGameUtilities::NOM_MURET);                  \
            muret->setScale(Vecteur3(1,(float)e,1));                                            \
            muret->assignerPositionCoin(1,Vecteur3((float)x1,(float)y1));                       \
            muret->assignerPositionCoin(2,Vecteur3((float)x2,(float)y2));                       \
            muret->setReboundRatio((float)rebond);                                              \
            mTable->add(muret);                                                                 \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            NodeWallEdition* wall = new NodeWallEdition(RazerGameUtilities::NOM_MURET);         \
            NodeControlPoint* p1 = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);\
            NodeControlPoint* p2 = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);\
            wall->add(p1);                                                                      \
            wall->add(p2);                                                                      \
            p1->setPosition(Vecteur3((float)x1,(float)y1));                                     \
            p2->setPosition(Vecteur3((float)x2,(float)y2));                                     \
            wall->setReboundRatio((float)rebond);                                               \
            wall->modifierEchelle(e);                                                           \
            mTable->add(wall);                                                                  \
        }                                                                                       \
    }                                                                                           

    AddWall(65,65,130,35,1,1);
    AddWall(130,-35,75,-66,1,0.8);
    AddWall(-80,-65,-130,-40,1,0.75);
    AddWall(-125,35,-75,60,2.5,1);
    AddWall(-35,30,30,30,0.5,1.2);
    AddWall(40,-30,-25,-30,2,0.4);

#define AddBonus(x,y,e)                                             \
    if(rand()&1)                                                    \
    {                                                               \
    NodeBonus* b = new NodeBonus(RazerGameUtilities::NAME_BONUS);   \
    b->setPosition(Vecteur3((float)x,(float)y));                    \
    b->setScale(Vecteur3((float)e,(float)e,1));                     \
    mTable->add(b);                                                 \
    }  

    AddBonus(-22.1519,57.4857 ,1);
    AddBonus(26.619  ,60.1458 ,1.2);
    AddBonus(23.1852 ,-59.9112,1);
    AddBonus(-22.3497,-59.9112,1);
    AddBonus(-57.5803,-23.4159,0.7);
    AddBonus(-62.3496,29.808  ,0.6);
    AddBonus(54.1487 ,26.8466 ,1.1);
    AddBonus(53.2077 ,-19.4675,1);

    fullRebuild();

    /// premiere modif, save state first
    if(!IsGameField() && !mCurrentState)
    {
        mCurrentState = creerNoeudXML();
    }

    if( mSelectedNodes.size() == 0 )
    {
        // no more item selected
        TransmitEvent(THERE_ARE_NO_NODE_SELECTED);
    }
    else
    {
        // selection present
        TransmitEvent(THERE_ARE_NODES_SELECTED);
    }

    mIsInit = true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::reinitialiser()
///
/// Remet le terrain a son etat de base
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::reinitialiser()
{
    initialiserArbreRendu();
    getZoneEdition()->reinitialiser();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* Terrain::creerNoeudXML()
///
/// Creation du noeud XML du Terrain
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* Terrain::creerNoeudXML()
{
    // Créer le noeud 
    XmlElement* racine = XMLUtils::createNode("Terrain");
    XMLUtils::writeAttribute(racine,"nom",getNom());

    VisiteurEcrireXML v;
    acceptVisitor(v);

    XmlElement* elem = v.obtenirRacine();
    XMLUtils::LinkEndChild(racine,elem);

    checkf(getZoneEdition(), "Tentative de sauvegarder la zone édition qui n'existe pas. S'assurer qu'on est pas en train d'essaye de save un terrain pour le mode jeu");
    if(getZoneEdition())
    {
        XMLUtils::LinkEndChild(racine,getZoneEdition()->creerNoeudXML());
    }

    return racine;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::animerTerrain( const float& temps )
///
/// Permet d'tick les noeuds des arbres du terrain, la physique reste géré par
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::animerTerrain( const float& temps )
{
    if(mLogicTree)
        mLogicTree->tick(temps);
    if(mNewNodeTree)
        mNewNodeTree->tick(temps);
    if(mRenderTree)
        mRenderTree->tick(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::ajouterNoeudTemp( const NoeudAbstrait* noeud )
///
/// Ajout d'un noeud dans l'arbre de noeud Temporaire
///
/// @param[in] const NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::ajouterNoeudTemp( NoeudAbstrait* noeud )
{
    mNewNodeTree->add(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::transfererNoeud( NoeudAbstrait* noeud )
///
/// Transfert d'un noeud de l'arbre de noeud temporaire à l'arbre de rendu
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::transfererNoeud( NoeudAbstrait* noeud )
{
    mNewNodeTree->unlinkChild(noeud);
    mTable->add(noeud);
    noeud->forceFullUpdate();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::retirerNoeudTemp( NoeudAbstrait* noeud )
///
/// Permet de detacher un noeud temporaire de l'arbre de noeud temporaire
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::retirerNoeudTemp( NoeudAbstrait* noeud )
{
    if(mNewNodeTree)
    {
        mNewNodeTree->unlinkChild(noeud);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::insideLimits( NoeudAbstrait* noeud )
///
/// Vérification pour voir si un noeud est dans les limites de la zone edition
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return bool Vrai si le noeud est dans les limites de la zone
///
////////////////////////////////////////////////////////////////////////
bool Terrain::insideLimits( NoeudAbstrait* noeud )
{
    /// Deprecated

    const Vecteur3& pos = noeud->getPosition();
    auto zone = getZoneEdition();
    checkf(zone,"call illégal à InsideLimits, doit seulement etre utiliser pour le mode édition");
    // Cas particulier pour des muret puisque ce sont des segment et non des cercles
    if(zone)
    {
        auto aabb = zone->getAABBExt();
        if( noeud->getType() == RazerGameUtilities::NOM_MURET)
        {
            Vecteur2 intersection;
            NodeWallAbstract *muret = (NodeWallAbstract *)noeud;

            return aabb.IsInside(muret->obtenirCoin1()) && aabb.IsInside(muret->obtenirCoin2());
            // 
            //         if (muret)
            //         {
            //             // Ligne du haut
            //             if(aidecollision::calculerCollisionSegmentSegment(
            //                 muret->obtenirCoin1().convertir<2>(),
            //                 muret->obtenirCoin2().convertir<2>(),
            //                 Vecteur2(-getZoneEdition()->obtenirLimiteExtX(),getZoneEdition()->obtenirLimiteExtY()),
            //                 Vecteur2(getZoneEdition()->obtenirLimiteExtX(),getZoneEdition()->obtenirLimiteExtY()),
            //                 intersection    // pas besoin du point dintersection
            //                 ).type != aidecollision::COLLISION_AUCUNE)
            //                 return false;
            //             // Ligne de droite
            //             if(aidecollision::calculerCollisionSegmentSegment(
            //                 muret->obtenirCoin1().convertir<2>(),
            //                 muret->obtenirCoin2().convertir<2>(),
            //                 Vecteur2(getZoneEdition()->obtenirLimiteExtX(),getZoneEdition()->obtenirLimiteExtY()),
            //                 Vecteur2(getZoneEdition()->obtenirLimiteExtX(),-getZoneEdition()->obtenirLimiteExtY()),
            //                 intersection    // pas besoin du point dintersection
            //                 ).type != aidecollision::COLLISION_AUCUNE)
            //                 return false;
            //             // Ligne du bas
            //             if(aidecollision::calculerCollisionSegmentSegment(
            //                 muret->obtenirCoin1().convertir<2>(),
            //                 muret->obtenirCoin2().convertir<2>(),
            //                 Vecteur2(getZoneEdition()->obtenirLimiteExtX(),-getZoneEdition()->obtenirLimiteExtY()),
            //                 Vecteur2(-getZoneEdition()->obtenirLimiteExtX(),-getZoneEdition()->obtenirLimiteExtY()),
            //                 intersection    // pas besoin du point dintersection
            //                 ).type != aidecollision::COLLISION_AUCUNE)
            //                 return false;
            //             // Ligne de Gauche
            //             if(aidecollision::calculerCollisionSegmentSegment(
            //                 muret->obtenirCoin1().convertir<2>(),
            //                 muret->obtenirCoin2().convertir<2>(),
            //                 Vecteur2(-getZoneEdition()->obtenirLimiteExtX(),-getZoneEdition()->obtenirLimiteExtY()),
            //                 Vecteur2(-getZoneEdition()->obtenirLimiteExtX(),getZoneEdition()->obtenirLimiteExtY()),
            //                 intersection    // pas besoin du point dintersection
            //                 ).type != aidecollision::COLLISION_AUCUNE)
            //                 return false;
            //        }
        }
        // Tests sur les positions avec leurs rayons beaucoup plus simple
        // sert aussi de float check pour les murets car leur rayon est nulle
        if(pos[VX]+noeud->getRadius() > getZoneEdition()->obtenirLimiteExtX() || pos[VX]-noeud->getRadius() < -getZoneEdition()->obtenirLimiteExtX())
            return false;
        if(pos[VY]+noeud->getRadius() > getZoneEdition()->obtenirLimiteExtY() || pos[VY]-noeud->getRadius() < -getZoneEdition()->obtenirLimiteExtY())
            return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::verifierValidite()
///
/// Verifie si le terrain respecte les conditions de jeu soit 2 maillet, 1 de chaque cote
/// de le table et une rondelle. Les elements a l'exterieur de la table sont eliminer
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::verifierValidite( bool afficherErreur /*= true*/, bool deleteExternNodes /*= true*/ )
{
    if(!mTable)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\naucune table presente sur le terrain");
        return false;
    }

    // on prend un des 2 buts
    if(!mTable->obtenirBut(1))
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\naucun buts presents sur le terrain");
        return false;
    }
    float hauteurBut = mTable->obtenirBut(1)->obtenirHauteurBut();

    static const std::string typeNoeudModifiable[] = {
        RazerGameUtilities::NOM_RONDELLE,
        RazerGameUtilities::NOM_MAILLET,
        RazerGameUtilities::NOM_ACCELERATEUR,
        RazerGameUtilities::NOM_PORTAIL,
        RazerGameUtilities::NOM_MURET,
        RazerGameUtilities::NAME_BONUS,
        RazerGameUtilities::NAME_POLYGONE,
    };
    const unsigned int nbTypeModifiable = ARRAY_COUNT(typeNoeudModifiable);

    bool nodeOutsideNotDeleted = false;
    // Suppression des noeuds qui ne sont pas sur la table
    for(unsigned int i=0; i<nbTypeModifiable; ++i)
    {
        NoeudGroupe* g = mTable->obtenirGroupe(typeNoeudModifiable[i]);
        checkf(g);
        if(g)
        {
            for(unsigned int j=0; j<g->childCount(); ++j)
            {
                NoeudAbstrait* n = g->find(j);
                // On verifie que le type est bon, surtout utile pour les bande exterieur qui ont un nom different et doivent etre ignorer par cette methode
                if(n->getType() == typeNoeudModifiable[i])
                {
                    if(!mTable->estSurTable(n) )
                    {
                        if(deleteExternNodes)
                        {
                            // La suppression du pNode l'enlevera du groupe
                            n->deleteThis();
                            // Repositionnement de i pour pointer au bon endroit a la prochaine iteration
                            j--; 
                        }
                        else
                        {
                            nodeOutsideNotDeleted = true;
                        }
                    }
                }
            }
        }
    }

    if(nodeOutsideNotDeleted && afficherErreur)
    {
        utilitaire::afficherErreur("Warning : Items present outside the table\nmight invalidate when trying to play.");
    }

    auto g = mTable->obtenirGroupe(RazerGameUtilities::NOM_PORTAIL);
    checkf(g);
    if(g && g->childCount() == 1)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\nil n'y a qu'un seul portail sur le terrain");
        return false;
    }


    // Verification des maillets
    auto leftMallet  = getLeftMallet();
    auto rightMallet = getRightMallet();
    auto puck        = getPuck();

    if(!puck)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\nil n'y a pas de rondelle presente sur la table");
        return false;
    }
    if(!leftMallet)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\nil n'y a pas de maillet present sur le cote gauche de la table");
        return false;
    }
    if(!rightMallet)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\nil n'y a pas de maillet present sur le cote droit de la table");
        return false;
    }

    // On multiplie le rayon par 3 pour avoir une distance supplementaire proportionnel a la taille de la rondelle
    if(hauteurBut <= puck->getRadius()*3)
    {
        if(afficherErreur)
        {
            std::ostringstream mess;
            mess << "Erreur: table invalide\nbuts trop petits pour jouer.\nDistance manquante = ";
            mess << (float)puck->getRadius()*2+20-hauteurBut;
            mess << " pixels";
            utilitaire::afficherErreur(mess.str());
        }
        return false;
    }


    Vecteur3 point1 = mTable->obtenirPoint(POSITION_HAUT_MILIEU)->getPosition();
    Vecteur3 point2 = mTable->obtenirPoint(POSITION_BAS_MILIEU)->getPosition();
    NoeudAbstrait* nodes[2] =
    {
        leftMallet,
        rightMallet
    };
#if BOX2D_INTEGRATED
    b2RayCastInput input;
    utilitaire::VEC3_TO_B2VEC(point1,input.p1);
    utilitaire::VEC3_TO_B2VEC(point2,input.p2);
    input.maxFraction = 1;

    for(unsigned int i=0; i<2; ++i)
    {
        auto body = nodes[i]->getPhysicBody();
        checkf(body);
        if(body)
        {
            for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {

                const int nbChild = f->GetShape()->GetChildCount();
                for(int childindex=0; childindex<nbChild; ++childindex)
                {
                    b2RayCastOutput output;
                    if ( f->RayCast( &output, input,childindex) )
                    {
                        // collision detected
                        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\nUn maillet touche a la ligne du milieu");
                        return false;
                    }
                }
            }
        }
    }
#else
    for(unsigned int i=0; i<2; ++i)
    {
        auto node = nodes[i];
        Vecteur2 intersection;
        if(aidecollision::calculerCollisionSegment(point1,point2,node->getPosition(),node->getRadius()).type != aidecollision::COLLISION_AUCUNE )
        {
            if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\nUn maillet touche a la ligne du milieu");
            return false;
        }
    }
#endif


    auto posleft = leftMallet->getPosition();
    auto posright = rightMallet->getPosition();
    if(posleft[VX] > 0)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\n2 Maillets sur le cote droite de la table");
        return false;
    }
    if(posright[VX] < 0)
    {
        if(afficherErreur)utilitaire::afficherErreur("Erreur: table invalide\n2 Maillets sur le cote gauche de la table");
        return false;
    }

    return !!leftMallet && !!rightMallet && !!puck && !nodeOutsideNotDeleted;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle* Terrain::getPuck()
///
/// Accesseur de la rondelle
///
///
/// @return NoeudRondelle* : pointeur sur la rondelle s'il la trouve, 0 sinon
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle* Terrain::getPuck() const
{
    if(mPuck)
    {
        checkf(IsGameField(), "Dans le mode édition on ne conserve pas de pointeur sur la puck");
        return mPuck;
    }
    checkf(!IsGameField(), "Dans le mode jeu on doit avoir un pointeur sur la puck");

    if(getTable())
    {
        NoeudComposite* g = (NoeudComposite*)getTable()->obtenirGroupe(RazerGameUtilities::NOM_RONDELLE);
        if(g)
        {
            for(unsigned int i=0; i<g->childCount(); ++i)
            {
                NoeudRondelle* r = dynamic_cast<NoeudRondelle *>(g->find(i));
                if(r)
                    return r;
            }
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::appliquerPhysique()
///
/// Applique la physique sur l'arbre de rendu contenant les noeuds de la table
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VISITEUR_FUNC_FUNC_DECLARATION(PlayTickNode)
{
    pNoeud->playTick(*(float*)pUserData);
}
void Terrain::appliquerPhysique( float temps )
{
    if(mLogicTree)
    {
        VisiteurFunction tick(PlayTickNode,&temps);
        mLogicTree->acceptVisitor(tick);
#if BOX2D_PLAY
        mWorld->Step(temps, 8, 8);
#elif MANUAL_PHYSICS_DETECTION
        mLogicTree->positionUpdate(temps);
        mLogicTree->collisionDetection(temps);
        mLogicTree->fixSpeed(temps);
        mLogicTree->fixOverlap();
#endif
    }
}

#if BOX2D_PLAY
////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::BeginContact( b2Contact* contact )
///
/// Callback before the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::BeginContact( b2Contact* contact )
{
    bool isEnabled = contact->IsEnabled();
    bool isTouching = contact->IsTouching();

    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    auto bodyA = fixtureA->GetBody();
    auto bodyB = fixtureB->GetBody();
    auto filterA = fixtureA->GetFilterData();
    auto filterB = fixtureB->GetFilterData();
    short category = filterA.categoryBits | filterB.categoryBits;
    bool isPuckPresent = !!(( category ) & CATEGORY_PUCK );

    if(isPuckPresent)
    {
        // on ne conserve que l'autre categorie
        category &= ~CATEGORY_PUCK;
        b2Body* bodies[2];
        // l'index 0 contient le body pointant sur la rondelle
        if(filterA.categoryBits & CATEGORY_PUCK)
        {
            bodies[0] = bodyA;
            bodies[1] = bodyB;
        }
        else
        {
            bodies[0] = bodyB;
            bodies[1] = bodyA;
        }


        switch(category)
        {
        case CATEGORY_BOUNDARY:
            // TODO:: à tester
            {
                NoeudBut *but = dynamic_cast<NoeudBut *>((NoeudAbstrait*)bodies[1]->GetUserData());
                if (but && mGame && !mGame->isNetworkClientGame())
                {
                    b2Body* rondelleBody = bodies[0];
                    NoeudRondelle* rondelle = (NoeudRondelle*)(rondelleBody->GetUserData());
                    Runnable* r = new Runnable([=](Runnable*){
                        if(mGame)
                        {
                            auto position = rondelle->getPosition();
                            if(position[VX] < 0)
                            {
                                mGame->incrementerPointsJoueurDroit();
                            }
                            else
                            {
                                mGame->incrementerPointsJoueurGauche();
                            }
                            SoundFMOD::obtenirInstance()->playEffect(GOAL_EFFECT);
                        }
                        mGame->miseAuJeu();
                        rondelleBody->SetLinearVelocity(b2Vec2(0,0));
                        rondelleBody->SetAngularVelocity(0);
                        rondelle->setAngle(0);
                    });
                    RunnableBreaker::attach(r);
                    RazerGameUtilities::RunOnUpdateThread(r,true);
                }
                else
                {
                    if(bodies[0]->GetLinearVelocity().LengthSquared() > 200)
                        SoundFMOD::obtenirInstance()->playEffect(COLLISION_MURET_EFFECT);
                }
            }
            break;
            //     case CATEGORY_PUCK    : // impossible case
            //         break;
        case CATEGORY_MALLET  :
            {
                SoundFMOD::obtenirInstance()->playEffect(effect(COLLISION_MAILLET_EFFECT1+(rand()%5)));
                NoeudMaillet* maillet = (NoeudMaillet*)(bodies[1]->GetUserData());
                NoeudRondelle* rondelle = (NoeudRondelle*)(bodies[0]->GetUserData());
                // Si partie en reseau, on doit envoyer un paquet game event pour dire de changer le last hitting mallet
                if(maillet && rondelle && mGame->isNetworkServerGame())
                {
                    PaquetGameEvent* wPaquet = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
                    wPaquet->setGameId(mGame->getUniqueGameId());
                    wPaquet->setEvent(GAME_EVENT_CHANGE_LAST_MALLET);
                    wPaquet->setEventOnPlayerLeft(maillet->estAGauche());
                    RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquet->getGameId(), wPaquet, TCP);
                }
                rondelle->setLastHittingMallet(maillet);
            }
            break;
        case CATEGORY_PORTAL  :
            {
                NoeudPortail* portail = (NoeudPortail*)bodies[1]->GetUserData();
                checkf(portail, "Portal's body's User Data is not the Portal");
                
                if(portail->isAttractionFieldActive())
                {
                    // Collision avec un portail et téléportation
                    NoeudGroupe *groupe = dynamic_cast<NoeudGroupe *>(portail->getParent());
                    if(groupe)
                    {
                        unsigned int nbEnfant = groupe->childCount();

                        if( nbEnfant > 1 )
                        {
                            // Choix aléatoire du portail de sortie
                            NoeudPortail* portailDeSortie = NULL;
                            while(!portailDeSortie || portailDeSortie == portail)
                            {
                                int noPortailDeSortie = rand()%nbEnfant;
                                portailDeSortie = dynamic_cast<NoeudPortail*>(groupe->find(noPortailDeSortie));
                            }

                            NoeudRondelle* rondelle = (NoeudRondelle*)bodies[0]->GetUserData();

                            // The new pos can only be assigned outside of the world's step, so we queue it
                            Runnable* r = new Runnable([=](Runnable*)
                            {
                                portailDeSortie->setIsAttractionFieldActive(false);
                                auto newPos = portailDeSortie->getPosition();
                                rondelle->setPosition(newPos);
                                SoundFMOD::obtenirInstance()->playEffect(effect(PORTAL_EFFECT));
                            });
                            RunnableBreaker::attach(r);
                            RazerGameUtilities::RunOnUpdateThread(r,true);
                            return;
                        }
                    }
                }
            }
            break;
        case CATEGORY_BOOST   :
            {
                NoeudAccelerateur* accelerateur = (NoeudAccelerateur*)bodies[1]->GetUserData();
                checkf(accelerateur, "Boost's body's User Data is not the boost");

                auto bonusAccel = accelerateur->obtenirBonusAccel();
                auto linearVelocity = bodies[0]->GetLinearVelocity();

                linearVelocity *= bonusAccel;
                bodies[0]->SetLinearVelocity(linearVelocity);

                SoundFMOD::obtenirInstance()->playEffect(effect(ACCELERATOR_EFFECT));
            }
            break;
        case CATEGORY_BONUS:
            {
                NodeBonus* bonus = (NodeBonus*)(bodies[1]->GetUserData());
                NoeudRondelle* rondelle = (NoeudRondelle*)(bodies[0]->GetUserData());

                // The execution ca modify box2D so we queue it
                Runnable* r = new Runnable([bonus,rondelle](Runnable*)
                {
                    bonus->ExecuteBonus(rondelle); 
                });
                RunnableBreaker::attach(r);
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        default:
            break;
        }
    }
    
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::EndContact( b2Contact* contact )
///
/// Callback after the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::EndContact( b2Contact* contact )
{
    bool isEnabled = contact->IsEnabled();
    bool isTouching = contact->IsTouching();

    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    auto bodyA = fixtureA->GetBody();
    auto bodyB = fixtureB->GetBody();
    auto filterA = fixtureA->GetFilterData();
    auto filterB = fixtureB->GetFilterData();
    short category = filterA.categoryBits | filterB.categoryBits;
    bool isPuckPresent = !!(( category ) & CATEGORY_PUCK );

    // la rondelle est sortie d'un portal
    if( category == (CATEGORY_PUCK|CATEGORY_PORTAL) )
    {
        b2Body* portalBody = NULL;
        if(filterA.categoryBits & CATEGORY_PORTAL)
        {
            portalBody = bodyA;
        }
        else
        {
            portalBody = bodyB;
        }

        NoeudPortail* portal = (NoeudPortail*)portalBody->GetUserData();
        checkf(portal, "Portal's body's User Data is not the Portal");

        portal->setIsAttractionFieldActive(true);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
///
/// Callback before the solving the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
/// @param[in] const b2Manifold * oldManifold
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
    B2_NOT_USED(contact);
    B2_NOT_USED(oldManifold);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
///
/// Callback after the solving the contact between 2 fixtures
///
/// @param[in] b2Contact * contact
/// @param[in] const b2ContactImpulse * impulse
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
    B2_NOT_USED(contact);
    B2_NOT_USED(impulse);
}
#endif


////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::getGoals( NoeudBut** pOutGoals )
///
/// /*Description*/
///
/// @param[in] NoeudBut * * pOutGoals
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::getGoals( NoeudBut** pOutGoals )
{
    pOutGoals[0] = NULL;
    pOutGoals[1] = NULL;

    NoeudTable* table = getTable();
    if(table)
    {
        pOutGoals[0] = table->obtenirBut(1);
        pOutGoals[1] = table->obtenirBut(2);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::fullRebuild()
///
/// Updates the content of the game to be ready to play
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VISITEUR_FUNC_FUNC_DECLARATION(ForceFullRebuildFunc)
{
    pNoeud->forceFullUpdate();
}
void Terrain::fullRebuild()
{
    checkf(mLogicTree,"Requete pour un full rebuild d'un terrain sans arbre");
    if(mLogicTree)
    {
        VisiteurFunction v(ForceFullRebuildFunc);
        mLogicTree->acceptVisitor(v);
    }
    if(mEditionZone)
    {
        mEditionZone->rebuild();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::setTableControlPointVisible( bool pVisible )
///
/// Show or hide the control points of the table ( they are hidden during play )
///
/// @param[in] bool pVisible
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::setTableControlPointVisible( bool pVisible )
{
    if(mTable)
    {
        for (int i = 0; i < NoeudTable::NB_CONTROL_POINTS ; ++i)
        {
            // the control points always exists with the table
            mTable->obtenirPoint(i)->setVisible(pVisible);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::deselectTableItems()
///
/// removes user selection on all nodes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::setTableItemsSelection( bool pSelect )
{
    if(mLogicTree)
    {
        if(pSelect)
        {
            mLogicTree->selectAll();
        }
        else
        {
            mLogicTree->deselectAll();
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::IsNodeSelected()
///
/// Indicates if any node in the logic tree is selected
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::IsAnyNodeSelected() const
{
    return mLogicTree && mLogicTree->selectionPresent();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::acceptVisitor( class VisiteurNoeud* visiteur )
///
/// Launch a visitor on the field
///
/// @param[in] class VisiteurNoeud * visiteur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::acceptVisitor( class VisiteurNoeud& visitor )
{
    if(mLogicTree)
    {
        mLogicTree->acceptVisitor(visitor);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::duplicateSelection()
///
/// duplicate nodes selected that can be duplicated
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::duplicateSelection()
{
    if(mLogicTree)
    {
        VisiteurDupliquer v(mLogicTree);
        acceptVisitor(v);
    }
    pushUndoState();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet* Terrain::getLeftMallet()
///
/// accesseur du maillet de gauche
///
///
/// @return NoeudMaillet*
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet* Terrain::getLeftMallet() const
{
    if(mLeftMallet)
    {
        checkf(IsGameField(), "Dans le mode édition on ne conserve pas de pointeur sur le maillet");
        return mLeftMallet;
    }
    checkf(!IsGameField(), "Dans le mode jeu on doit avoir un pointeur sur le maillet");

    NoeudMaillet* maillet = NULL;
    if(getTable())
    {
        NoeudGroupe* g = getTable()->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
        checkf(g, "Groupe pour les maillets manquant");
        if(g)
        {
            for(unsigned int i=0; i<g->childCount(); ++i)
            {
                NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->find(i));
                if( m->getPosition()[VX] <= 0 )
                    maillet = m;
            }
        }
    }
    return maillet;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet* Terrain::getRightMallet()
///
/// accesseur du maillet de droite
///
///
/// @return NoeudMaillet*
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet* Terrain::getRightMallet() const
{
    if(mRightMallet)
    {
        checkf(IsGameField(), "Dans le mode édition on ne conserve pas de pointeur sur le maillet");
        return mRightMallet;
    }
    checkf(!IsGameField(), "Dans le mode jeu on doit avoir un pointeur sur le maillet");

    NoeudMaillet* maillet = NULL;
    if(getTable())
    {
        NoeudGroupe* g = getTable()->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
        checkf(g, "Groupe pour les maillets manquant");
        if(g)
        {
            for(unsigned int i=0; i<g->childCount(); ++i)
            {
                NoeudMaillet* m = dynamic_cast<NoeudMaillet *>(g->find(i));
                if(m->getPosition()[VX]>0)
                    maillet = m;
            }
        }
    }
    return maillet;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::IsNodeAtValidEditionPosition( NoeudAbstrait* pNode, bool pDoHightlightNodeInCollision /*= false*/ )
///
/// indicate if the node can be released at that position safely
///
/// @param[in] NoeudAbstrait * pNode : node to validate
/// @param[in] bool pDoHightlightNodeInCollision : when true, node in collision with the node to validate will be highlighted
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::IsNodeAtValidEditionPosition( NoeudAbstrait* pNode, bool pDoHightlightNodeInCollision /*= false*/ )
{
    VisiteurCollision visiteur(pNode,pDoHightlightNodeInCollision);
    acceptVisitor(visiteur);
    return !(visiteur.collisionPresente()) && insideLimits(pNode);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::FixCollidingObjects()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::FixCollidingObjects()
{
#if BOX2D_INTEGRATED  
    mWorld->Step(0.001f,0,1000);
    mWorld->Step(0.001f,0,1000);
    mWorld->Step(0.001f,0,1000);
    return true;
#else
    bool tableValide = false;

    static const int n = 5;
    static const std::string groups[n] = {
        RazerGameUtilities::NOM_ACCELERATEUR,
        RazerGameUtilities::NOM_RONDELLE,
        RazerGameUtilities::NOM_MURET,
        RazerGameUtilities::NOM_PORTAIL,
        RazerGameUtilities::NOM_MAILLET,
    };

    for(unsigned int iter = 0; iter < 10 && !tableValide ; ++iter)
    {
        tableValide = true;
        for(int g=0; g<n; ++g)
        {
            NoeudGroupe* groupe = mTable->obtenirGroupe(groups[g]);
            if(groupe)
            {
                for (auto it = groupe->obtenirEnfants().begin(); it != groupe->obtenirEnfants().end(); it++)
                {
                    tableValide &= FixCollindingNode(*it,2);
                }
            }
        }
    }
    return tableValide;
#endif //BOX2D_INTEGRATED
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::FixCollindingNode( NoeudAbstrait* node )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * node
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::FixCollindingNode( NoeudAbstrait* pNode, unsigned int nbIterations )
{
    // Si l'un des pNode est null, on ne fait pas le travail
    if(!pNode)
        return true;

    // Avant les tests de collision, on regarde d'abord si le pNode est dans les limites
    if(!insideLimits(pNode))
    {
        // Repositionne au centre de la table
        pNode->setPosition(Vecteur3());
    }

    VisiteurCollision v(pNode,false);
    acceptVisitor(v);

    unsigned int tentative = 0;
    while( v.collisionPresente() && ++tentative <= nbIterations)
    {
        ConteneurNoeuds liste;
        ConteneurDetailsCollision details = v.obtenirConteneurDetailsCollision();
        v.obtenirListeCollision(liste);
        for (int j = 0; j < liste.size()  ; j++)
        {
            //Vecteur3 deplacement(elementSurTable_[i]->getPosition() - liste[j]->getPosition());
            Vecteur2 deplacement( details[j].direction*details[j].enfoncement );
            if(deplacement.norme() == 0)
                deplacement = Vecteur3(1.0,1.0);
            //deplacement.normaliser();
            VisiteurDeplacement vDeplacement(deplacement);
            pNode->acceptVisitor(vDeplacement);
        }
        if(!insideLimits(pNode))
        {
            // Repositionne au centre de la table
            pNode->setPosition(Vecteur3());
        }
        v.reinitialiser();
        acceptVisitor(v);
    }

    // Il faut la partie égale de la comparaison, car le ++tentative 
    // de la condition while ne ce fera pas s'il n'y a plus de collision
    return tentative <= nbIterations;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float Terrain::GetTableWidth()
///
/// /*Description*/
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float Terrain::GetTableWidth() const
{
    if(mTable)
    {
        return mTable->GetWidth();
    }
    if(mEditionZone)
    {
        return mEditionZone->obtenirLimiteExtY()*2.f;
    }
    return ZoneEdition::DEFAUT_LIMITE_EXT_Y*2.f;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::NodeSelectionNotification( NoeudAbstrait* node, bool selectionne_ )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * node
/// @param[in] bool selectionne_
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::NodeSelectionNotification( NoeudAbstrait* node, bool selected )
{
    int nbSelectedOld = (int)mSelectedNodes.size();
    if(selected)
    {
        mSelectedNodes.insert(node);
    }
    else
    {
        auto it = mSelectedNodes.find(node);
        if(it != mSelectedNodes.end())
        {
            mSelectedNodes.erase(it);
        }
    }
    int nbSelectedNew = (int)mSelectedNodes.size();
    if(nbSelectedOld != nbSelectedNew)
    {
        if( nbSelectedNew == 0 )
        {
            // no more item selected
            TransmitEvent(THERE_ARE_NO_NODE_SELECTED);
        }
        else
        {
            // selection present
            TransmitEvent(THERE_ARE_NODES_SELECTED);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::CanSelectedNodeBeDeleted()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::CanSelectedNodeBeDeleted() const
{
    for(auto it=mSelectedNodes.begin(); it != mSelectedNodes.end(); ++it)
    {
        NoeudPoint *point = dynamic_cast<NoeudPoint *>(*it);
        if (!point)
        {
            // seulement les noeud point sont selectionnable et ne peuvent etre deleted so haha
            // dont ask dont tell....
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::initNecessaryPointersForGame()
///
/// Assigne les pointeurs sur les maillets et la rondelle pour la partie
/// Lance une exception si quelque chose manque
/// ie : mode édition => check table seulement  
/// mode Jeu => check table, rondelle, maillets
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
void Terrain::initNecessaryPointersForGame()
{
    mLeftMallet = NULL;
    mRightMallet = NULL;
    mPuck = NULL;

    mTable = mLogicTree->obtenirTable();
    if(mTable == NULL)
    {
        throw ExceptionJeu("Missing Table on field");
    }

    if(IsGameField())
    {
        NoeudMaillet* mailletFound[2] = {NULL,NULL};
        {
            NoeudGroupe* g = mTable->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
            checkf(g, "Groupe pour les maillets manquant");
            if(g)
            {
                for(unsigned int i=0; i<g->childCount() && i<2; ++i)
                {
                    NoeudMaillet* m = (NoeudMaillet*)(g->find(i));
                    mailletFound[i] = m;
                }
            }
        }
        {
            NoeudGroupe* g = mTable->obtenirGroupe(RazerGameUtilities::NOM_RONDELLE);
            checkf(g, "Groupe pour la rondelle manquante");
            if(g)
            {
                for(unsigned int i=0; i<g->childCount(); ++i)
                {
                    NoeudRondelle* r = dynamic_cast<NoeudRondelle *>(g->find(i));
                    if(r)
                    {
                        mPuck = r;
                    }
                }
            }
        }

        if(!mPuck)
        {
            throw ExceptionJeu("Missing puck on table");
        }
        if(!mailletFound[0] || !mailletFound[1])
        {
            throw ExceptionJeu("Missing mallet on table");
        }

        if(mailletFound[1]->getPosition()[VX] < mailletFound[0]->getPosition()[VX])
        {
            mLeftMallet = mailletFound[1];
            mRightMallet = mailletFound[0];
        }
        else
        {
            mLeftMallet = mailletFound[0];
            mRightMallet = mailletFound[1];
        }

        // Assignation des position de base des elements pour la reinitialisation suite a un but
        mLeftMallet->modifierPositionOriginale(mLeftMallet->getPosition());
        mRightMallet->modifierPositionOriginale(mRightMallet->getPosition());
        mPuck->validerPropriteteTablePourJeu();
        mPuck->modifierPositionOriginale(mPuck->getPosition());

#if WIN32
        if (GestionnaireHUD::Exists())
        {
	        auto leftBonuses = GestionnaireHUD::obtenirInstance()->getLeftPlayerBonuses();
	        leftBonuses->setModifiers(&mLeftMallet->GetModifiers());
	
	        auto rightBonuses = GestionnaireHUD::obtenirInstance()->getRightPlayerBonuses();
	        rightBonuses->setModifiers(&mRightMallet->GetModifiers());
        }
#endif

    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn RazerKey Terrain::getSelectedNodeUniqueKey()
///
/// checks if selected nodes are the same type and returns that type
/// if not, return RAZER_KEY_NONE
///
///
/// @return RazerKey
///
////////////////////////////////////////////////////////////////////////
RazerKey Terrain::getSelectedNodeUniqueKey() const
{
    RazerKey key = RAZER_KEY_NONE;
    for(auto node = mSelectedNodes.begin(); node != mSelectedNodes.end();++node)
    {
        RazerKey nodeKey = (*node)->getKey();
        if(key != RAZER_KEY_NONE && key != nodeKey)
        {
            /// found 2 nodes of different type
            return RAZER_KEY_NONE;
        }
        key = nodeKey;
    }
    return key;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int Terrain::gatherSelectedNodeProperties( FullProperties* properties )
///
/// /*Description*/
///
/// @param[in] FullProperties * properties
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int Terrain::gatherSelectedNodeProperties( FullProperties* properties )
{
    if(properties)
    {
        VisitorGatherProperties v(properties);
        RazerKey key = getSelectedNodeUniqueKey();
        if(key == RAZER_KEY_NONE)
        {
            if(mTable)
            {
                mTable->acceptVisitor(v);
            }
        }
        else
        {
            BoundingBox aabb;
            for(auto it=mSelectedNodes.begin(); it != mSelectedNodes.end(); ++it)
            {
                aabb += (*it)->getPosition();
                (*it)->acceptVisitor(v);
            }
            auto center = aabb.GetCenter();
            properties->mPositionX = center[VX];
            properties->mPositionY = center[VY];

            /// les assignation d'angle sont toujours relatif et non absolu, donc
            /// on affiche 0 
            properties->mAngle = 0;
            properties->mPropertyFlagAssignment |= 1<<ASSIGNED_ANGLE;
            properties->mPropertyFlagAssignment &= ~(1<<INVALID_ANGLE);
        }

        // permet de savoir si quelque chose a ete assigné
        return properties->mPropertyFlagAssignment != 0;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int Terrain::applySelectedNodeProperties( class FullProperties* properties )
///
/// /*Description*/
///
/// @param[in] class FullProperties * properties
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int Terrain::applySelectedNodeProperties( class FullProperties* properties )
{
    if(properties)
    {
        RazerKey key = getSelectedNodeUniqueKey();
        BoundingBox aabb;
        if(key == RAZER_KEY_NONE)
        {
            if(mTable)
            {
                VisitorSetProperties v(properties,aabb);
                mTable->acceptVisitor(v);
            }
        }
        else
        {
            for(auto it=mSelectedNodes.begin(); it != mSelectedNodes.end(); ++it)
            {
                aabb += (*it)->getPosition();
            }
            VisitorSetProperties v(properties,aabb);
            for(auto it=mSelectedNodes.begin(); it != mSelectedNodes.end(); ++it)
            {
                (*it)->acceptVisitor(v);
            }
        }
        FixCollidingObjects();
        pushUndoState();
    }
    return 1;
}

int Terrain::BeginModification(FieldModificationStrategyType type, const FieldModificationStrategyEvent& beginEvent)
{
    if(mModifStrategy)
    {
        delete mModifStrategy;
    }
    mModifStrategy = NULL;

    switch (type)
    {
    case FIELD_MODIFICATION_NONE:
        break;
    case FIELD_MODIFICATION_MOVE: 
        {
            FieldModificationStrategyMove* moveModif = new FieldModificationStrategyMove(this,beginEvent);
            mModifStrategy = moveModif;
        }
        break;
    case FIELD_MODIFICATION_ROTATE: 
        {
            FieldModificationStrategyRotate* rotateModif = new FieldModificationStrategyRotate(this,beginEvent);
            mModifStrategy = rotateModif;
        }
        break;
    case FIELD_MODIFICATION_SCALE: 
        {
            FieldModificationStrategyScale* scaleModif = new FieldModificationStrategyScale(this,beginEvent);
            mModifStrategy = scaleModif;
        }
        break;
    case FIELD_MODIFICATION_ADD_PORTAL:
        {
            FieldModificationStrategyAddNode* addModif = new FieldModificationStrategyAddNode(this,beginEvent,RazerGameUtilities::NOM_PORTAIL);
            mModifStrategy = addModif;
        }
        break;
    case FIELD_MODIFICATION_ADD_BOOST:
        {
            FieldModificationStrategyAddNode* addModif = new FieldModificationStrategyAddNode(this,beginEvent,RazerGameUtilities::NOM_ACCELERATEUR);
            mModifStrategy = addModif;
        }
        break;
    case FIELD_MODIFICATION_ADD_WALL:
        {
            FieldModificationStrategyAddWall* addModif = new FieldModificationStrategyAddWall(this,beginEvent,RazerGameUtilities::NOM_MURET);
            mModifStrategy = addModif;
        }
        break;
    case FIELD_MODIFICATION_ADD_MALLET:
        {
            FieldModificationStrategyAddNode* addModif = new FieldModificationStrategyAddNode(this,beginEvent,RazerGameUtilities::NOM_MAILLET);
            mModifStrategy = addModif;
        }
        break;
    case FIELD_MODIFICATION_ADD_PUCK:
        {
            FieldModificationStrategyAddNode* addModif = new FieldModificationStrategyAddNode(this,beginEvent,RazerGameUtilities::NOM_RONDELLE);
            mModifStrategy = addModif;
        }
        break;
    case FIELD_MODIFICATION_ADD_BONUS:
        {
            FieldModificationStrategyAddNode* addModif = new FieldModificationStrategyAddNode(this,beginEvent,RazerGameUtilities::NAME_BONUS);
            mModifStrategy = addModif;
        }
        break;
    default:
        checkf(0,"Unknown modification type");
        break;
    }

    return !!mModifStrategy;
}
int Terrain::ReceiveModificationEvent(const FieldModificationStrategyEvent& pEvent)
{
    int r = 0;
    if(mModifStrategy)
    {
        r = mModifStrategy->receivedEvent(pEvent);
    }
    return r;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::cancelModification()
///
/// Cancels current modification
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::cancelModification()
{
    if(mModifStrategy)
    {
        mModifStrategy->cancelStratedy();
        delete mModifStrategy;
    }
    mModifStrategy = NULL;
}


int Terrain::EndModification()
{
    int r = 0;
    if(mModifStrategy)
    {
        r = mModifStrategy->endStrategy();
        delete mModifStrategy;

        pushUndoState();
    }
    mModifStrategy = NULL;
    return r;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool Terrain::equals( Terrain* terrain )
///
/// checks if both field are the same
///
/// @param[in] Terrain * terrain
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool Terrain::equals( Terrain* terrain )
{
    if(!terrain)
    {
        return false;
    }
    if(terrain == this)
    {
        return true;
    }


    if(IsGameField() != terrain->IsGameField())
    {
        return false;
    }

    auto tree1 = getLogicTree();
    auto tree2 = terrain->getLogicTree();
    if( (!tree1 && tree2 )|| (tree1 && !tree2 ) )
    {
         return false;
    }
    if(tree1)
    {
        if(!tree1->equals(tree2))
        {
            return false;
        }
    }

    auto zone1 = getZoneEdition();
    auto zone2 = terrain->getZoneEdition();
    if(zone1)
    {
        if(!zone1->equals(zone2))
        {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::visitSelectedNodes( VisiteurNoeud& visitor )
///
/// /*Description*/
///
/// @param[in] VisiteurNoeud & visitor
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::visitSelectedNodes( VisiteurNoeud& visitor )
{
    for(auto it=mSelectedNodes.begin(); it != mSelectedNodes.end(); ++it)
    {
        (*it)->acceptVisitor(visitor);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int Terrain::undoModification()
///
/// /*Description*/
///
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int Terrain::undoModification()
{
    if(mUndoBuffer.size())
    {
        mDoingUndoRedo = true;
        auto elem = mUndoBuffer.back();
        
        /// L'état courant est poussé dans la pile de Redo pour le reappliquer plus tard
        /// L'etat dans le undo buffer, devient l'état courant

        mUndoBuffer.pop_back();
        mRedoBuffer.push_back(mCurrentState);
        mCurrentState = elem;

        initialiserXml(elem,false);
        mDoingUndoRedo = false;
        if(mUndoBuffer.empty())
        {
            TransmitEvent(CANNOT_UNDO);
        }
        TransmitEvent(CAN_REDO);

        return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int Terrain::redoModification()
///
/// /*Description*/
///
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int Terrain::redoModification()
{
    if(mRedoBuffer.size())
    {
        mDoingUndoRedo = true;
        auto elem = mRedoBuffer.back();

        mRedoBuffer.pop_back();
        mUndoBuffer.push_back(mCurrentState);
        mCurrentState = elem;

        initialiserXml(elem,false);
        mDoingUndoRedo = false;

        if(mRedoBuffer.empty())
        {
            TransmitEvent(CANNOT_REDO);
        }
        TransmitEvent(CAN_UNDO);

        return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::pushUndoState()
///
/// Adds an undo state onto the stack and empties the redo stack
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::pushUndoState()
{
    checkf(mCurrentState, "Current Undo State inexistant when trying to push state");
    if(mCurrentState)
    {
        mUndoBuffer.push_back(mCurrentState);
    }

    mCurrentState = creerNoeudXML();

    if(mUndoBuffer.size() > UNDO_BUFFERSIZE)
    {
        delete mUndoBuffer.front();
        mUndoBuffer.pop_front();
    }

    for(auto it=mRedoBuffer.begin(); it!= mRedoBuffer.end(); ++it)
    {
        delete (*it);
    }
    mRedoBuffer.clear();

    TransmitEvent(CAN_UNDO);
    TransmitEvent(CANNOT_REDO);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::reApplyCurrentState()
///
/// cancels current modifications and reset the field to the last known correct state
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::reApplyCurrentState()
{
    if(mCurrentState)
    {
        mDoingUndoRedo = true;
        initialiserXml(mCurrentState,false);
        mDoingUndoRedo = false;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Terrain::deleteSelectedNodes()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Terrain::deleteSelectedNodes()
{
    VisiteurSuppression v;
    acceptVisitor(v);
    pushUndoState();
}




///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////