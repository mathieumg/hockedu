///////////////////////////////////////////////////////////////////////////
/// @file NoeudTable.h
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDTABLE_H__
#define __ARBRE_NOEUDS_NOEUDTABLE_H__

#include "NoeudComposite.h"
#include "NoeudPoint.h"
#include "NoeudMuret.h"
#include "PositionObserver.h"

class NodeWallAbstract;
class NoeudBut;
class NoeudGroupe;
enum TypePosPoint;

struct BonusProperty
{
    BonusType type;
    bool enabled;
    bool modified;
    float duration;
};

typedef std::map<TypePosPoint,GroupeTripleAdresseFloat> ListeIndexPoints;
///////////////////////////////////////////////////////////////////////////
/// @class NoeudTable
/// @brief Classe qui représente la table de jeu (surface simple).
///
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudTable : public NoeudComposite, public PositionObserver
{
public:
    typedef NoeudComposite Super;

	/// Constructeur à partir du type du noeud.
   NoeudTable();
   /// Destructeur.
   virtual ~NoeudTable();

   static int expectedChildCount();
   /// fonction de comparaison de 2 noeuds
   virtual bool equals(NoeudAbstrait* n)
   {
       auto t = dynamic_cast<NoeudTable*>(n);
       return !!t && mFrictionRatio==t->mFrictionRatio&& Super::equals(n);
   }
   /// Affiche le cube.
   virtual void renderReal() const;
   /// Fonction appeler dans afficher concret pour faire le
   /// rendu OpenGL, uniquement utilisé sous APPLE.
   /// utiliser les liste d'affichage pour windows
   virtual void renderOpenGLES() const;
   /// Effectue l'animation
   virtual void tick( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);
   /// Ajoute un noeud enfant.
   virtual bool add( NoeudAbstrait* enfant );
   /// Calcule la hauteur max en y plus et y moins et la longueur max de la table
   void calculerHautLongMax(float reponse[]);
   /// Algorithme pour savoir si l'element est sur la table
   bool estSurTable(NoeudAbstrait* noeud);
   bool estSurTable(Vecteur2 position);

   /// Creation du noeud XML d'un point
   virtual XmlElement* createXmlNode();
   /// Initialisation du NoeudTable à partir d'un element XML
   virtual bool initFromXml(const XmlElement* element);
   /// Permet de detecter si la rondelle entre en collision avec les muret exterieur malgre la grande vitesse
   NodeWallAbstract* detectionCollisionGrandeVitesseMuret(const Vecteur2& anciennePos,const Vecteur2& nouvellePos, Vecteur2 &intersectionRetour);
   
   /// Permet de trouver les vertex correspondants dans les modeles 3D
   static GroupeTripleAdresseFloat trouverVertex(const struct aiScene* scene, const struct aiNode* noeud, const GroupeCoord& listePoints);


   /// Ajoute les bande exterieur comme enfant au groupe de Muret pour les traiter pareil
   void reassignerParentBandeExt();

   /// Replace le modele de la table avec le deplacement des points
   void replacerModele();

   /// Accesseur de listeIndexPointsModeleTable_
   static ListeIndexPoints& obtenirListeIndexPointsModeleTable() { return listeIndexPointsModeleTable_; }
#if WIN32  
   static void initialiserListeIndexPoints(Modele3D* modele);
   static void queueTableModelMove(TypePosPoint type,const Vecteur3& move);
#endif

   /// Recreates the physics body according to current attributes
   virtual void updatePhysicBody();

   /// update the table when a modification is done on either middle control points
   virtual void updateObserver( const  class PositionSubject* pSubject );

   /// retrieves the max width of the table
   float GetWidth() const;

   BonusType getRandomBonus() const;

   static const unsigned int NB_CONTROL_POINTS = 8;
   /// taille par défaut de table
   static const Vecteur3 DEFAULT_SIZE;
   /// 3 pour les NoeudPoint et Centre, 2 pour les 4 ligne de 1/3 field
   static const int NB_HORIZONTAL_VERTICES = 3 + 2;
   /// 3 pour les NoeudPoint
   static const int NB_VERTICAL_VERTICES = 3;
   static const unsigned int NB_BANDES = 8;
   static const int TABLE_CORNER_COUNT = 4;
   static const float Z_HEIGHT_TABLE_SURFACE;               // hauteur en z ou ce trouve la surface de la table
   static const float Z_HEIGHT_TABLE_BOARDS;               // hauteur en z ou ce trouve le top de la bordure de la table
   static const float Z_HEIGHT_TABLE_EXTERIOR_BORDERS;      // hauteur en z ou ce trouve le top du contour exterieur de la table
   static const float Z_HEIGHT_TABLE_UNDER;                 // hauteur en z du dessous de la table
   static const float Z_HEIGHT_TABLE_BOTTOM;                // hauteur en z du bas des pieds de la table
private:
   ///Création du vecteur qui contient les points
   std::vector<NoeudPoint*> vecteurPoint_;

   /// Création des buts
   NoeudBut* butJoueur1_;
   NoeudBut* butJoueur2_;

   /// Id pour la texture
   //GLuint textureId_; 

   /// Coefficient de friction de la surface de la table, sauvegarder en XML
   float mFrictionRatio; // usually between 0 and 1

   /// Informations sur les 8 points modifiables
   typedef std::pair<TypePosPoint,TypePosPoint> CouplePoint;
   CouplePoint droiteMuret_[NB_BANDES];
   NodeWallAbstract* bande_[NB_BANDES];

   Vecteur3 mTableVertices[NB_HORIZONTAL_VERTICES][NB_VERTICAL_VERTICES];
   // all locations of the arc + the corner, the corner is at index 0
   Vecteur2f** mCornerArcLocations;
   int        mCornerArcLocationCount[TABLE_CORNER_COUNT];
   static ListeIndexPoints listeIndexPointsModeleTable_;
   
   BonusProperty mBonusProperties[NB_BONUS_TYPE];
   std::vector<BonusProperty> mAvailableBonuses;
/// Accesseurs
public:

    const Vecteur3** GetTableVertices() const { return (const Vecteur3**)mTableVertices; }
    // Corner Arc location accessor, count is 0 if array is invalid
    void GetCornerArcLocations( int cornerIndex, Vecteur2f*& locations, int& count );
    /// Accesseur de point dans le vecteurPoint
	NoeudPoint* obtenirPoint( int typePosPoint);
	/// Accesseur des Buts
	NoeudBut* obtenirBut( int joueur ) const;
	/// Retourne la zone occupee par la table
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Retourne le rayon du noeud
	virtual float getRadius() const;
    /// Retourne le muret de la zone d'edition correspondant a son enum

	/// Accesseur de coefFriction_
	float obtenirCoefFriction() const { return mFrictionRatio; }
	void modifierCoefFriction(float val) { mFrictionRatio = val; }

	/// Accesseur du coefRebond des bandes
	float obtenirCoefRebond(int quelBande) {return bande_[quelBande]->getReboundRatio();}
	void modifierCoefRebond(int quelBande, float coef) {bande_[quelBande]->setReboundRatio(coef);}

	/// Accesseur des groupes de la table
	NoeudGroupe* obtenirGroupe(RazerKey typeEnfant);
	static const float rayonCercleCentre_;
};


#endif // __ARBRE_NOEUDS_NOEUDTABLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
