///////////////////////////////////////////////////////////////////////////////
/// @file NoeudTable.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "AideCollision.h"
#include "NoeudGroupe.h"

#include <math.h>
#include "VisiteurDeplacement.h"

#include "XMLUtils.h"
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
#include "ArbreRendu.h"

ListeIndexPoints NoeudTable::listeIndexPointsModeleTable_ = ListeIndexPoints();
const Vecteur3 NoeudTable::DEFAULT_SIZE = Vecteur3(300,150);
const float NoeudTable::rayonCercleCentre_ = 25;

const int nodeHeight[3] = {0,NoeudTable::NB_VERTICAL_VERTICES>>1,NoeudTable::NB_VERTICAL_VERTICES-1};
const int nodeWidth[3] = {0,NoeudTable::NB_HORIZONTAL_VERTICES>>1,NoeudTable::NB_HORIZONTAL_VERTICES-1};

CreateListDelegateImplementation(Table)
{
    NoeudTable::initialiserListeIndexPoints(pModel);
    // dont make a list for table
    return 0;
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
   : NoeudComposite(typeNoeud) , coefFriction_(5)
{
    

    //GestionnaireModeles::obtenirInstance()->recharger(RazerGameUtilities::NOM_TABLE);
    //aidegl::glLoadTexture(RazerGameUtilities::NOM_DOSSIER+"table_hockey.png",textureId_,true);
    const float longueurTable = DEFAULT_SIZE[VX];
    const float hauteurTable = DEFAULT_SIZE[VY];
    assignerEstSelectionnable(false);
    assignerSelection(false);


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

    hautGauche_->  attach(this);
    hautMilieu_->  attach(this);
    hautDroite_->  attach(this);
    milieuGauche_->attach(this);
    milieuDroite_->attach(this);
    basGauche_->   attach(this);
    basMilieu_->   attach(this);
    basDroite_->   attach(this);

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
    ajouter(hautGauche_);
    ajouter(hautMilieu_);
    ajouter(hautDroite_);
    ajouter(milieuGauche_);
    ajouter(milieuDroite_);
    ajouter(basGauche_);
    ajouter(basMilieu_);
    ajouter(basDroite_);


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
    ajouter(mr1);
    ajouter(mr2);
    ajouter(mr3);
    ajouter(mr4);
    ajouter(mr5);
    ajouter(mr6);
    ajouter(mr7);
    ajouter(mr8);
    bande_[0] = mr8;//mr1;
    bande_[1] = mr1;//mr2;
    bande_[2] = mr2;//mr3;
    bande_[3] = mr5;//mr4;
    bande_[4] = mr6;//mr5;
    bande_[5] = mr4;//mr6;
    bande_[6] = mr3;//mr7;
    bande_[7] = mr7;//mr8;

    updatePhysicBody();
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
    if(!modele)
        return;
#if WIN32
    const aiScene* scene = modele->obtenirScene();
    const aiNode* rootNode = scene->mRootNode;

    GroupeCoord vertexBasMilieu;
    vertexBasMilieu.push_back(Vecteur3i(0, -75, -1));
    vertexBasMilieu.push_back(Vecteur3i(0, -75, 16));
    vertexBasMilieu.push_back(Vecteur3i(0, -82, 16));
    vertexBasMilieu.push_back(Vecteur3i(0, -82, 11));
    vertexBasMilieu.push_back(Vecteur3i(0, -102, 11));
    vertexBasMilieu.push_back(Vecteur3i(0, -93, -73));

    GroupeCoord vertexHautMilieu;
    vertexHautMilieu.push_back(Vecteur3i(0, 75, -1));
    vertexHautMilieu.push_back(Vecteur3i(0, 75, 16));
    vertexHautMilieu.push_back(Vecteur3i(0, 82, 16));
    vertexHautMilieu.push_back(Vecteur3i(0, 82, 11));
    vertexHautMilieu.push_back(Vecteur3i(0, 102, 11));
    vertexHautMilieu.push_back(Vecteur3i(0, 92, -73));

    GroupeCoord vertexMilieuGauche;
    vertexMilieuGauche.push_back(Vecteur3i(-173, 0, 11));
    vertexMilieuGauche.push_back(Vecteur3i(-156, 0, 11));
    vertexMilieuGauche.push_back(Vecteur3i(-156, 0, 16));
    vertexMilieuGauche.push_back(Vecteur3i(-150, 0, 16));
    vertexMilieuGauche.push_back(Vecteur3i(-150, 0, -1));
    vertexMilieuGauche.push_back(Vecteur3i(-156, 0, -73));

    GroupeCoord vertexMilieuDroit;
    vertexMilieuDroit.push_back(Vecteur3i(173, 0, 11));
    vertexMilieuDroit.push_back(Vecteur3i(156, 0, 11));
    vertexMilieuDroit.push_back(Vecteur3i(156, 0, 16));
    vertexMilieuDroit.push_back(Vecteur3i(150, 0, 16));
    vertexMilieuDroit.push_back(Vecteur3i(150, 0, -1));
    vertexMilieuDroit.push_back(Vecteur3i(157, 0, -73));

    GroupeCoord vertexHautGauche;
    vertexHautGauche.push_back(Vecteur3i(-173, 102, 11));
    vertexHautGauche.push_back(Vecteur3i(-156, 82, 11));
    vertexHautGauche.push_back(Vecteur3i(-156, 82, 16));
    vertexHautGauche.push_back(Vecteur3i(-150, 75, 16));
    vertexHautGauche.push_back(Vecteur3i(-150, 75, -1));
    vertexHautGauche.push_back(Vecteur3i(-156, 92, -73));
    vertexHautGauche.push_back(Vecteur3i(-110, 65, -73));
    vertexHautGauche.push_back(Vecteur3i(-110, 31, -73));
    vertexHautGauche.push_back(Vecteur3i(-81, 65, -73));
    vertexHautGauche.push_back(Vecteur3i(-81, 31, -73));
    vertexHautGauche.push_back(Vecteur3i(-95, 48, -133));
    vertexHautGauche.push_back(Vecteur3i(-124, 48, -133));
    vertexHautGauche.push_back(Vecteur3i(-124, 82, -133));
    vertexHautGauche.push_back(Vecteur3i(-95, 82, -133));

    GroupeCoord vertexBasGauche;
    vertexBasGauche.push_back(Vecteur3i(-173, -102, 11));
    vertexBasGauche.push_back(Vecteur3i(-156, -82, 11));
    vertexBasGauche.push_back(Vecteur3i(-156, -82, 16));
    vertexBasGauche.push_back(Vecteur3i(-150, -75, 16));
    vertexBasGauche.push_back(Vecteur3i(-150, -75, -1));
    vertexBasGauche.push_back(Vecteur3i(-156, -93, -73));
    vertexBasGauche.push_back(Vecteur3i(-110, -65, -73));
    vertexBasGauche.push_back(Vecteur3i(-110, -31, -73));
    vertexBasGauche.push_back(Vecteur3i(-81, -65, -73));
    vertexBasGauche.push_back(Vecteur3i(-81, -31, -73));
    vertexBasGauche.push_back(Vecteur3i(-95, -48, -133));
    vertexBasGauche.push_back(Vecteur3i(-124, -48, -133));
    vertexBasGauche.push_back(Vecteur3i(-124, -82, -133));
    vertexBasGauche.push_back(Vecteur3i(-95, -82, -133));

    GroupeCoord vertexHautDroit;
    vertexHautDroit.push_back(Vecteur3i(173, 102, 11));
    vertexHautDroit.push_back(Vecteur3i(156, 82, 11));
    vertexHautDroit.push_back(Vecteur3i(156, 82, 16));
    vertexHautDroit.push_back(Vecteur3i(150, 75, 16));
    vertexHautDroit.push_back(Vecteur3i(150, 75, -1));
    vertexHautDroit.push_back(Vecteur3i(157, 92, -73));
    vertexHautDroit.push_back(Vecteur3i(110, 65, -73));
    vertexHautDroit.push_back(Vecteur3i(110, 31, -73));
    vertexHautDroit.push_back(Vecteur3i(82, 65, -73));
    vertexHautDroit.push_back(Vecteur3i(82, 31, -73));
    vertexHautDroit.push_back(Vecteur3i(96, 48, -133));
    vertexHautDroit.push_back(Vecteur3i(125, 48, -133));
    vertexHautDroit.push_back(Vecteur3i(125, 82, -133));
    vertexHautDroit.push_back(Vecteur3i(96, 82, -133));

    GroupeCoord vertexBasDroit;
    vertexBasDroit.push_back(Vecteur3i(173, -102, 11));
    vertexBasDroit.push_back(Vecteur3i(156, -82, 11));
    vertexBasDroit.push_back(Vecteur3i(156, -82, 16));
    vertexBasDroit.push_back(Vecteur3i(150, -75, 16));
    vertexBasDroit.push_back(Vecteur3i(150, -75, -1));
    vertexBasDroit.push_back(Vecteur3i(157, -93, -73));
    vertexBasDroit.push_back(Vecteur3i(110, -65, -73));
    vertexBasDroit.push_back(Vecteur3i(110, -31, -73));
    vertexBasDroit.push_back(Vecteur3i(82, -65, -73));
    vertexBasDroit.push_back(Vecteur3i(82, -31, -73));
    vertexBasDroit.push_back(Vecteur3i(96, -48, -133));
    vertexBasDroit.push_back(Vecteur3i(125, -48, -133));
    vertexBasDroit.push_back(Vecteur3i(125, -82, -133));
    vertexBasDroit.push_back(Vecteur3i(96, -82, -133));

    listeIndexPointsModeleTable_[POSITION_BAS_MILIEU] = trouverVertex(scene, rootNode, vertexBasMilieu);
    listeIndexPointsModeleTable_[POSITION_HAUT_MILIEU]  = trouverVertex(scene, rootNode, vertexHautMilieu);
    listeIndexPointsModeleTable_[POSITION_MILIEU_GAUCHE]  = trouverVertex(scene, rootNode, vertexMilieuGauche);
    listeIndexPointsModeleTable_[POSITION_MILIEU_DROITE]  = trouverVertex(scene, rootNode, vertexMilieuDroit);
    listeIndexPointsModeleTable_[POSITION_HAUT_GAUCHE] = trouverVertex(scene, rootNode, vertexHautGauche);
    listeIndexPointsModeleTable_[POSITION_BAS_GAUCHE]  = trouverVertex(scene, rootNode, vertexBasGauche);
    listeIndexPointsModeleTable_[POSITION_HAUT_DROITE]  = trouverVertex(scene, rootNode, vertexHautDroit);
    listeIndexPointsModeleTable_[POSITION_BAS_DROITE]  = trouverVertex(scene, rootNode, vertexBasDroit);
#endif
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::afficherConcret() const
{
    
    // Appel à la version de la classe de base pour l'affichage des enfants.
    //NoeudComposite::afficherConcret();
    DrawChild();
    {
#if WIN32
        Modele3D* pModel = obtenirModele();
        if(pModel)
        {
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
            static const float hauteurLigne = 0.f;
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
                    glVertex3f(cur[VX]+moitieLargeurLigne,cur[VY],hauteurLigne);
                    glVertex3f(cur[VX]-moitieLargeurLigne,cur[VY],hauteurLigne);
                    glVertex3f(down[VX]-moitieLargeurLigne,down[VY],hauteurLigne);
                    glVertex3f(down[VX]+moitieLargeurLigne,down[VY],hauteurLigne);
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
    NoeudAbstrait::afficherConcret();
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
        const int nbVertices = (NB_HORIZONTAL_VERTICES*2+(NB_VERTICAL_VERTICES-2)*2);
        GLfloat vertices[nbVertices*3];
        int count = 0;


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
/// @fn void NoeudTable::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::animer( const float& temps)
{
   // Appel à la version de la classe de base pour l'animation des enfants.
   NoeudComposite::animer(temps);
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
/// @fn bool NoeudTable::ajouter( NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::ajouter( NoeudAbstrait* enfant )
{
    // si un groupe existe pour ce type de noeud on lui assigne
    NoeudGroupe* g = obtenirGroupe(enfant->obtenirType());
    if(g)
    {
        return g->ajouter(enfant);
    }
    // sinon la table prend le noeud comme enfant normalement
    return NoeudComposite::ajouter(enfant);
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
/// @fn float NoeudTable::obtenirRayon(  )
///
/// Retourne le rayon du noeud.
///
///
/// @return float : rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
float NoeudTable::obtenirRayon() const
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
/// Calcule et retourne la hauteur et longueur 
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

    // Le noeud est a la position 0 alors il est certainement sur la table
    if(posNoeud.estNul())
        return true;

    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        Vecteur2 point1 = obtenirPoint(droiteMuret_[i].first)->getPosition().convertir<2>();
        Vecteur2 point2 = obtenirPoint(droiteMuret_[i].second)->getPosition().convertir<2>();
        Vecteur2 intersection;
        if(aidecollision::calculerCollisionSegmentSegment(Vecteur2(),posNoeud,point1,point2,intersection).type != aidecollision::COLLISION_AUCUNE )
        {
            if(posNoeud.norme2() < intersection.norme2())
                    return true;
            return false;
        }
    }
    return true;
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
        for(unsigned int i=0; i<groupe->obtenirNombreEnfants(); ++i)
        {
            NodeWallAbstract* muret = dynamic_cast<NodeWallAbstract*>(groupe->chercher(i));
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
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudTable::creerNoeudXML()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudTable::creerNoeudXML()
{
    XmlElement* elementNoeud = NoeudComposite::creerNoeudXML();
    
    XMLUtils::writeAttribute<float>(elementNoeud,"coefFriction",coefFriction_);
    
    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        std::ostringstream name;
        name << "rebondBande";
        name << i;
        float coef = bande_[i]->getReboundRatio();
        XMLUtils::writeAttribute<float>(elementNoeud,name.str().c_str(),coef);
    }

    return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudTable::initialiser( const XmlElement* element )
///
/// Initialisation du NoeudTable à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::initialiser( const XmlElement* element )
{
    if(!Super::initialiser(element))
        return false;
    if(!XMLUtils::readAttribute(element,"coefFriction",coefFriction_))
        throw ExceptionJeu("Error reading table's fricition coefficient");

    // On assigne le coefficient de rebon des bandes exterieurs
    float rebond[NB_BANDES];
    if(!XMLUtils::readArray(rebond,NB_BANDES,element,"rebondBande"))
        throw ExceptionJeu("Error reading rink boards rebound coefficient");

    for(unsigned int i=0; i<NB_BANDES; ++i)
    {
        assignerCoefRebond(i,rebond[i]);
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
                obtenirPoint(typeNoeud)->initialiser(child);
                controlPointVisited |= 1<<typeNoeud;
            }
            else
            {
                CreateAndInitNodesFromXml(child);
            }
        }
    }
    else
    {
        throw ExceptionJeu("%s : Missing tree root",type_.c_str());
    }

    if(controlPointVisited != (1<<NB_CONTROL_POINTS)-1 )
    {
        throw ExceptionJeu("%s : control points missing in the file",type_.c_str());
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
    unsigned int nbEnfants = obtenirNombreEnfants();
    for(unsigned int i=0; i<nbEnfants; i++)
    {
        NoeudGroupe *g = dynamic_cast<NoeudGroupe *>(chercher(i));
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
            groupe->ajouter(bande_[i]);
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
                    if(positionVertex==listePoints[l])
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
    for(int i=0; i<vecteurPoint_.size(); i++)
    {
        Vecteur3 deplacement(vecteurPoint_[i]->obtenirPositionInitiale()-vecteurPoint_[i]->getPosition());
        const GroupeTripleAdresseFloat* liste = vecteurPoint_[i]->obtenirListePointsAChanger();
        if(liste)
        {
            for(unsigned int j=0; j<liste->size(); j++)
            {
                *(liste->get(j)[VX]) += (float)deplacement[VX];
                *(liste->get(j)[VY]) += (float)deplacement[VY];
            }
        }
    }


}
////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::assignerCoefRebond( int index, float coefRebond )
///
/// Assigne le coefficient de rebond a la bande de l'index voulu
///
/// @param[in] int index : l'index de la bande
/// @param[in] float coefRebond
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::assignerCoefRebond( int index, float coefRebond )
{
    bande_[index]->setReboundRatio(coefRebond);
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
            myFixtureDef.filter.categoryBits = CATEGORY_NONE;
            myFixtureDef.filter.maskBits = CATEGORY_NONE;
            myFixtureDef.filter.groupIndex = 1;

            mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
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
    updatePhysicBody();
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



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////