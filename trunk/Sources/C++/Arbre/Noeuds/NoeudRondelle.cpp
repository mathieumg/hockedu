///////////////////////////////////////////////////////////////////////////////
/// @file NoeudRondelle.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudRondelle.h"
#include "ArbreRenduINF2990.h"
#include "UsineNoeudRondelle.h"
#include "FacadeModele.h"
#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "AideCollision.h"
#include "NoeudTable.h"
#include "NoeudMaillet.h"
#include "NoeudPoint.h"
#include "Partie.h"
#include "NoeudAccelerateur.h"
#include "Utilitaire.h"
#include "DecodeString.h"
#include "NoeudMuretRelatif.h"
#include <iostream>
#include "SoundFMOD.h"
#include "NoeudGroupe.h"
#include "Terrain.h"
#include "NoeudPortail.h"

unsigned int NoeudRondelle::rondellesPresentes=0;
bool UsineNoeudRondelle::bypassLimitePourTest = false;

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::NoeudRondelle(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle::NoeudRondelle(const std::string& typeNoeud)
	: NoeudAbstrait(typeNoeud),puissanceVent_(0.05)
{
	NoeudRondelle::rondellesPresentes++;

	// Test pour s'assurer que l'arbre de rendu existe
	if(FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990())
		table_ = FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->obtenirTable();

	coefFriction_ = 2.5;
	velocite_ = Vecteur3(0.0,0.0, 0.0);
	angle_ = 0.0;
	vitesseRotation_ = 0.0;

	FacadeModele::obtenirInstance()->ajouterElementSurTable(this);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::~NoeudRondelle()
///
/// Ce destructeur désallouee la liste d'affichage de la rondelle et diminuer le nombre de rondelle.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle::~NoeudRondelle()
{
	FacadeModele::obtenirInstance()->supprimerElementSurTable(this);
	NoeudRondelle::rondellesPresentes--;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::afficherConcret() const
{
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudAbstrait::afficherConcret();

    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::animer(float temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::animer( const float& temps)
{
	//A mettre dans majPosition();
	//positionRelative_ += velocite_*temps;
	// Appel à la version de la classe de base pour l'animation des enfants.
	NoeudAbstrait::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::accueillirVisiteurNoeud( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::accueillirVisiteurNoeud( VisiteurNoeud& v )
{
	v.visiterNoeudRondelle(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::gestionCollision( float temps )
///
/// Application des lois de la physique
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::gestionCollision( const float& temps )
{
	// Reinitialisation du vecteur d'enfoncement
	enfoncement_.remetAZero();
	vitesseResultante_.remetAZero();
	bonusAccelResultant_ = 1;

	positionRelative_ = anciennePos_;
	//const double cycles = 10;
	//double dt = temps/cycles;
	//for(int i=0; i<cycles;i++)
	{
		
		positionRelative_+=velocite_*temps;

		Vecteur2 vecteurDirecteur(positionRelative_-anciennePos_);
		vecteurDirecteur.normaliser();
		Vecteur2 normale(vecteurDirecteur[VY],-vecteurDirecteur[VX]);	// Pointe vers la droite

		const Vecteur2 NormaleLongueurRayon = normale*obtenirRayon();
		const Vecteur2 DirecteurLongueurRayon = vecteurDirecteur*obtenirRayon();
		collision_ = false;

		// Collision avec un portail et téléportation
		NoeudGroupe*  groupe = table_->obtenirGroupe(ArbreRenduINF2990::NOM_PORTAIL);
		if(groupe)
		{
			// Parcours de la liste des portails
			unsigned int nbEnfant = groupe->obtenirNombreEnfants();
			for(unsigned int i = 0; i < nbEnfant; ++i)
			{
				NoeudPortail* portail = dynamic_cast<NoeudPortail*>(groupe->chercher(i));
				if(portail)
				{
					double sommeRayon = (portail->obtenirRayon())/5 + obtenirRayon();
					Vecteur3 distance(portail->obtenirPositionAbsolue()-obtenirPositionAbsolue());

					// Collision
					if(portail->champAttractionActive() && distance.norme() <= sommeRayon && nbEnfant > 1 )
					{
						// Choix aléatoire du portail de sortie
						int noPortailDeSortie = 0;
						while((noPortailDeSortie = rand()%nbEnfant) == i);

						NoeudPortail* portailDeSortie = dynamic_cast<NoeudPortail*>(groupe->chercher(noPortailDeSortie));
						portailDeSortie->modifierChampAttractionActif(false);
						anciennePos_ = positionRelative_ = portailDeSortie->obtenirPositionRelative();
						enfoncement_.remetAZero();
						collision_ = false;
						SoundFMOD::obtenirInstance()->playEffect(effect(PORTAL_EFFECT));
						return;
					}
				}
			}
		}


		/// Collisions sur les maillets
		NoeudMaillet* maillet = FacadeModele::obtenirInstance()->obtenirMailletJoueurGauche();
		
		for (int i = 0; i < 2 ; i++)
		{
			if(maillet)
			{
				aidecollision::DetailsCollision details = aidecollision::calculerCollisionCercle(
					maillet->obtenirPositionAbsolue().convertir<2>(),maillet->obtenirRayon(),
					obtenirPositionAbsolue().convertir<2>(),obtenirRayon());
				if(details.type != aidecollision::COLLISION_AUCUNE)
				{
					Vecteur3 droiteEntreCentres = maillet->obtenirPositionAbsolue()-obtenirPositionAbsolue();
					bool collisionMaillet = false;
					double time = aidecollision::TimeOfClosestApproach(
						anciennePos_.convertir<2>(),
						maillet->obtenirAnciennePos().convertir<2>(),
						velocite_.convertir<2>(),
						maillet->obtenirVelocite().convertir<2>(),
						obtenirRayon(),
						maillet->obtenirRayon(),
						collisionMaillet);
					if(collisionMaillet && time <= temps)
					{
						Vecteur3 nouvellePosTOIRondelle = anciennePos_+velocite_*time;
						Vecteur3 nouvellePosTOIMaillet = maillet->obtenirAnciennePos()+maillet->obtenirVelocite()*time;
						droiteEntreCentres = nouvellePosTOIRondelle-nouvellePosTOIMaillet;
						droiteEntreCentres.normaliser();
					}
					details.direction.normaliser();
					enfoncement_ -= details.direction*details.enfoncement;

					droiteEntreCentres.normaliser();
					Vecteur3 normale(droiteEntreCentres[VY],-droiteEntreCentres[VX]);

					Vecteur3 reflexion = calculerReflexion(velocite_,droiteEntreCentres);
					vitesseResultante_ += reflexion;

					Vecteur3 projectionTemp = calculerProjectionDroite(maillet->obtenirVelocite(),droiteEntreCentres);
					vitesseResultante_ += projectionTemp*1.3;

					SoundFMOD::obtenirInstance()->playEffect(effect(COLLISION_MAILLET_EFFECT1+(rand()%5)));			
					collision_ = true;
				}
			}
			
			maillet = FacadeModele::obtenirInstance()->obtenirMailletJoueurDroit();
		}

		// Collision avec un accelerateur
		groupe = table_->obtenirGroupe(ArbreRenduINF2990::NOM_ACCELERATEUR);
		if(groupe)
		{
			unsigned int nbEnfant = groupe->obtenirNombreEnfants();
			for(unsigned int i=0; i<nbEnfant; ++i)
			{
				NoeudAccelerateur* accel = dynamic_cast<NoeudAccelerateur*>(groupe->chercher(i));
				if(accel)
				{
					double sommeRayon = accel->obtenirRayon()+obtenirRayon();
					Vecteur3 distance(accel->obtenirPositionAbsolue()-obtenirPositionAbsolue());
					// Collision
					if(distance.norme2() <= sommeRayon*sommeRayon)
					{
						if(accel->estActiver())
						{
							bonusAccelResultant_ *= accel->obtenirBonusAccel();
							accel->modifierActiver(false);
							SoundFMOD::obtenirInstance()->playEffect(effect(ACCELERATOR_EFFECT));
						}
					}
				}
			}
		}

		

		// TODO:::  Pas faire les tests avec les objets statique lorsque la velocite est nul 
		groupe = table_->obtenirGroupe(ArbreRenduINF2990::NOM_MURET);
		/// Collision les murets
		if(groupe)
		{
			VisiteurCollision v(this,false);
		
			v.reinitialiser();
			groupe->accueillirVisiteurNoeud(v);
			if(v.collisionPresente())
			{
				ConteneurNoeuds liste;
				v.obtenirListeCollision(liste);
				ConteneurNoeuds::iterator iter = liste.begin();
				ConteneurDetailsCollision details = v.obtenirConteneurDetailsCollision();
				aidecollision::DetailsCollision detailsRes;
				detailsRes.type = aidecollision::COLLISION_SEGMENT;
				detailsRes.enfoncement = 0;
				detailsRes.direction.remetAZero();
				double rebond = 1;
				int i=0;
				for(; iter != liste.end(); iter++, ++i)
				{
					NoeudMuret *muret = dynamic_cast<NoeudMuret *>((*iter));
					if (muret)
					{
						rebond *= muret->obtenirCoefRebond();
						enfoncement_ -= details[i].direction*details[i].enfoncement*1.05;
						detailsRes.enfoncement += details[i].enfoncement;
						detailsRes.direction += details[i].direction;
					}
				}
				detailsRes.direction.normaliser();
				Vecteur3 normale(-detailsRes.direction[VY],detailsRes.direction[VX]);

				vitesseResultante_ += calculerReflexion(velocite_,normale)*-1*rebond;
				
				collision_ = true;
				if(velocite_.norme2() > 200)
					SoundFMOD::obtenirInstance()->playEffect(COLLISION_MURET_EFFECT);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::majPosition( float temps )
///
/// Permet de faire un tick d'avancement sur cet objet
///
/// @param[in] float temps : un tick de temps en secondes
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::majPosition( const float& temps )
{
	anciennePos_ = positionRelative_;
	positionRelative_ += velocite_*temps;
	angle_ = (int)(angle_ + 5*vitesseRotation_)%360;
	updateMatrice();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::ajusterEnfoncement()
///
/// Gestion de l'enfoncement de la rondelle dans les obstacles.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::ajusterEnfoncement()
{
	positionRelative_ -= enfoncement_*1.1;
	// Initialisation d'un vecteur pour l'intersection
	Vecteur2 intersection;
	// Inversion du vecteur de direction de la rondelle
	//////// La methode doit retourne le muret pas juste ces point pour le coef de rebond
	NoeudMuret* muret = table_->detectionCollisionGrandeVitesseMuret(anciennePos_.convertir<2>(),positionRelative_.convertir<2>(),intersection);
	if(muret)
	{
		Vecteur2 directeur(muret->obtenirCoin2()-muret->obtenirCoin1());
		directeur.normaliser();
		Vecteur2 normale(-directeur[VY],directeur[VX]);
		Vecteur2 direction(anciennePos_-positionRelative_);
		if(produitScalaire(normale,direction) < 0)
			normale*=-1;
		// La normale de la normale est la droite elle meme
		velocite_ = calculerReflexion(velocite_,directeur.convertir<3>())*-1*1.10;//muret->obtenirCoefRebond();
		normale*= obtenirRayon();
		positionRelative_ = intersection.convertir<3>()+normale;
	}

	if(!table_->estSurTable(this) )
	{
		Partie* partie = FacadeModele::obtenirInstance()->obtenirPartieCourante();
		if(partie != 0)
		{
			if(positionRelative_[VX] < 0)
			{
				partie->incrementerPointsJoueurDroit();
				
			}
			else
			{
				partie->incrementerPointsJoueurGauche();
			}
			SoundFMOD::obtenirInstance()->playEffect(GOAL_EFFECT);
			partie->afficherScore();
			// RENDU DANS afficherScore() de Partie
// 			FacadeModele::obtenirInstance()->togglePause();
// 			utilitaire::afficherErreur("Pointage: "+DecodeString::toString(partie->obtenirPointsJoueurGauche() )+"\t"+DecodeString::toString(partie->obtenirPointsJoueurDroit() ) );
// 			FacadeModele::obtenirInstance()->togglePause();
		}
		partie->miseAuJeu();
		// Rendu dans miseAuJeu de Partie
// 		positionRelative_ = positionOriginale_;
 		velocite_.remetAZero();
		
		
	}
	positionRelative_[VZ] = 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::ajusterVitesse( float temps )
///
/// Mise à jour de la vitesse et de la vitesse de rotation de la rondelle.
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::ajusterVitesse( const float& temps )
{
	// Ajustement de la vitesse par les portails
	NoeudAbstrait* groupe = (NoeudAbstrait*)table_->obtenirGroupe(ArbreRenduINF2990::NOM_PORTAIL);
	if(groupe)
	{
		// Parcours de la liste des portails
		unsigned int nbEnfant = groupe->obtenirNombreEnfants();
		for(unsigned int i = 0; i < nbEnfant; ++i)
		{
			// Le portail à traiter
			NoeudPortail* portail = dynamic_cast<NoeudPortail*>(groupe->chercher(i));
			if(portail)
			{
				// Distance entre le centre du portail et le centre de la rondelle
				Vecteur3 distance(portail->obtenirPositionAbsolue()-obtenirPositionAbsolue());
				
				// La vitesse sera affectée seulement si la distance est plus petite que 50
				if(distance.norme() <= 50)
				{
					if(portail->champAttractionActive())
					{
						Vecteur3 ajustement = distance;
						ajustement.normaliser();

						// Ajustement de la vitesse
						if(distance.norme2() != 0)
							ajustement *= 37*portail->obtenirRayon()/(distance.norme2());

						velocite_+= ajustement;
					}
				}
				else if(!portail->champAttractionActive())
				{
					portail->modifierChampAttractionActif(true);
				}
					
			}
		}
	}
	


	// Algorithme pour calculer le vent !!!!!
	Vecteur3 directeur;
	if(positionRelative_[VX] == 0 && positionRelative_[VY] == 0)
	{
		// si la rondelle est au centre du terrain on lui donne une direction aleatoire pour commencer le mouvement
		directeur = Vecteur3((rand()&1) == 0 ? 1 : -1, (rand()&1) == 0 ? 1 : -1);
	}
	else
	{
		directeur = Vecteur3(2*coeffDirectVent_*sqrt(abs(positionRelative_[VX])/coeffDirectVent_)*(positionRelative_[VX] <0?-1:1),positionRelative_[VY] <0?-1:1);
	}
	directeur.normaliser();
	directeur *= -coeffVent_*positionRelative_[VX]*positionRelative_[VX]+puissanceVent_;
	velocite_ += directeur;
	
	///////////////////////////////////////////////


	// Algo pour rotation !!
	if(collision_)
	{
		Vecteur3 vitesseAvant( -1 * velocite_ );
		vitesseAvant.normaliser();
		Vecteur3 vitesseApres(vitesseResultante_);
		vitesseApres.normaliser();

		if((vitesseAvant[VX] < 0 && vitesseApres[VX] < 0 && vitesseAvant[VY] < 0 && vitesseApres[VY] > 0) ||
			(vitesseAvant[VX] > 0 && vitesseApres[VX] < 0 && vitesseAvant[VY] < 0 && vitesseApres[VY] < 0) ||
			(vitesseAvant[VX] > 0 && vitesseApres[VX] > 0 && vitesseAvant[VY] > 0 && vitesseApres[VY] < 0) ||
			(vitesseAvant[VX] < 0 && vitesseApres[VX] > 0 && vitesseAvant[VY] > 0 && vitesseApres[VY] > 0))
			vitesseRotation_ = -1 * acos(produitScalaire(vitesseAvant, vitesseApres));
		else
			vitesseRotation_ = acos(produitScalaire(vitesseAvant, vitesseApres));

		velocite_ = vitesseResultante_;
	}

	// Diminution de la vitesse par la friction
	velocite_ *= (1.0-coefFriction_*temps);
	// Modification de la vitesse selon les bonus accel
	velocite_ *= bonusAccelResultant_;

	// Cap de vitesse
	if(velocite_.norme2() > 1000000)
	{
		velocite_.normaliser();
		velocite_*= 1000;
	}

	velocite_[VZ] = 0;
	vitesseRotation_ *= 0.99;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::validerPropriteteTablePourJeu()
///
/// Permet de valider la position originale de la rondelle lorsqu'on passe au mode jeu
/// Pour connaitre la position de reinitialisation apres un but
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::validerPropriteteTablePourJeu() throw(std::logic_error)
{
	if(obtenirTerrain())
	{
		table_ = obtenirTerrain()->obtenirTable();
		if(table_ != NULL)
		{
			positionOriginale_ = positionRelative_;
			// CoeffVent _ = puissanceMax/ x^2
			double x = table_->obtenirPoint(POSITION_MILIEU_DROITE)->obtenirPositionAbsolue()[VX]*0.50;
			coeffVent_ = puissanceVent_/(x*x);


			//coeffDirectVent_ = X/Y^2
			Vecteur3 ref = rand()&1 ? table_->obtenirPoint(POSITION_HAUT_DROITE)->obtenirPositionAbsolue() : table_->obtenirPoint(POSITION_BAS_DROITE)->obtenirPositionAbsolue();
			coeffDirectVent_ = ref[VX]/ref[VY]/ref[VY];

			coefFriction_ = table_->obtenirCoefFriction()/10;
		}
		else
			throw std::logic_error("Aucune table sur le terrain de la rondelle");
	}
	else
		throw std::logic_error("Aucun terrain pour la rondelle qui tente de ce valider");
}








///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
