///////////////////////////////////////////////////////////////////////////////
/// @file NoeudTable.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "NodeRinkBoards.h"
#include "BoundingBox.h"
#include "ExceptionJeu.h"

#if WIN32
#include "GestionnaireModeles.h"
#include <GL\glu.h>
#include "FacadeModele.h"
#endif

#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "AideCollision.h"
#include "NoeudGroupe.h"
#include <math.h>
#include "XMLUtils.h"
#include "ArbreRendu.h"
#include "Terrain.h"
#include "Solution_Defines.h"
#include "VisiteurNoeud.h"
#include "NoeudRondelle.h"

ListeIndexPoints NoeudTable::listeIndexPointsModeleTable_ = ListeIndexPoints();
const Vecteur3 NoeudTable::DEFAULT_SIZE = Vecteur3(300,150);
const float NoeudTable::rayonCercleCentre_ = 25;
const float Z_HEIGHT_TABLE_SURFACE = -1;               // hauteur en z ou ce trouve la surface de la table
const float Z_HEIGHT_TABLE_BOARDS = 16;                // hauteur en z ou ce trouve le top de la bordure de la table
const float Z_HEIGHT_TABLE_EXTERIOR_BORDERS = 11;      // hauteur en z ou ce trouve le top du contour exterieur de la table
const float Z_HEIGHT_TABLE_UNDER = -73;                // hauteur en z du dessous de la table
const float Z_HEIGHT_TABLE_BOTTOM = -133;              // hauteur en z du bas des pieds de la table


const int nbCorners = 4;
const int nodeHeight[3] = {0,NoeudTable::NB_VERTICAL_VERTICES>>1,NoeudTable::NB_VERTICAL_VERTICES-1};
const int nodeWidth[3] = {0,NoeudTable::NB_HORIZONTAL_VERTICES>>1,NoeudTable::NB_HORIZONTAL_VERTICES-1};

const std::string BonusXmlTag = "BonusProp";


CreateListDelegateImplementation(Table)
{
#if WIN32  
    NoeudTable::initialiserListeIndexPoints(pModel);
#endif
    // dont make a list for table
    return 0;
}

const std::string* GroupTypes[] =
{
    &RazerGameUtilities::NAME_TABLE_CONTROL_POINT ,
    &RazerGameUtilities::NAME_RINK_BOARD          ,
    &RazerGameUtilities::NOM_MAILLET              ,
    &RazerGameUtilities::NOM_RONDELLE             ,
    &RazerGameUtilities::NOM_ACCELERATEUR         ,
    &RazerGameUtilities::NOM_MURET                ,
    &RazerGameUtilities::NOM_PORTAIL              ,
    &RazerGameUtilities::NAME_BONUS               ,
    &RazerGameUtilities::NAME_POLYGONE            ,
};
const int NB_GROUP_TYPES = ARRAY_COUNT(GroupTypes);


#if WIN32  

enum ListePointState
{
    LISTEPOINTSTATE_OK,
    LISTEPOINTSTATE_PENDING,
    LISTEPOINTSTATE_ERROR,
};
ListePointState listePointInit = LISTEPOINTSTATE_PENDING;
struct queuedMove
{
    TypePosPoint type;
    Vecteur3 move;
};
std::vector<queuedMove> moveQueue;
#include "..\Reseau\FacadePortability.h"
HANDLE_MUTEX mutex = NULL;

void emptyQueue()
{
    while(!moveQueue.empty())
    {
        queuedMove& q = moveQueue.back();
        auto iter = NoeudTable::obtenirListeIndexPointsModeleTable().find(q.type);
        if(iter != NoeudTable::obtenirListeIndexPointsModeleTable().end())
        {
            // si on ne trouve pas la liste ici, c'est que le modele n'existe pas
            auto liste = &iter->second;
            for(unsigned int i=0; i<liste->size(); i++)
            {
                *(liste->get(i)[VX]) += q.move[VX];
                *(liste->get(i)[VY]) += q.move[VY];
            }
        }
        moveQueue.pop_back();
    }
    FacadePortability::releaseMutex(mutex);
    CloseHandle(mutex);
    mutex = NULL;
}


void NoeudTable::queueTableModelMove(TypePosPoint type,const Vecteur3& move)
{
    if(listePointInit != LISTEPOINTSTATE_ERROR)
    {
        if(!mutex)
        {
            FacadePortability::createMutex(mutex);
        }
        FacadePortability::takeMutex(mutex);
        // queue the move
        queuedMove qmove;
        qmove.type = type;
        qmove.move = move;
        moveQueue.push_back(qmove);

        // if it finish initialise since then, apply the modifs
        if(listePointInit == LISTEPOINTSTATE_OK)
        {
            emptyQueue();
        }
        if(mutex)FacadePortability::releaseMutex(mutex);

    }
    else
    {
        moveQueue.clear();
    }
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::initialiserListeIndexPoints()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::initialiserListeIndexPoints( Modele3D* modele )
{
    if(!mutex)
    {
        FacadePortability::createMutex(mutex);
    }
    if(!modele)
    {
        FacadePortability::takeMutex(mutex);
        listePointInit = LISTEPOINTSTATE_ERROR;
        FacadePortability::releaseMutex(mutex);
        CloseHandle(mutex);
        mutex = NULL;
        return;
    }

    const aiScene* scene = modele->obtenirScene();
    const aiNode* rootNode = scene->mRootNode;

    GroupeCoord vertexBasMilieu;
    vertexBasMilieu.push_back   (Vecteur3i(0   , -75 , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexBasMilieu.push_back   (Vecteur3i(0   , -75 , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexBasMilieu.push_back   (Vecteur3i(0   , -82 , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexBasMilieu.push_back   (Vecteur3i(0   , -82 , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexBasMilieu.push_back   (Vecteur3i(0   , -102, (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexBasMilieu.push_back   (Vecteur3i(0   , -93 , (int)Z_HEIGHT_TABLE_UNDER));
                                                       
    GroupeCoord vertexHautMilieu;                      
    vertexHautMilieu.push_back  (Vecteur3i(0   , 75  , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexHautMilieu.push_back  (Vecteur3i(0   , 75  , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexHautMilieu.push_back  (Vecteur3i(0   , 82  , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexHautMilieu.push_back  (Vecteur3i(0   , 82  , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexHautMilieu.push_back  (Vecteur3i(0   , 102 , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexHautMilieu.push_back  (Vecteur3i(0   , 92  , (int)Z_HEIGHT_TABLE_UNDER));
                                                       
    GroupeCoord vertexMilieuGauche;                    
    vertexMilieuGauche.push_back(Vecteur3i(-173, 0   , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexMilieuGauche.push_back(Vecteur3i(-156, 0   , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexMilieuGauche.push_back(Vecteur3i(-156, 0   , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexMilieuGauche.push_back(Vecteur3i(-150, 0   , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexMilieuGauche.push_back(Vecteur3i(-150, 0   , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexMilieuGauche.push_back(Vecteur3i(-156, 0   , (int)Z_HEIGHT_TABLE_UNDER));
                                                       
    GroupeCoord vertexMilieuDroit;                     
    vertexMilieuDroit.push_back (Vecteur3i(173 , 0   , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexMilieuDroit.push_back (Vecteur3i(156 , 0   , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexMilieuDroit.push_back (Vecteur3i(156 , 0   , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexMilieuDroit.push_back (Vecteur3i(150 , 0   , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexMilieuDroit.push_back (Vecteur3i(150 , 0   , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexMilieuDroit.push_back (Vecteur3i(157 , 0   , (int)Z_HEIGHT_TABLE_UNDER));
                                                       
    GroupeCoord vertexHautGauche;                      
    vertexHautGauche.push_back  (Vecteur3i(-173, 102 , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexHautGauche.push_back  (Vecteur3i(-156, 82  , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexHautGauche.push_back  (Vecteur3i(-156, 82  , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexHautGauche.push_back  (Vecteur3i(-150, 75  , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexHautGauche.push_back  (Vecteur3i(-150, 75  , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexHautGauche.push_back  (Vecteur3i(-156, 92  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautGauche.push_back  (Vecteur3i(-110, 65  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautGauche.push_back  (Vecteur3i(-110, 31  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautGauche.push_back  (Vecteur3i(-81 , 65  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautGauche.push_back  (Vecteur3i(-81 , 31  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautGauche.push_back  (Vecteur3i(-95 , 48  , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexHautGauche.push_back  (Vecteur3i(-124, 48  , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexHautGauche.push_back  (Vecteur3i(-124, 82  , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexHautGauche.push_back  (Vecteur3i(-95 , 82  , (int)Z_HEIGHT_TABLE_BOTTOM));
                                                       
    GroupeCoord vertexBasGauche;                       
    vertexBasGauche.push_back   (Vecteur3i(-173, -102, (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexBasGauche.push_back   (Vecteur3i(-156, -82 , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexBasGauche.push_back   (Vecteur3i(-156, -82 , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexBasGauche.push_back   (Vecteur3i(-150, -75 , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexBasGauche.push_back   (Vecteur3i(-150, -75 , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexBasGauche.push_back   (Vecteur3i(-156, -93 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasGauche.push_back   (Vecteur3i(-110, -65 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasGauche.push_back   (Vecteur3i(-110, -31 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasGauche.push_back   (Vecteur3i(-81 , -65 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasGauche.push_back   (Vecteur3i(-81 , -31 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasGauche.push_back   (Vecteur3i(-95 , -48 , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexBasGauche.push_back   (Vecteur3i(-124, -48 , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexBasGauche.push_back   (Vecteur3i(-124, -82 , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexBasGauche.push_back   (Vecteur3i(-95 , -82 , (int)Z_HEIGHT_TABLE_BOTTOM));
                                                       
    GroupeCoord vertexHautDroit;                       
    vertexHautDroit.push_back   (Vecteur3i(173 , 102 , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexHautDroit.push_back   (Vecteur3i(156 , 82  , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexHautDroit.push_back   (Vecteur3i(156 , 82  , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexHautDroit.push_back   (Vecteur3i(150 , 75  , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexHautDroit.push_back   (Vecteur3i(150 , 75  , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexHautDroit.push_back   (Vecteur3i(157 , 92  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautDroit.push_back   (Vecteur3i(110 , 65  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautDroit.push_back   (Vecteur3i(110 , 31  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautDroit.push_back   (Vecteur3i(82  , 65  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautDroit.push_back   (Vecteur3i(82  , 31  , (int)Z_HEIGHT_TABLE_UNDER));
    vertexHautDroit.push_back   (Vecteur3i(96  , 48  , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexHautDroit.push_back   (Vecteur3i(125 , 48  , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexHautDroit.push_back   (Vecteur3i(125 , 82  , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexHautDroit.push_back   (Vecteur3i(96  , 82  , (int)Z_HEIGHT_TABLE_BOTTOM));
                                                       
    GroupeCoord vertexBasDroit;                        
    vertexBasDroit.push_back    (Vecteur3i(173 , -102, (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexBasDroit.push_back    (Vecteur3i(156 , -82 , (int)Z_HEIGHT_TABLE_EXTERIOR_BORDERS));
    vertexBasDroit.push_back    (Vecteur3i(156 , -82 , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexBasDroit.push_back    (Vecteur3i(150 , -75 , (int)Z_HEIGHT_TABLE_BOARDS));
    vertexBasDroit.push_back    (Vecteur3i(150 , -75 , (int)Z_HEIGHT_TABLE_SURFACE));
    vertexBasDroit.push_back    (Vecteur3i(157 , -93 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasDroit.push_back    (Vecteur3i(110 , -65 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasDroit.push_back    (Vecteur3i(110 , -31 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasDroit.push_back    (Vecteur3i(82  , -65 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasDroit.push_back    (Vecteur3i(82  , -31 , (int)Z_HEIGHT_TABLE_UNDER));
    vertexBasDroit.push_back    (Vecteur3i(96  , -48 , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexBasDroit.push_back    (Vecteur3i(125 , -48 , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexBasDroit.push_back    (Vecteur3i(125 , -82 , (int)Z_HEIGHT_TABLE_BOTTOM));
    vertexBasDroit.push_back    (Vecteur3i(96  , -82 , (int)Z_HEIGHT_TABLE_BOTTOM));

    listeIndexPointsModeleTable_[POSITION_BAS_MILIEU] = trouverVertex(scene, rootNode, vertexBasMilieu);
    listeIndexPointsModeleTable_[POSITION_HAUT_MILIEU]  = trouverVertex(scene, rootNode, vertexHautMilieu);
    listeIndexPointsModeleTable_[POSITION_MILIEU_GAUCHE]  = trouverVertex(scene, rootNode, vertexMilieuGauche);
    listeIndexPointsModeleTable_[POSITION_MILIEU_DROITE]  = trouverVertex(scene, rootNode, vertexMilieuDroit);
    listeIndexPointsModeleTable_[POSITION_HAUT_GAUCHE] = trouverVertex(scene, rootNode, vertexHautGauche);
    listeIndexPointsModeleTable_[POSITION_BAS_GAUCHE]  = trouverVertex(scene, rootNode, vertexBasGauche);
    listeIndexPointsModeleTable_[POSITION_HAUT_DROITE]  = trouverVertex(scene, rootNode, vertexHautDroit);
    listeIndexPointsModeleTable_[POSITION_BAS_DROITE]  = trouverVertex(scene, rootNode, vertexBasDroit);

    FacadePortability::takeMutex(mutex);
    listePointInit = LISTEPOINTSTATE_OK;
    emptyQueue();
}
#endif



////////////////////////////////////////////////////////////////////////
///
/// @fn int NoeudTable::expectedChildCount()
///
/// /*Description*/
///
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int NoeudTable::expectedChildCount()
{
    return NB_GROUP_TYPES;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudTable::NoeudTable(const std::string& typeNoeud)
///
/// Ce constructeur cree tout les points necessaire au jeu et assigne 
/// les valeurs de longueur et hauteur de la table.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudTable::NoeudTable(const std::string& typeNoeud)
   : NoeudComposite(RAZER_KEY_TABLE,typeNoeud) , mFrictionRatio(0.3f),mListRenderCorners(0)
{
    for(int i=0; i<NB_BONUS_TYPE; ++i)
    {
        mBonusProperties[i].type = BonusType(i);
        mBonusProperties[i].enabled = true;
        mBonusProperties[i].modified = false;
        mBonusProperties[i].duration = 10.f;
        mAvailableBonuses.push_back(mBonusProperties[i]);
    }

    /// les noeuds points ne peuvent etre supprimer
    mFlags.SetFlag(false,NODEFLAGS_CAN_BE_DELETED);

    for(int i=0; i<NB_GROUP_TYPES; ++i)
    {
        /// Groupe destine a contenir les noeud concret pour un meilleur parcours d'arbre
        NoeudGroupe* group = new NoeudGroupe(RazerGameUtilities::NOM_GROUPE,*GroupTypes[i]);
        add(group);
    }

    //GestionnaireModeles::obtenirInstance()->recharger(RazerGameUtilities::NOM_TABLE);
    //aidegl::glLoadTexture(RazerGameUtilities::NOM_DOSSIER+"table_hockey.png",textureId_,true);
    const float longueurTable = DEFAULT_SIZE[VX];
    const float hauteurTable = DEFAULT_SIZE[VY];
    setCanBeSelected(false);
    setSelection(false);


    /// Création des 8 points de la table
    NoeudPoint* hautGauche_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,-longueurTable/2,hauteurTable/2, POSITION_HAUT_GAUCHE);
    NoeudPoint* hautMilieu_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,0,hauteurTable/2, POSITION_HAUT_MILIEU);
    NoeudPoint* hautDroite_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,longueurTable/2,hauteurTable/2, POSITION_HAUT_DROITE);
    NoeudPoint* milieuGauche_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,-longueurTable/2,0, POSITION_MILIEU_GAUCHE);
    NoeudPoint* milieuDroite_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,longueurTable/2,0, POSITION_MILIEU_DROITE);
    NoeudPoint* basGauche_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,-longueurTable/2,-hauteurTable/2, POSITION_BAS_GAUCHE);
    NoeudPoint* basMilieu_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,0,-hauteurTable/2, POSITION_BAS_MILIEU);
    NoeudPoint* basDroite_= new NoeudPoint(RazerGameUtilities::NAME_TABLE_CONTROL_POINT,longueurTable/2,-hauteurTable/2, POSITION_BAS_DROITE);

    /// Ajout du point symétrique à chacun des points
    hautGauche_->modifierPointSym(hautDroite_);
    hautMilieu_->modifierPointSym(basMilieu_);
    hautDroite_->modifierPointSym(hautGauche_);
    milieuGauche_->modifierPointSym(milieuDroite_);
    milieuDroite_->modifierPointSym(milieuGauche_);
    basGauche_->modifierPointSym(basDroite_);
    basMilieu_->modifierPointSym(hautMilieu_);
    basDroite_->modifierPointSym(basGauche_);

    hautGauche_->  PositionSubject::attach(this);
    hautMilieu_->  PositionSubject::attach(this);
    hautDroite_->  PositionSubject::attach(this);
    milieuGauche_->PositionSubject::attach(this);
    milieuDroite_->PositionSubject::attach(this);
    basGauche_->   PositionSubject::attach(this);
    basMilieu_->   PositionSubject::attach(this);
    basDroite_->   PositionSubject::attach(this);

    /// Ajout dans le vecteur contenant les points pour la sauvegarde
    vecteurPoint_.push_back(hautGauche_);
    vecteurPoint_.push_back(hautMilieu_);
    vecteurPoint_.push_back(hautDroite_);
    vecteurPoint_.push_back(milieuGauche_);
    vecteurPoint_.push_back(milieuDroite_);
    vecteurPoint_.push_back(basGauche_);
    vecteurPoint_.push_back(basMilieu_);
    vecteurPoint_.push_back(basDroite_);

    /// Ajout de chacun des points comme enfant de la table
    add(hautGauche_);
    add(hautMilieu_);
    add(hautDroite_);
    add(milieuGauche_);
    add(milieuDroite_);
    add(basGauche_);
    add(basMilieu_);
    add(basDroite_);


    /// enregistrement des position des point associe entre eux
    droiteMuret_[0] = CouplePoint(POSITION_HAUT_GAUCHE,POSITION_HAUT_MILIEU);
    droiteMuret_[1] = CouplePoint(POSITION_HAUT_MILIEU,POSITION_HAUT_DROITE);
    droiteMuret_[2] = CouplePoint(POSITION_HAUT_DROITE,POSITION_MILIEU_DROITE);
    droiteMuret_[3] = CouplePoint(POSITION_MILIEU_DROITE,POSITION_BAS_DROITE);
    droiteMuret_[4] = CouplePoint(POSITION_BAS_DROITE,POSITION_BAS_MILIEU);
    droiteMuret_[5] = CouplePoint(POSITION_BAS_MILIEU,POSITION_BAS_GAUCHE);
    droiteMuret_[6] = CouplePoint(POSITION_BAS_GAUCHE,POSITION_MILIEU_GAUCHE);
    droiteMuret_[7] = CouplePoint(POSITION_MILIEU_GAUCHE,POSITION_HAUT_GAUCHE);

    
    // Allocation de l'espace mémoire pour les but et on donne les paramètre nécessaire à l'affichage
    butJoueur1_ = new NoeudBut(RazerGameUtilities::NOM_BUT,1,hautGauche_,basGauche_,milieuGauche_);
    butJoueur2_ = new NoeudBut(RazerGameUtilities::NOM_BUT,2,hautDroite_,basDroite_,milieuDroite_);
    butJoueur1_->modifierButAdverse(butJoueur2_);
    butJoueur2_->modifierButAdverse(butJoueur1_);


    //Ajout de mur relatif aux point et buts pour qu'il s'ajustent automatiquement à leur déplacement
    NodeRinkBoards      *mr1 = new NodeRinkBoards(hautGauche_,hautMilieu_),
                        *mr2 = new NodeRinkBoards(hautMilieu_,hautDroite_),
                        *mr3 = new NodeRinkBoards(basMilieu_,basGauche_),
                        *mr4 = new NodeRinkBoards(basDroite_,basMilieu_),
                        *mr5 = new NodeRinkBoards(hautDroite_,butJoueur2_,true),//ok
                        *mr6 = new NodeRinkBoards(butJoueur2_,basDroite_,false),
                        *mr7 = new NodeRinkBoards(basGauche_,butJoueur1_,false),//ok
                        *mr8 = new NodeRinkBoards(butJoueur1_,hautGauche_,true);
    add(mr1);
    add(mr2);
    add(mr3);
    add(mr4);
    add(mr5);
    add(mr6);
    add(mr7);
    add(mr8);
    bande_[0] = mr8;//mr1;
    bande_[1] = mr1;//mr2;
    bande_[2] = mr2;//mr3;
    bande_[3] = mr5;//mr4;
    bande_[4] = mr6;//mr5;
    bande_[5] = mr4;//mr6;
    bande_[6] = mr3;//mr7;
    bande_[7] = mr7;//mr8;

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudTable::~NoeudTable()
///
/// Ce destructeur repositionne le modele3D de la table avant d'en sortir.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudTable::~NoeudTable()
{
   replacerModele();
   if(mListRenderCorners)
   {
       glDeleteLists(*mListRenderCorners,nbCorners);
   }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::renderReal() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::renderReal() const
{
    
    // Appel à la version de la classe de base pour l'affichage des enfants.
    //NoeudComposite::renderReal();
    DrawChild();
    {
#if WIN32
        GLint renderMode;
        glGetIntegerv(GL_RENDER_MODE,&renderMode);
        if(renderMode == GL_SELECT)
        {
            // dont draw table when selecting
            return;
        }

        Modele3D* pModel = getModel();
        if(pModel)
        {
            // render corners
            if(mListRenderCorners)
            {
                glPushMatrix();
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                if(pModel->appliquerMateriau("GreenTeam"))
                {
                    glPushMatrix();
                    glCallList(mListRenderCorners[1]);
                    glPopMatrix();

                    glPushMatrix();
                    glCallList(mListRenderCorners[2]);
                    glPopMatrix();

                    // car la fonction appliquer materiau fait un push matrix dans le mode Texture
                    glMatrixMode(GL_TEXTURE);
                    glPopMatrix();
                    glMatrixMode(GL_MODELVIEW);
                }
                if(pModel->appliquerMateriau("RedTeam"))
                {
                    glPushMatrix();
                    glCallList(mListRenderCorners[0]);
                    glPopMatrix();
                    glPushMatrix();
                    glCallList(mListRenderCorners[3]);
                    glPopMatrix();

                    // car la fonction appliquer materiau fait un push matrix dans le mode Texture
                    glMatrixMode(GL_TEXTURE);
                    glPopMatrix();
                    glMatrixMode(GL_MODELVIEW);
                }
                glPopAttrib();
                glPopMatrix();
            }

            glPushMatrix();
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            pModel->dessiner(true);
            glPopAttrib();
            glPopMatrix();
        }

        glColor4f(1.0f,0.0f,1.0f,1.0f);

        // États de la lumière 
        GLboolean lighting_state;
        // Désactiver l'éclairage
        glGetBooleanv(GL_LIGHTING, &lighting_state);
        glDisable(GL_LIGHTING);

        FacadeModele::getInstance()->DeActivateShaders();
        {
            static const float moitieLargeurLigne = 1.0f;
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            glColor3f(0.0,0.0,1.0);
            glBegin(GL_QUADS);

            // dessin des lignes verticals dans la table
            for(int i=1; i<NB_HORIZONTAL_VERTICES-1; ++i)
            {
                for(int j=0; j<NB_VERTICAL_VERTICES-1; ++j)
                {
                    // Dessin des lignes verticales de la table
                    const Vecteur3& cur = mTableVertices[i][j];
                    const Vecteur3& down = mTableVertices[i][j+1];
                    glVertex3f(cur[VX]+moitieLargeurLigne,cur[VY],Z_HEIGHT_TABLE_SURFACE+1);
                    glVertex3f(cur[VX]-moitieLargeurLigne,cur[VY],Z_HEIGHT_TABLE_SURFACE+1);
                    glVertex3f(down[VX]-moitieLargeurLigne,down[VY],Z_HEIGHT_TABLE_SURFACE+1);
                    glVertex3f(down[VX]+moitieLargeurLigne,down[VY],Z_HEIGHT_TABLE_SURFACE+1);
                }
            }

            glEnd();
            glPopMatrix();
            GLUquadric* cercleCentre_ = gluNewQuadric();
            GLUquadric* centre_ = gluNewQuadric();
            // Dessin du cercle au centre de la table

            glPushMatrix();
            glColor3f(1.0,0.0,0.0);
            gluDisk(cercleCentre_,rayonCercleCentre_-1,rayonCercleCentre_+1,32,32);
            gluDisk(centre_,0,2,32,32);
            glPopMatrix();

            glPopAttrib();
            gluDeleteQuadric(centre_);
            gluDeleteQuadric(cercleCentre_);
        }
        FacadeModele::getInstance()->ActivateShaders();

        // Réactiver l'éclairage et (s'il y a lieu)
        if (lighting_state == GL_TRUE) {
            glEnable(GL_LIGHTING);
        }
#else
    renderOpenGLES();
#endif
    }

    
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::renderOpenGLES()
///
/// Fonction appeler dans afficher concret pour faire le
/// rendu OpenGL, uniquement utilisé sous APPLE.
/// utiliser les liste d'affichage pour windows
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::renderOpenGLES() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    // dessin de la table
    {
        const int nbVertices = (NB_HORIZONTAL_VERTICES*2+(NB_VERTICAL_VERTICES-2)*2)+2;
        GLfloat vertices[nbVertices*3];
        int count = 0;

        // Le point du centre
        vertices[count++] = 0;
        vertices[count++] = 0;
        vertices[count++] = -1;
        
        // La partie de gauche
        for(int i=1;i<NB_VERTICAL_VERTICES-1; ++i)
        {
            vertices[count++] = mTableVertices[0][i][VX];
            vertices[count++] = mTableVertices[0][i][VY];
            vertices[count++] = -1;
        }
        // La partie du bas
        for(int i=0; i<NB_HORIZONTAL_VERTICES; ++i)
        {
            vertices[count++] = mTableVertices[i][NB_VERTICAL_VERTICES-1][VX];
            vertices[count++] = mTableVertices[i][NB_VERTICAL_VERTICES-1][VY];
            vertices[count++] = -1;
        }
        // La partie de droite
        for(int i=NB_VERTICAL_VERTICES-2; i>0; --i)
        {
            vertices[count++] = mTableVertices[NB_HORIZONTAL_VERTICES-1][i][VX];
            vertices[count++] = mTableVertices[NB_HORIZONTAL_VERTICES-1][i][VY];
            vertices[count++] = -1;
        }

        // La partie du haut
        for(int i=NB_HORIZONTAL_VERTICES-1; i>=0; --i)
        {
            vertices[count++] = mTableVertices[i][0][VX];
            vertices[count++] = mTableVertices[i][0][VY];
            vertices[count++] = -1;
        }
        
        // On remet le point gauche pour le triangle fan
        vertices[count++] = mTableVertices[0][1][VX];
        vertices[count++] = mTableVertices[0][1][VY];
        vertices[count++] = -1;

        glColor4f(1.0f,1.0f,1.0f,1.0f);
        glVertexPointer (3, GL_FLOAT , 0, vertices); 
        glDrawArrays (GL_TRIANGLE_FAN, 0, nbVertices);
    }

    // dessin des lignes verticales
    {
        const float moitieLargeurLigne = 1.0f;
        const float hauteurLigne = -0.5f;
        // 
        const int nbVertices = 4;
        GLfloat vertices[nbVertices*3];

        for(int i=1; i<NB_HORIZONTAL_VERTICES-1; ++i)
        {
            for(int j=0; j<NB_VERTICAL_VERTICES-1; ++j)
            {
                // Dessin des lignes verticales de la table
                const Vecteur3& cur = mTableVertices[i][j];
                const Vecteur3& down = mTableVertices[i][j+1];

                vertices[0] = cur[VX]+moitieLargeurLigne;
                vertices[1] = cur[VY];
                vertices[2] = hauteurLigne;
                vertices[3] = cur[VX]-moitieLargeurLigne;
                vertices[4] = cur[VY];
                vertices[5] = hauteurLigne;
                vertices[6] = down[VX]-moitieLargeurLigne;
                vertices[7] = down[VY];
                vertices[8] = hauteurLigne;
                vertices[9] = down[VX]+moitieLargeurLigne;
                vertices[10] = down[VY];
                vertices[11] = hauteurLigne;

                glColor4f(0.0f,0.0f,1.0f,1.0f);
                glVertexPointer (3, GL_FLOAT , 0, vertices); 
                glDrawArrays (GL_TRIANGLE_FAN, 0, nbVertices);
            }
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::tick( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::tick( const float& temps)
{
   // Appel à la version de la classe de base pour l'animation des enfants.
   NoeudComposite::tick(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudTable::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNoeudTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudTable::add( NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::add( NoeudAbstrait* enfant )
{
    // si un groupe existe pour ce type de noeud on lui assigne
    NoeudGroupe* g = obtenirGroupe(enfant->getType());
    if(g)
    {
        return g->add(enfant);
    }
    // sinon la table prend le noeud comme enfant normalement
    return NoeudComposite::add(enfant);
}



    ////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint* NoeudTable::obtenirPoint( int initiale )
///
/// Retourne un pointeur sur le point désigner par ses initiales, i.e. hautGauche_ = POSITION_HAUT_GAUCHE.
///
/// @param[in] int initiale : les initiales du nom du point.
///
/// @return NoeudPoint* : un pointeur sur le point.
///
////////////////////////////////////////////////////////////////////////
NoeudPoint* NoeudTable::obtenirPoint( int typePosPoint )
{
    return vecteurPoint_[typePosPoint];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait::PaireVect3 NoeudTable::obtenirZoneOccupee(  )
///
/// Retourne la position occupee par la table.
///
///
/// @return NoeudAbstrait::PaireVect3 : position min et max occuppee.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait::PaireVect3 NoeudTable::obtenirZoneOccupee() const
{
    /// A COMPLETER
    return PaireVect3();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudTable::getRadius(  )
///
/// Retourne le rayon du noeud.
///
///
/// @return float : rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
float NoeudTable::getRadius() const
{
    return 0;
    // A COMPLETER
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudBut* NoeudTable::obtenirBut( int joueur )
///
/// Accesseur des buts de la table.
///
/// @param[in] int joueur : le numero du joueur: 1 pour le but de gauche, 2 pour le but de droite
///
/// @return NoeudBut* : un pointeur sur le but voulu.
///
////////////////////////////////////////////////////////////////////////
NoeudBut* NoeudTable::obtenirBut( int joueur ) const
{
    switch(joueur)
    {
    case 1: return butJoueur1_;
    case 2: return butJoueur2_;
    default : throw ExceptionJeu("numero [%d] de joueur invalide", joueur);
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::calculerHautLongMax( float* reponse )
///
/// Calcule et retourne la hauteur et longueur (half length)
/// maximale de la table, donc le rectangle englobant
///
/// @param[in] float * reponse : le pointeur ou mettre la reponse
/// La reponse est de la forme 0- hauteurMax, 1- longueurMax, dans l'ordre
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::calculerHautLongMax(float reponse[])
{
    // Puisque le table est symétrique par rapport a y seulement
    // On a besoin de 2 variable pour la hauteur puisqu'elle diffère
    
    // On trouve la hauteur max en haut
    float maxHauteurEntre2=max(obtenirPoint(POSITION_HAUT_GAUCHE)->getPosition()[VY],obtenirPoint(POSITION_HAUT_MILIEU)->getPosition()[VY]);
    float hauteurYHaut=max(maxHauteurEntre2,obtenirPoint(POSITION_HAUT_DROITE)->getPosition()[VY]); 

    // On trouve la hauteur max en bas
    maxHauteurEntre2=min(obtenirPoint(POSITION_BAS_GAUCHE)->getPosition()[VY],obtenirPoint(POSITION_BAS_MILIEU)->getPosition()[VY]);
    float hauteurYBas=-1*min(maxHauteurEntre2,obtenirPoint(POSITION_BAS_DROITE)->getPosition()[VY]);

    // On compare la hauteur max en haut et en bas
    float HauteurY=max(hauteurYHaut,hauteurYBas);
    reponse[0]=HauteurY;

    // On trouve la longueur max, on le fait sur le cote droit seulement puisqua la table est symetrique en Y
    float maxLongueurEntre2=max(obtenirPoint(POSITION_HAUT_DROITE)->getPosition()[VX],obtenirPoint(POSITION_MILIEU_DROITE)->getPosition()[VX]);
    float longueurX=max(maxLongueurEntre2,obtenirPoint(POSITION_BAS_DROITE)->getPosition()[VX]);
    reponse[1]=longueurX;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudTable::estSurTable( NoeudAbstrait* noeud )
///
/// Algorithme pour savoir si l'element est sur la table
///
/// @param[in] NoeudAbstrait * noeud : noeud que l'on veut savoir s'il est sur la table
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::estSurTable(NoeudAbstrait* noeud)
{
    if(noeud == 0)
        return false;

    Vecteur2 posNoeud = noeud->getPosition().convertir<2>();
    return estSurTable(posNoeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeWallAbstract* NoeudTable::detectionCollisionGrandeVitesseMuret( Vecteur2 anciennePos, Vecteur2 nouvellePos )
///
/// Permet de detecter si la rondelle entre en collision avec les muret exterieur malgre la grande vitesse
///
/// @param[in] Vecteur2 anciennePos : ancienne position de la rondelle avant son deplacement
/// @param[in] Vecteur2 nouvellePos : nouvelle position de la rondelle apres son deplacement
///
/// @return Vecteur2 retourne le vecteur directeur du muret en collision
///                 vecteur nul si aucune intersection
///
////////////////////////////////////////////////////////////////////////
NodeWallAbstract* NoeudTable::detectionCollisionGrandeVitesseMuret( const Vecteur2& anciennePos,const Vecteur2& nouvellePos, Vecteur2 &intersectionRetour )
{
    float distance = 9999999;
    NodeWallAbstract* retour = 0;
    NoeudGroupe* groupe = obtenirGroupe(RazerGameUtilities::NOM_MURET);
    if(groupe)
    {
        for(unsigned int i=0; i<groupe->childCount(); ++i)
        {
            NodeWallAbstract* muret = dynamic_cast<NodeWallAbstract*>(groupe->find(i));
            if(muret)
            {
                Vecteur2 point1 = muret->obtenirCoin1().convertir<2>();
                Vecteur2 point2 = muret->obtenirCoin2().convertir<2>();
                Vecteur2 directeur(point2-point1);
                directeur.normaliser();
                directeur*=1.05f;
                point2 += directeur;
                point1 -= directeur;
                Vecteur2 intersection;
                if(aidecollision::calculerCollisionSegmentSegment(anciennePos,nouvellePos,point1,point2,intersection).type != aidecollision::COLLISION_AUCUNE )
                {
                    float distCur = (intersection-anciennePos).norme2();
                    if(distCur <= distance)
                    {
                        distance = distCur;
                        intersectionRetour = intersection;
                        retour = muret;
                    }
                }
            }
        }
    }
    return retour;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudTable::estSurTable( Vecteur2i position )
///
/// Algorithme pour savoir si la position de l'element est sur la table
///
/// @param[in] Vecteur2i position : position d'un element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::estSurTable(Vecteur2 position)
{
    // Le noeud est a la position 0 alors il est certainement sur la table
    if(position.estNul())
        return true;

#if BOX2D_INTEGRATED  
    b2RayCastInput input;
    input.p1 = b2Vec2(0,0);
    utilitaire::VEC3_TO_B2VEC(position,input.p2);
    input.maxFraction = 1;


    const int nbNodes = NB_BANDES+2;
    NoeudAbstrait* nodes[nbNodes];
    memcpy(nodes,bande_,NB_BANDES*sizeof(NoeudAbstrait*));
    nodes[nbNodes-2] = butJoueur1_;
    nodes[nbNodes-1] = butJoueur2_;

    for(unsigned int i=0; i<nbNodes; ++i)
    {
        auto node = nodes[i];
        checkf(node);
        if(node)
        {
            auto body = node->getPhysicBody();
            //checkf(body);
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
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;


#else
    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        Vecteur2 point1 = obtenirPoint(droiteMuret_[i].first)->getPosition().convertir<2>();
        Vecteur2 point2 = obtenirPoint(droiteMuret_[i].second)->getPosition().convertir<2>();
        Vecteur2 intersection;
        if(aidecollision::calculerCollisionSegmentSegment(Vecteur2(),position,point1,point2,intersection).type != aidecollision::COLLISION_AUCUNE )
        {
            if(position.norme2() < intersection.norme2())
                return true;
            return false;
        }
    }
    return true;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudTable::createXmlNode()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudTable::createXmlNode()
{
    XmlElement* elementNoeud = NoeudComposite::createXmlNode();
    
    XMLUtils::writeAttribute<float>(elementNoeud,"coefFriction",mFrictionRatio);
    
    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        std::ostringstream name;
        name << "rebondBande";
        name << i;
        float coef = bande_[i]->getReboundRatio();
        XMLUtils::writeAttribute<float>(elementNoeud,name.str().c_str(),coef);
    }

    for(int type=0; type<NB_BONUS_TYPE; ++type)
    {
        if(mBonusProperties[type].modified)
        {
            XmlElement* bonusProp = XMLUtils::createNode(BonusXmlTag.c_str());
            XMLUtils::writeAttribute(bonusProp,"type", type);
            XMLUtils::writeAttribute(bonusProp,"en", mBonusProperties[type].enabled);
            XMLUtils::writeAttribute(bonusProp,"duration", mBonusProperties[type].duration);
            XMLUtils::LinkEndChild(elementNoeud,bonusProp);
        }
    }

    return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudTable::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudTable à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::initFromXml( const XmlElement* element )
{
    auto field = getField();
    if(field)
    {
        field->setTable(this);
    }


    if(!Super::initFromXml(element))
        return false;
    if(!XMLUtils::readAttribute(element,"coefFriction",mFrictionRatio))
        throw ExceptionJeu("Error reading table's fricition coefficient");

    // On assigne le coefficient de rebon des bandes exterieurs
    float rebond[NB_BANDES];
    if(!XMLUtils::readArray(rebond,NB_BANDES,element,"rebondBande"))
        throw ExceptionJeu("Error reading rink boards rebound coefficient");

    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        modifierCoefRebond(i,rebond[i]);
    }

    int controlPointVisited = 0;

    const ArbreRendu* treeRoot = GetTreeRoot();
    if(treeRoot)
    {
        for( auto child = XMLUtils::FirstChildElement(element); child; child = XMLUtils::NextSibling(child) )
        {
            auto name = XMLUtils::GetNodeTag(child);
            if(name == RazerGameUtilities::NAME_TABLE_CONTROL_POINT)
            {
                int typeNoeud;
                if( !XMLUtils::readAttribute(child,"typePosNoeud", typeNoeud) || ((unsigned int)typeNoeud) >= NB_CONTROL_POINTS )
                {
                    throw ExceptionJeu("Erreur de lecture d'attribut");
                }
                obtenirPoint(typeNoeud)->initFromXml(child);
                controlPointVisited |= 1<<typeNoeud;
            }
            else if(name == RazerGameUtilities::NOM_GROUPE)
            {
                // compatibilité ancienne version
                for( auto groupChild = XMLUtils::FirstChildElement(child); groupChild; groupChild = XMLUtils::NextSibling(groupChild) )
                {
                    CreateAndInitNodesFromXml(groupChild);
                }
            }
            else if(name == RazerGameUtilities::NAME_CONTROL_POINT)
            {
                checkf(0,"invalid position for a control point, ignoring node");
            }
            else if(name == BonusXmlTag)
            {
                int type;
                bool enable;
                float duration;
                if( !XMLUtils::readAttribute(child,"type", type) || type >= NB_BONUS_TYPE )
                {
                    continue;
                }
                if( !XMLUtils::readAttribute(child,"en", enable))
                {
                    continue;
                }
                if( !XMLUtils::readAttribute(child,"duration", duration))
                {
                    continue;
                }
                mBonusProperties[type].enabled = enable;
                mBonusProperties[type].modified = true;
                mBonusProperties[type].duration = duration;
            }
            else
            {
                CreateAndInitNodesFromXml(child);
            }
        }
    }
    else
    {
        throw ExceptionJeu("%s : Missing tree root",mType.c_str());
    }

    if(controlPointVisited != (1<<NB_CONTROL_POINTS)-1 )
    {
        throw ExceptionJeu("%s : control points missing in the file",mType.c_str());
    }

    const float l1 = butJoueur1_->getLength(), l2 =  butJoueur2_->getLength();
    if( l1 != l2 )
    {
        if(l1>l2)
        {
            butJoueur1_->updateLongueur(l2/l1);
        }
        else
        {
            butJoueur2_->updateLongueur(l1/l2);
        }
    }

    // refresh available bonuses
    mAvailableBonuses.clear();
    for(int i=0; i<NB_BONUS_TYPE; ++i)
    {
        if(mBonusProperties[i].enabled)
        {
            mAvailableBonuses.push_back(mBonusProperties[i]);
        }
    }


    return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudGroupe* NoeudTable::obtenirGroupe( std::string typeEnfant )
///
/// accesseur des noeuds groupes de la table
///
/// @param[in] std::string typeEnfant
///
/// @return NoeudGroupe* le groupe contenant ce type d'enfants, 0 si la table n'a pas de groupe de ce type
///
////////////////////////////////////////////////////////////////////////
NoeudGroupe* NoeudTable::obtenirGroupe(std::string typeEnfant)
{
    unsigned int nbEnfants = childCount();
    for(unsigned int i=0; i<nbEnfants; i++)
    {
        NoeudGroupe *g = dynamic_cast<NoeudGroupe *>(find(i));
        if (g)
        {
            if(g->obtenirTypeEnfants() == typeEnfant)
                return g;
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudGroupe* NoeudTable::obtenirGroupe( unsigned int typeIdEnfant )
///
/// /*Description*/
///
/// @param[in] unsigned int typeIdEnfant
///
/// @return NoeudGroupe*
///
////////////////////////////////////////////////////////////////////////
NoeudGroupe* NoeudTable::obtenirGroupe( unsigned int typeIdEnfant )
{
#if WIN32
    return obtenirGroupe(GestionnaireModeles::obtenirInstance()->obtenirNameFromTypeId(typeIdEnfant));
#endif
    return NULL;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::reassignerParentBandeExt(  )
///
/// Ajoute les bande exterieur comme enfant au groupe de Muret pour les traiter pareil
/// Les murets relatif doivent etre creer dans le constructeur de talbes puisqu'ils doivent connaitre 
/// l'adresse memoire des points (le probleme reside dans le chargement XML, on ne peut savoir a quoi relie les
/// muret relatifs a leur chargement), 
/// Pour que les bandes exterieur soient traiter de la meme facon que les
///
/// Ceci DOIT etre appeler apres l'initialisation de l'arbre ET apres le chargement en XML
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::reassignerParentBandeExt()
{
    NoeudGroupe *groupe = dynamic_cast<NoeudGroupe *>(obtenirGroupe(RazerGameUtilities::NOM_MURET));
    if (groupe)
    {
        for(unsigned int i=0; i<NB_BANDES; ++i)
        {
            //la table va perdre ce muret comme enfants et le groupe le recevra.
            // La bande aura maintenant le groupe comme parent.
            groupe->add(bande_[i]);
        }
    }
    else
    {
        utilitaire::afficherErreur("La table n'a pas de groupe pour les murets, on ne peut reassigner les bandes exterieures");
    }
}
////////////////////////////////////////////////////////////////////////
///
/// @fn GroupeTripleIndex NoeudTable::trouverVertex( const aiScene* scene, const aiNode* noeud ) 
///
/// Retourne la liste de vertex correspondant aux coordonnees envoyees
///
/// @param[in] const aiScene*   scene       : la scene ou est contenue le modele 3D
/// @param[in] const aiNode*    noeud       : le noeud(aiNode) a partir duquel faire la recherche traiter
/// @param[in] GroupeCoord      listePoints : liste des points a trouver
///
/// @return GroupeTripleIndex : groupe d'index correspondant a un des points envoyes en parametre
///
////////////////////////////////////////////////////////////////////////
GroupeTripleAdresseFloat NoeudTable::trouverVertex( const aiScene* scene, const aiNode* noeud, const GroupeCoord& listePoints )
{
    
    GroupeTripleAdresseFloat retour; // Liste a retourner
#if WIN32
    for (unsigned int i=0; i<noeud->mNumMeshes; i++)
    {
        const aiMesh* mesh = scene->mMeshes[noeud->mMeshes[i]];
        for (unsigned int j=0; j<mesh->mNumFaces; j++)
        {
            const aiFace* face = &mesh->mFaces[j];
            for (unsigned int k=0; k<face->mNumIndices; k++)
            {
                int indexVertex = face->mIndices[k];
                Vecteur3i positionVertex((int)floorf(mesh->mVertices[indexVertex].x+0.5f), (int)floorf(mesh->mVertices[indexVertex].y+0.5f), (int)floorf(mesh->mVertices[indexVertex].z+0.5f));
                for(int l=0; l<listePoints.size(); l++)
                {
                    // 25 because 5^2 and giving a tolerence of 5 pixel to find the right vertex
                    unsigned int diff = (listePoints[l]-positionVertex).norme2()+25;
                    if(diff < 50)
                    {
                        retour.insert(Vecteur3pf(&(mesh->mVertices[indexVertex].x), &(mesh->mVertices[indexVertex].y), &(mesh->mVertices[indexVertex].z)));
                    }
                }
            }
        }
    }

    // Ajoute les enfants du noeud courant a la liste
    for (unsigned int i=0; i<noeud->mNumChildren; i++)
    {
        GroupeTripleAdresseFloat ajout = trouverVertex(scene, noeud->mChildren[i], listePoints);
        for(unsigned int p=0; p<ajout.size(); p++)
        {
            retour.push_back(ajout[p]);
        }
        
    }
#endif
    
    return retour;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::replacerModele(  )
///
/// Remodifie le modele selon le deplacement des points
///
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::replacerModele()
{
//     for(int i=0; i<vecteurPoint_.size(); i++)
//     {
//         Vecteur3 deplacement(vecteurPoint_[i]->obtenirPositionInitiale()-vecteurPoint_[i]->getPosition());
//         const GroupeTripleAdresseFloat* liste = vecteurPoint_[i]->obtenirListePointsAChanger();
//         if(liste)
//         {
//             for(unsigned int j=0; j<liste->size(); j++)
//             {
//                 *(liste->get(j)[VX]) += (float)deplacement[VX];
//                 *(liste->get(j)[VY]) += (float)deplacement[VY];
//             }
//         }
//     }


}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::updatePhysicBody()
{
    /// Mise a jour de la cache des vertex
    
    static const TypePosPoint nodes[3][3] = 
    {
        {POSITION_HAUT_GAUCHE,POSITION_HAUT_MILIEU,POSITION_HAUT_DROITE},
        {POSITION_MILIEU_GAUCHE,TypePosPoint(666),POSITION_MILIEU_DROITE},
        {POSITION_BAS_GAUCHE,POSITION_BAS_MILIEU,POSITION_BAS_DROITE}
    };

    // contour de la table
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            if(i==1 && j==1)
            {
                mTableVertices[nodeWidth[j]][nodeHeight[i]] = Vecteur3(0,0,0);
            }
            else
            {
                mTableVertices[nodeWidth[j]][nodeHeight[i]] = obtenirPoint(nodes[i][j])->getPosition();
            }
        }
    }


    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            // ligne intermediaire verticale
            if(i<2)
            {
                auto nbIntermediate = nodeWidth[i+1]-nodeWidth[i]-1;
                if(nbIntermediate != 0)
                {
                    auto w = mTableVertices[nodeWidth[i+1]][nodeHeight[j]] - mTableVertices[nodeWidth[i]][nodeHeight[j]];
                    w /= (float)nbIntermediate+1;
                    auto pos = mTableVertices[nodeWidth[i]][nodeHeight[j]];
                    for(int l=nodeWidth[i]+1; l<nodeWidth[i+1]; ++l)
                    {
                        pos += w;
                        mTableVertices[l][nodeHeight[j]] = pos;
                    }
                }
            }

            // ligne intermediaire horizontale, pas utilisé pour l'instant
//             if(j<3)
//             {
//                 auto nbIntermediate = nodeHeight[j+1]-nodeHeight[j]-1;
//                 if(nbIntermediate != 0)
//                 {
//                     auto w = mTableVertices[nodeWidth[i]][nodeHeight[j+1]] - mTableVertices[nodeWidth[i]][nodeHeight[j]];
//                     w /= (float)nbIntermediate+1;
//                     auto pos = mTableVertices[nodeWidth[i]][nodeHeight[j]];
//                     for(int l=nodeHeight[j]+1; l<nodeHeight[j+1]; ++l)
//                     {
//                         pos += w;
//                         mTableVertices[nodeWidth[i]][l] = pos;
//                     }
//                 }
//             }
        }
    }

#if BOX2D_INTEGRATED

    clearPhysicsBody();
    auto world = getWorld();
    if(world)
    {
        {
            auto pHaut = obtenirPoint(POSITION_HAUT_MILIEU), pBas = obtenirPoint(POSITION_BAS_MILIEU);
            if(pHaut && pBas)
            {
                b2BodyDef myBodyDef;
                myBodyDef.type = b2_staticBody; //this will be a dynamic body
                myBodyDef.position.Set(0, 0); //set the starting position
                myBodyDef.angle = 0; //set the starting angle

                mPhysicBody = world->CreateBody(&myBodyDef);

                b2Vec2 topPosB2,bottomPosB2 ;
                utilitaire::VEC3_TO_B2VEC(pHaut->getPosition(),topPosB2);
                utilitaire::VEC3_TO_B2VEC(pBas->getPosition(),bottomPosB2);
                b2EdgeShape shape;
                shape.Set(bottomPosB2,topPosB2);

                b2FixtureDef myFixtureDef;
                myFixtureDef.shape = &shape; //this is a pointer to the shapeHaut above
                myFixtureDef.density = 1;
                RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_TABLE,IsInGame());

                mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
                NoeudRondelle* puck = getField()->getPuck();
                if(puck)
                {
                    float offset = puck->getRadius()*utilitaire::ratioWorldToBox2D*2;
                    topPosB2.x -= offset;
                    bottomPosB2.x -= offset;

                    myFixtureDef.filter.categoryBits = CATEGORY_MIDLANE;
                    myFixtureDef.filter.maskBits = CATEGORY_PUCK;
                    myFixtureDef.isSensor = true;

                    shape.Set(bottomPosB2,topPosB2);
                    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body

                    topPosB2.x += offset;
                    bottomPosB2.x += offset;
                    topPosB2.x += offset;
                    bottomPosB2.x += offset;

                    shape.Set(bottomPosB2,topPosB2);
                    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
                }
                mPhysicBody->SetUserData(this);
            }
        }


        // First position : starting angle
        // second position : ending angle
        // last position : corner's position
        // will start the modeling from the first position passing by the corner and ending at the second position
        Vecteur2 Corners[][3] =
        {
            // haut droite
            {
                obtenirBut(2)->obtenirPositionHaut(),
                obtenirPoint(POSITION_HAUT_MILIEU)->getPosition(),
                obtenirPoint(POSITION_HAUT_DROITE)->getPosition()
            },
            // haut gauche
            {
                obtenirPoint(POSITION_HAUT_MILIEU)->getPosition(),
                obtenirBut(1)->obtenirPositionHaut(),
                obtenirPoint(POSITION_HAUT_GAUCHE)->getPosition()
            },
            //bas gauche
            {
                obtenirBut(1)->obtenirPositionBas(),
                obtenirPoint(POSITION_BAS_MILIEU)->getPosition(),
                obtenirPoint(POSITION_BAS_GAUCHE)->getPosition()
            },
            //bas droite
            {
                obtenirPoint(POSITION_BAS_MILIEU)->getPosition(),
                obtenirBut(2)->obtenirPositionBas(),
                obtenirPoint(POSITION_BAS_DROITE)->getPosition()
            },
            
        };
        /// here assumes the array above is not empty

        if(mListRenderCorners)
        {
            glDeleteLists(*mListRenderCorners,nbCorners);
        }
        else
        {
            mListRenderCorners = new GLuint[nbCorners];
        }
        mListRenderCorners[0] = glGenLists(1);
        mListRenderCorners[1] = glGenLists(1);
        mListRenderCorners[2] = glGenLists(1);
        mListRenderCorners[3] = glGenLists(1);

        /// Create round corner for the table
        for(int i=0; i<nbCorners; ++i)
        {
            glNewList(mListRenderCorners[i],GL_COMPILE);
            Vecteur2 posMid1   = Corners[i][0];//pHautMilieu->getPosition();
            Vecteur2 posMid2   = Corners[i][1];//pMilieuGauche->getPosition();
            Vecteur2 posCorner = Corners[i][2];//pHautGauche->getPosition();

            /// choose appropriate starting point for the arc
            Vecteur2 dir1 = posMid1-posCorner;
            Vecteur2 dir2 = posMid2-posCorner;
            float minLength1 = dir1.norme();
            float minLength2 = dir2.norme();
            static float defaultLength = 30;
            float length = defaultLength;
            if(minLength1 < length)
            {
                length = minLength1;
            }
            if(minLength2 < length)
            {
                length = minLength2;
            }

            dir1.normaliser();
            dir2.normaliser();
            dir1 *= length;
            dir2 *= length;

            Vecteur2 startingPoint = posCorner+dir1;
            Vecteur2 endPoint = posCorner+dir2;
            dir1 = dir1.tournerPiSur2() + startingPoint;
            dir2 = dir2.tournerPiSur2() + endPoint;

            Vecteur2 intersection;
            if(aidecollision::calculerIntersection2Droites(startingPoint,dir1,endPoint,dir2,intersection))
            {
                // only draw circle when the interseciton is inside the table
                if(intersection.norme2() < posCorner.norme2())
                {
                    b2Vec2 firstPoint,lastPoint,mid;
                    utilitaire::VEC3_TO_B2VEC(intersection,mid);
                    // position of starting points as if the intersection was (0,0)
                    Vecteur2 p1Relative = startingPoint - intersection;
                    Vecteur2 p2Relative = endPoint - intersection ;
                    const float radius = p1Relative.norme()*utilitaire::ratioWorldToBox2D;
                    p1Relative.normaliser();
                    p2Relative.normaliser();
                    float startingAngle = atan2f(p1Relative.Y(),p1Relative.X());
                    float endAngle = atan2f(p2Relative.Y(),p2Relative.X());

                    // making sure the starting angle is smaller than the ending angle
                    while(endAngle < startingAngle)
                    {
                        endAngle += 2*3.1415926535f;
                    }

                    b2EdgeShape line;
                    static float nbLines = 10;
                    const float deltaAngle = (endAngle-startingAngle)/nbLines;
                    b2Vec2 p1;
                    b2Vec2 p2; 
                    utilitaire::VEC3_TO_B2VEC(startingPoint,p1);

                    b2FixtureDef myFixtureDef;
                    myFixtureDef.shape = &line; //this is a pointer to the shape above
                    myFixtureDef.density = 1;
                    myFixtureDef.restitution = 0.75f;  // TODO:: not hardcoded value, use surrounding boards to eval rebound ratio
                    RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_RINK_BOARD,IsInGame());
                    const float invRatio = 1.f/utilitaire::ratioWorldToBox2D;
                    glScalef(invRatio,invRatio,1);
                    glBegin(GL_QUAD_STRIP);

                    glVertex3f(p1.x,p1.y,Z_HEIGHT_TABLE_BOARDS);
                    glVertex3f(p1.x,p1.y,Z_HEIGHT_TABLE_SURFACE);
                    for( float curAngle = startingAngle; curAngle < endAngle; curAngle+=deltaAngle)
                    {
                        p2.x = (cos(curAngle)*radius);
                        p2.y = (sin(curAngle)*radius);
                        p2 += mid;
                        line.Set(p1,p2);

                        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
                        p1 = p2;

                        glVertex3f(p1.x,p1.y,Z_HEIGHT_TABLE_BOARDS);
                        glVertex3f(p1.x,p1.y,Z_HEIGHT_TABLE_SURFACE);
                    }
                    utilitaire::VEC3_TO_B2VEC(endPoint,p2);
                    line.Set(p1,p2);
                    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
                    glVertex3f(p2.x,p2.y,Z_HEIGHT_TABLE_BOARDS);
                    glVertex3f(p2.x,p2.y,Z_HEIGHT_TABLE_SURFACE);
                    glEnd();

                    glBegin(GL_TRIANGLE_FAN);
                    utilitaire::VEC3_TO_B2VEC(posCorner,p2);
                    // initial point for the triangle fan
                    glVertex3f(p2.x,p2.y,Z_HEIGHT_TABLE_BOARDS);

                    utilitaire::VEC3_TO_B2VEC(startingPoint,p1);
                    glVertex3f(p1.x,p1.y,Z_HEIGHT_TABLE_BOARDS);
                    for( float curAngle = startingAngle; curAngle < endAngle; curAngle+=deltaAngle)
                    {
                        p2.x = (cos(curAngle)*radius);
                        p2.y = (sin(curAngle)*radius);
                        p2 += mid;
                        glVertex3f(p2.x,p2.y,Z_HEIGHT_TABLE_BOARDS);
                    }
                    utilitaire::VEC3_TO_B2VEC(endPoint,p2);
                    glVertex3f(p2.x,p2.y,Z_HEIGHT_TABLE_BOARDS);

                    glEnd();
                }
            }
            glEndList();
        }
        

    }
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::updatePosition( class PositionSubject& pSubject )
///
/// update the table when a modification is done on either middle control points
///
/// @param[in] class PositionSubject & pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::updateObserver( const  PositionSubject* pSubject )
{
    if(!isWorldLocked())
    {
        updatePhysicBody();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float NoeudTable::GetWidth()
///
/// retrieves the max width of the table
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float NoeudTable::GetWidth() const
{
    BoundingBox aabb;
    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        NodeWallAbstract* wall = bande_[i];
        aabb += wall->obtenirCoin1();
        aabb += wall->obtenirCoin2();
    }
    if(aabb.IsValid)
    {
        return aabb.GetSize()[VX];
    }
    return DEFAULT_SIZE[VX];
}

BonusType NoeudTable::getRandomBonus() const
{
    if(mAvailableBonuses.empty())
    {
        return NB_BONUS_TYPE;
    }
    return mAvailableBonuses[rand()%mAvailableBonuses.size()].type;
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////