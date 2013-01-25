///////////////////////////////////////////////////////////////////////////////
/// @file NoeudTable.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudTable.h"
#include "aideGL.h"
#include "ArbreRenduINF2990.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "NoeudMuretRelatif.h"
#include "AideCollision.h"
#include "NoeudGroupe.h"
#include "GestionnaireModeles.h"
#include <math.h>
#include "VisiteurDeplacement.h"
#include <GL\glu.h>
#include "XMLUtils.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"

ListeIndexPoints NoeudTable::listeIndexPointsModeleTable_ = ListeIndexPoints();

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
   : NoeudComposite(typeNoeud) , coefFriction_(5), rayonCercleCentre_(25)
{
	

	//GestionnaireModeles::obtenirInstance()->recharger(ArbreRenduINF2990::NOM_TABLE);
	//aidegl::glLoadTexture(ArbreRenduINF2990::NOM_DOSSIER+"table_hockey.png",textureId_,true);
	float longueurTable = 300.;
	float hauteurTable = 150.;
	selectionnable_ = false;
	selectionne_ = false;

	/// Création des 8 points de la table
	NoeudPoint* hautGauche_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,-longueurTable/2,hauteurTable/2, POSITION_HAUT_GAUCHE);
	NoeudPoint* hautMilieu_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,0,hauteurTable/2, POSITION_HAUT_MILIEU);
	NoeudPoint* hautDroite_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,longueurTable/2,hauteurTable/2, POSITION_HAUT_DROITE);
	NoeudPoint* milieuGauche_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,-longueurTable/2,0, POSITION_MILIEU_GAUCHE);
	NoeudPoint* milieuDroite_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,longueurTable/2,0, POSITION_MILIEU_DROITE);
	NoeudPoint* basGauche_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,-longueurTable/2,-hauteurTable/2, POSITION_BAS_GAUCHE);
	NoeudPoint* basMilieu_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,0,-hauteurTable/2, POSITION_BAS_MILIEU);
	NoeudPoint* basDroite_= new NoeudPoint(ArbreRenduINF2990::NOM_POINT,longueurTable/2,-hauteurTable/2, POSITION_BAS_DROITE);

	/// Ajout du point symétrique à chacun des points
	hautGauche_->modifierPointSym(hautDroite_);
	hautMilieu_->modifierPointSym(basMilieu_);
	hautDroite_->modifierPointSym(hautGauche_);
	milieuGauche_->modifierPointSym(milieuDroite_);
	milieuDroite_->modifierPointSym(milieuGauche_);
	basGauche_->modifierPointSym(basDroite_);
	basMilieu_->modifierPointSym(hautMilieu_);
	basDroite_->modifierPointSym(basGauche_);

    hautMilieu_->attach(*this);
    basMilieu_->attach(*this);

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
	butJoueur1_ = new NoeudBut(ArbreRenduINF2990::NOM_BUT_MILIEU,1,hautGauche_,basGauche_,milieuGauche_);

	butJoueur2_ = new NoeudBut(ArbreRenduINF2990::NOM_BUT_MILIEU,2,hautDroite_,basDroite_,milieuDroite_);
	butJoueur1_->modifierButAdverse(butJoueur2_);
	butJoueur2_->modifierButAdverse(butJoueur1_);


	//Ajout de mur relatif aux point et buts pour qu'il s'ajustent automatiquement à leur déplacement
	NoeudMuretRelatif   *mr1 = new NoeudMuretRelatif(hautGauche_,hautMilieu_),
						*mr2 = new NoeudMuretRelatif(hautMilieu_,hautDroite_),
						*mr3 = new NoeudMuretRelatif(basMilieu_,basGauche_),
						*mr4 = new NoeudMuretRelatif(basDroite_,basMilieu_),
						*mr5 = new NoeudMuretRelatif(hautDroite_,butJoueur2_,true),//ok
						*mr6 = new NoeudMuretRelatif(butJoueur2_,basDroite_,false),
						*mr7 = new NoeudMuretRelatif(basGauche_,butJoueur1_,false),//ok
						*mr8 = new NoeudMuretRelatif(butJoueur1_,hautGauche_,true);
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
    Modele3D* pModel = obtenirModele();
    if(pModel)
    {
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        pModel->dessiner(true);
        glPopAttrib();
        glPopMatrix();
    }

    // TODO:: cache the values when modifying / loading the control points
	// Dessin des lignes sur la table et des zones de but
	// Ligne bleu de gauche 1/3
	Vecteur3 pointHautLigneGauche1 = vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_HAUT_GAUCHE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue())/3.0f;
	Vecteur3 pointcentreLigneGauche1 = vecteurPoint_[POSITION_MILIEU_GAUCHE]->obtenirPositionAbsolue()*0.33f;
	Vecteur3 pointBasLigneGauche1 = vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_BAS_GAUCHE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue())/3.0f;
	// Ligne bleu de gauche 2/3
	Vecteur3 pointHautLigneGauche2 = vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_HAUT_GAUCHE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue())*2.0f/3.0f;
	Vecteur3 pointcentreLigneGauche2 = vecteurPoint_[POSITION_MILIEU_GAUCHE]->obtenirPositionAbsolue()*0.66f;
	Vecteur3 pointBasLigneGauche2 = vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_BAS_GAUCHE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue())*2.0f/3.0f;
	// Ligne bleu de Droite 1/3
	Vecteur3 pointHautLigneDroite1 = vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_HAUT_DROITE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue())/3.0f;
	Vecteur3 pointcentreLigneDroite1 = vecteurPoint_[POSITION_MILIEU_DROITE]->obtenirPositionAbsolue() * 0.33f;
	Vecteur3 pointBasLigneDroite1 = vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_BAS_DROITE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue())/3.0f;
	// Ligne bleu de Droite 2/3
	Vecteur3 pointHautLigneDroite2 = vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_HAUT_DROITE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_HAUT_MILIEU]->obtenirPositionAbsolue())*2.0f/3.0f;
	Vecteur3 pointcentreLigneDroite2 = vecteurPoint_[POSITION_MILIEU_DROITE]->obtenirPositionAbsolue() * 0.66f;
	Vecteur3 pointBasLigneDroite2 = vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue()+ (vecteurPoint_[POSITION_BAS_DROITE]->obtenirPositionAbsolue() - vecteurPoint_[POSITION_BAS_MILIEU]->obtenirPositionAbsolue())*2.0f/3.0f;

	const float hauteurLigne = 0;
	float moitieLargeurLigne = 1.0f;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
	// Ligne bleu de gauche 1/3
	glVertex3d(pointHautLigneGauche1[VX]+moitieLargeurLigne,pointHautLigneGauche1[VY],hauteurLigne);////////
	glVertex3d(pointHautLigneGauche1[VX]-moitieLargeurLigne,pointHautLigneGauche1[VY],hauteurLigne);//////// top part de la 2re ligne.
	glVertex3d(pointcentreLigneGauche1[VX]-moitieLargeurLigne,pointcentreLigneGauche1[VY],hauteurLigne);//
	glVertex3d(pointcentreLigneGauche1[VX]+moitieLargeurLigne,pointcentreLigneGauche1[VY],hauteurLigne);

	glVertex3d(pointcentreLigneGauche1[VX]+moitieLargeurLigne,pointcentreLigneGauche1[VY],hauteurLigne);
	glVertex3d(pointcentreLigneGauche1[VX]-moitieLargeurLigne,pointcentreLigneGauche1[VY],hauteurLigne);
	glVertex3d(pointBasLigneGauche1[VX]-moitieLargeurLigne,pointBasLigneGauche1[VY],hauteurLigne);
	glVertex3d(pointBasLigneGauche1[VX]+moitieLargeurLigne,pointBasLigneGauche1[VY],hauteurLigne);

	// Ligne bleu de gauche 2/3
	glVertex3d(pointHautLigneGauche2[VX]+moitieLargeurLigne,pointHautLigneGauche2[VY],hauteurLigne);
	glVertex3d(pointHautLigneGauche2[VX]-moitieLargeurLigne,pointHautLigneGauche2[VY],hauteurLigne);
	glVertex3d(pointcentreLigneGauche2[VX]-moitieLargeurLigne,pointcentreLigneGauche2[VY],hauteurLigne);
	glVertex3d(pointcentreLigneGauche2[VX]+moitieLargeurLigne,pointcentreLigneGauche2[VY],hauteurLigne);

	
	glVertex3d(pointcentreLigneGauche2[VX]+moitieLargeurLigne,pointcentreLigneGauche2[VY],hauteurLigne);
	glVertex3d(pointcentreLigneGauche2[VX]-moitieLargeurLigne,pointcentreLigneGauche2[VY],hauteurLigne);
	glVertex3d(pointBasLigneGauche2[VX]-moitieLargeurLigne,pointBasLigneGauche2[VY],hauteurLigne);
	glVertex3d(pointBasLigneGauche2[VX]+moitieLargeurLigne,pointBasLigneGauche2[VY],hauteurLigne);
	// Ligne bleu de Droite 1/3
	glVertex3d(pointHautLigneDroite1[VX]+moitieLargeurLigne,pointHautLigneDroite1[VY],hauteurLigne);
	glVertex3d(pointHautLigneDroite1[VX]-moitieLargeurLigne,pointHautLigneDroite1[VY],hauteurLigne);
	glVertex3d(pointcentreLigneDroite1[VX]-moitieLargeurLigne,pointcentreLigneDroite1[VY],hauteurLigne);
	glVertex3d(pointcentreLigneDroite1[VX]+moitieLargeurLigne,pointcentreLigneDroite1[VY],hauteurLigne);

	
	glVertex3d(pointcentreLigneDroite1[VX]+moitieLargeurLigne,pointcentreLigneDroite1[VY],hauteurLigne);
	glVertex3d(pointcentreLigneDroite1[VX]-moitieLargeurLigne,pointcentreLigneDroite1[VY],hauteurLigne);
	glVertex3d(pointBasLigneDroite1[VX]-moitieLargeurLigne,pointBasLigneDroite1[VY],hauteurLigne);
	glVertex3d(pointBasLigneDroite1[VX]+moitieLargeurLigne,pointBasLigneDroite1[VY],hauteurLigne);
	// Ligne bleu de Droite 2/3


	glVertex3d(pointHautLigneDroite2[VX]+moitieLargeurLigne,pointHautLigneDroite2[VY],hauteurLigne);
	glVertex3d(pointHautLigneDroite2[VX]-moitieLargeurLigne,pointHautLigneDroite2[VY],hauteurLigne);
	glVertex3d(pointcentreLigneDroite2[VX]-moitieLargeurLigne,pointcentreLigneDroite2[VY],hauteurLigne);
	glVertex3d(pointcentreLigneDroite2[VX]+moitieLargeurLigne,pointcentreLigneDroite2[VY],hauteurLigne);

	
	glVertex3d(pointcentreLigneDroite2[VX]+moitieLargeurLigne,pointcentreLigneDroite2[VY],hauteurLigne);
	glVertex3d(pointcentreLigneDroite2[VX]-moitieLargeurLigne,pointcentreLigneDroite2[VY],hauteurLigne);
	glVertex3d(pointBasLigneDroite2[VX]-moitieLargeurLigne,pointBasLigneDroite2[VY],hauteurLigne);
	glVertex3d(pointBasLigneDroite2[VX]+moitieLargeurLigne,pointBasLigneDroite2[VY],hauteurLigne);
	
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
	default : throw std::logic_error("numero de joueur invalide");
	}
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
	float maxHauteurEntre2=max(obtenirPoint(POSITION_HAUT_GAUCHE)->obtenirPositionAbsolue()[VY],obtenirPoint(POSITION_HAUT_MILIEU)->obtenirPositionAbsolue()[VY]);
	float hauteurYHaut=max(maxHauteurEntre2,obtenirPoint(POSITION_HAUT_DROITE)->obtenirPositionAbsolue()[VY]); 

	// On trouve la hauteur max en bas
	maxHauteurEntre2=min(obtenirPoint(POSITION_BAS_GAUCHE)->obtenirPositionAbsolue()[VY],obtenirPoint(POSITION_BAS_MILIEU)->obtenirPositionAbsolue()[VY]);
	float hauteurYBas=-1*min(maxHauteurEntre2,obtenirPoint(POSITION_BAS_DROITE)->obtenirPositionAbsolue()[VY]);

	// On compare la hauteur max en haut et en bas
	float HauteurY=max(hauteurYHaut,hauteurYBas);
	reponse[0]=HauteurY;

	// On trouve la longueur max, on le fait sur le cote droit seulement puisqua la table est symetrique en Y
	float maxLongueurEntre2=max(obtenirPoint(POSITION_HAUT_DROITE)->obtenirPositionAbsolue()[VX],obtenirPoint(POSITION_MILIEU_DROITE)->obtenirPositionAbsolue()[VX]);
	float longueurX=max(maxLongueurEntre2,obtenirPoint(POSITION_BAS_DROITE)->obtenirPositionAbsolue()[VX]);
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

	Vecteur2 posNoeud = noeud->obtenirPositionAbsolue().convertir<2>();

	// Le noeud est a la position 0 alors il est certainement sur la table
	if(posNoeud.estNul())
		return true;

	for(int i=0; i<8; ++i)
	{
		Vecteur2 point1 = obtenirPoint(droiteMuret_[i].first)->obtenirPositionAbsolue().convertir<2>();
		Vecteur2 point2 = obtenirPoint(droiteMuret_[i].second)->obtenirPositionAbsolue().convertir<2>();
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
/// @fn NoeudMuret* NoeudTable::detectionCollisionGrandeVitesseMuret( Vecteur2 anciennePos, Vecteur2 nouvellePos )
///
/// Permet de detecter si la rondelle entre en collision avec les muret exterieur malgre la grande vitesse
///
/// @param[in] Vecteur2 anciennePos : ancienne position de la rondelle avant son deplacement
/// @param[in] Vecteur2 nouvellePos : nouvelle position de la rondelle apres son deplacement
///
/// @return Vecteur2 retourne le vecteur directeur du muret en collision
///					vecteur nul si aucune intersection
///
////////////////////////////////////////////////////////////////////////
NoeudMuret* NoeudTable::detectionCollisionGrandeVitesseMuret( const Vecteur2& anciennePos,const Vecteur2& nouvellePos, Vecteur2 &intersectionRetour )
{
	float distance = 9999999;
	NoeudMuret* retour = 0;
	NoeudGroupe* groupe = obtenirGroupe(ArbreRenduINF2990::NOM_MURET);
	if(groupe)
	{
		for(unsigned int i=0; i<groupe->obtenirNombreEnfants(); ++i)
		{
			NoeudMuret* muret = dynamic_cast<NoeudMuret*>(groupe->chercher(i));
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

	for(int i=0; i<8; ++i)
	{
		Vecteur2 point1 = obtenirPoint(droiteMuret_[i].first)->obtenirPositionAbsolue().convertir<2>();
		Vecteur2 point2 = obtenirPoint(droiteMuret_[i].second)->obtenirPositionAbsolue().convertir<2>();
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
/// @fn TiXmlElement* NoeudTable::creerNoeudXML()
///
/// /*Description*/
///
///
/// @return TiXmlElement*
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* NoeudTable::creerNoeudXML()
{
	TiXmlElement* elementNoeud = NoeudComposite::creerNoeudXML();
    
    XMLUtils::ecrireAttribute<float>(elementNoeud,"coefFriction",coefFriction_);
	for (int i = 0; i < 8 ; i++)
	{
        std::ostringstream name;
        name << "rebondBande";
        name << i;
        float coef = bande_[i]->obtenirCoefRebond();
        XMLUtils::ecrireAttribute<float>(elementNoeud,name.str().c_str(),coef);
	}

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudTable::initialiser( const TiXmlElement* element )
///
/// Initialisation du NoeudTable à partir d'un element XML
///
/// @param[in] const TiXmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudTable::initialiser( const TiXmlElement* element )
{
	if(!NoeudComposite::initialiser(element))
		return false;
    if(!XMLUtils::LireAttribute(element,"coefFriction",coefFriction_))
		return false;

	// On assigne le coefficient de rebon des bandes exterieurs
	for (int i = 0; i < 8 ; i++)
	{
		float tempElem;
        std::ostringstream name;
        name << "rebondBande";
        name << i;

        if(!XMLUtils::LireAttribute(element,name.str().c_str(),tempElem))
			return false;
		assignerCoefRebond(i,tempElem);
	}

	return true;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret* NoeudTable::obtenirMuretZoneEdition( TypePosMuretEdition type ) const
///
/// Retourne le muret de la zone d'edition associe au numero donne selon l'enum
///
/// @param[in] TypePosMuretEdition type : le numero du muret
///
/// @return NoeudMuret* : un pointeur sur le muret en question
///
////////////////////////////////////////////////////////////////////////
NoeudMuret* NoeudTable::obtenirMuretZoneEdition( TypePosMuretEdition type ) const
{
	return muretZoneEdition_[type];
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
	return obtenirGroupe(GestionnaireModeles::obtenirInstance()->obtenirNameFromTypeId(typeIdEnfant));
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudTable::reassignerParentBandeExt(  )
///
/// Ajoute les bande exterieur comme enfant au groupe de Muret pour les traiter pareil
///	Les murets relatif doivent etre creer dans le constructeur de talbes puisqu'ils doivent connaitre 
/// l'adresse memoire des points (le probleme reside dans le chargement XML, on ne peut savoir a quoi relie les
/// muret relatifs a leur chargement), 
/// Pour que les bandes exterieur soient traiter de la meme facon que les
///
///	Ceci DOIT etre appeler apres l'initialisation de l'arbre ET apres le chargement en XML
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudTable::reassignerParentBandeExt()
{
	NoeudGroupe *groupe = dynamic_cast<NoeudGroupe *>(obtenirGroupe(ArbreRenduINF2990::NOM_MURET));
	if (groupe)
	{
		for (int i = 0; i < 8 ; i++)
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
/// @param[in] const aiScene*	scene		: la scene ou est contenue le modele 3D
/// @param[in] const aiNode*	noeud		: le noeud(aiNode) a partir duquel faire la recherche traiter
/// @param[in] GroupeCoord		listePoints	: liste des points a trouver
///
/// @return GroupeTripleIndex : groupe d'index correspondant a un des points envoyes en parametre
///
////////////////////////////////////////////////////////////////////////
GroupeTripleAdresseFloat NoeudTable::trouverVertex( const aiScene* scene, const aiNode* noeud, const GroupeCoord& listePoints )
{
	GroupeTripleAdresseFloat retour; // Liste a retourner
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
		Vecteur3 deplacement(vecteurPoint_[i]->obtenirPositionInitiale()-vecteurPoint_[i]->obtenirPositionAbsolue());
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
	bande_[index]->modifierCoefRebond(coefRebond);
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
#if BOX2D_INTEGRATED

    clearPhysicsBody();

    auto pHaut = obtenirPoint(POSITION_HAUT_MILIEU), pBas = obtenirPoint(POSITION_BAS_MILIEU);
    if(pHaut && pBas)
    {
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.position.Set(0, 0); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = getWorld()->CreateBody(&myBodyDef);

        b2Vec2 topPosB2,bottomPosB2 ;
        utilitaire::VEC3_TO_B2VEC(pHaut->obtenirPositionAbsolue(),topPosB2);
        utilitaire::VEC3_TO_B2VEC(pBas->obtenirPositionAbsolue(),bottomPosB2);
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
void NoeudTable::updateObserver( PositionSubject& pSubject )
{
    updatePhysicBody();
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////