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
#include "aiScene.h"
#include "NoeudPoint.h"
#include "NoeudMuret.h"
#include "PositionSubject.h"

class NodeWallAbstract;
class NoeudBut;
class NoeudGroupe;
enum TypePosPoint;

typedef std::map<TypePosPoint,GroupeTripleAdresseFloat> ListeIndexPoints;
enum TypePosMuretEdition{HAUT,GAUCHE,DROITE,BAS};
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

	/// Constructeur à partir du type du noeud.
   NoeudTable(const std::string& typeNoeud);
   /// Destructeur.
   ~NoeudTable();

   /// Affiche le cube.
   virtual void afficherConcret() const;
   /// Effectue l'animation
   virtual void animer( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);
   /// Ajoute un noeud enfant.
   virtual bool ajouter( NoeudAbstrait* enfant );
   /// Calcule la hauteur max en y plus et y moins et la longueur max de la table
   void calculerHautLongMax(float reponse[]);
   /// Algorithme pour savoir si l'element est sur la table
   bool estSurTable(NoeudAbstrait* noeud);
   bool estSurTable(Vecteur2 position);

   /// Creation du noeud XML d'un point
   virtual XmlElement* creerNoeudXML();
   /// Initialisation du NoeudTable à partir d'un element XML
   virtual bool initialiser(const XmlElement* element);
   /// Permet de detecter si la rondelle entre en collision avec les muret exterieur malgre la grande vitesse
   NodeWallAbstract* detectionCollisionGrandeVitesseMuret(const Vecteur2& anciennePos,const Vecteur2& nouvellePos, Vecteur2 &intersectionRetour);
   
   /// Permet de trouver les vertex correspondants dans les modeles 3D
   static GroupeTripleAdresseFloat trouverVertex(const aiScene* scene, const aiNode* noeud, const GroupeCoord& listePoints);


   /// Ajoute les bande exterieur comme enfant au groupe de Muret pour les traiter pareil
   void reassignerParentBandeExt();

   /// Replace le modele de la table avec le deplacement des points
   void replacerModele();

   /// Assigner le coefficient de rebond a la bande voulu a lindex voulu
   void assignerCoefRebond(int index, float coefRebond);


   static void initialiserListeIndexPoints(Modele3D* modele);

   /// Recreates the physics body according to current attributes
   virtual void updatePhysicBody();

   /// update the table when a modification is done on either middle control points
   virtual void updateObserver( class PositionSubject* pSubject );


   static const unsigned int NB_CONTROL_POINTS = 8;
   /// taille par défaut de table
   static const Vecteur3 DEFAULT_SIZE;
private:
   
      
   ///Création du vecteur qui contient les points
   std::vector<NoeudPoint*> vecteurPoint_;

   /// Création des buts
   NoeudBut* butJoueur1_;
   NoeudBut* butJoueur2_;

   /// Id pour la texture
   //GLuint textureId_; 

   /// Coefficient de friction de la surface de la table, sauvegarder en XML
   float coefFriction_;

   /// Informations sur les 8 points modifiables
   typedef std::pair<TypePosPoint,TypePosPoint> CouplePoint;
   CouplePoint droiteMuret_[8];
   NodeWallAbstract* bande_[8];

   
   static ListeIndexPoints listeIndexPointsModeleTable_;
   
   /// Variable pour stocker les murets de la zone d'edition
   NodeWallAbstract* muretZoneEdition_[4];


/// Accesseurs
public:

	/// Accesseur de listeIndexPointsModeleTable_
	static ListeIndexPoints& obtenirListeIndexPointsModeleTable() { return listeIndexPointsModeleTable_; }

	/// Accesseur de point dans le vecteurPoint
	NoeudPoint* obtenirPoint( int typePosPoint);
	/// Accesseur des Buts
	NoeudBut* obtenirBut( int joueur ) const;
	/// Retourne la zone occupee par la table
	virtual PaireVect3 obtenirZoneOccupee() const;
	/// Retourne le rayon du noeud
	virtual float obtenirRayon() const;
	/// Retourne le muret de la zone d'edition correspondant a son enum
	NodeWallAbstract* obtenirMuretZoneEdition(TypePosMuretEdition type) const;

	/// Accesseur de coefFriction_
	float obtenirCoefFriction() const { return coefFriction_; }
	void modifierCoefFriction(float val) { coefFriction_ = val; }

	/// Accesseur du coefRebond des bandes
	float obtenirCoefRebond(int quelBande) {return bande_[quelBande]->getReboundRatio();}
	void modifierCoefRebond(int quelBande, float coef) {bande_[quelBande]->setReboundRatio(coef);}

	/// Accesseur des groupes de la table
	NoeudGroupe* obtenirGroupe(std::string typeEnfant);
	NoeudGroupe* obtenirGroupe(unsigned int typeIdEnfant);



	static const float rayonCercleCentre_;

	

};


#endif // __ARBRE_NOEUDS_NOEUDTABLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
