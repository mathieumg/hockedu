///////////////////////////////////////////////////////////////////////////////
/// @file NoeudRondelle.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudRondelle.h"
#include "FacadeModele.h"
#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "AideCollision.h"
#include "NoeudTable.h"
#include "NoeudMaillet.h"
#include "NoeudPoint.h"
#include "Partie.h"
#include "NoeudAccelerateur.h"
#include "DecodeString.h"
#include "NoeudMuretRelatif.h"
#include <iostream>
#include "SoundFMOD.h"
#include "NoeudGroupe.h"
#include "Terrain.h"
#include "NoeudPortail.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "NoeudBut.h"
#include "UsineNoeud.h"

unsigned int NoeudRondelle::rondellesPresentes=0;
bool UsineNoeudRondelle::bypassLimitePourTest = false;


const float NoeudRondelle::DEFAULT_RADIUS = 8;

CreateListDelegateImplementation(Puck)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudRondelle::DEFAULT_RADIUS);
}

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
	: NoeudAbstrait(typeNoeud),puissanceVent_(0.05f)
{
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);

    NoeudRondelle::rondellesPresentes++;

	mCoefFriction = 2.5f;
	mVelocite = Vecteur3(0.0f,0.0f,0.0f);
	mAngle = 0.0f;
	mVitesseRotation = 0.0f;


	FacadeModele::getInstance()->ajouterElementSurTable(this);
    updatePhysicBody();
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
	FacadeModele::getInstance()->supprimerElementSurTable(this);
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
	//positionRelative_ += mVelocite*temps;
	// Appel à la version de la classe de base pour l'animation des enfants.
	NoeudAbstrait::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::acceptVisitor( VisiteurNoeud& v )
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
    if(!table_)
    {
        if(GetTerrain())
        {
            table_ = GetTerrain()->getTable();
        }
        if(!table_)
        {
            return;
        }
    }
    
    // Reinitialisation du vecteur d'enfoncement
	enfoncement_.remetAZero();
	vitesseResultante_.remetAZero();
	bonusAccelResultant_ = 1;

	positionRelative_ = anciennePos_;
	//const float cycles = 10;
	//float dt = temps/cycles;
	//for(int i=0; i<cycles;i++)
	{
		
		positionRelative_+=mVelocite*temps;

		Vecteur2 vecteurDirecteur(positionRelative_-anciennePos_);
		vecteurDirecteur.normaliser();
		Vecteur2 normale(vecteurDirecteur[VY],-vecteurDirecteur[VX]);	// Pointe vers la droite

		const Vecteur2 NormaleLongueurRayon = normale*obtenirRayon();
		const Vecteur2 DirecteurLongueurRayon = vecteurDirecteur*obtenirRayon();
		collision_ = false;

		// Collision avec un portail et téléportation
		NoeudGroupe*  groupe = table_->obtenirGroupe(RazerGameUtilities::NOM_PORTAIL);
		if(groupe)
		{
			// Parcours de la liste des portails
			unsigned int nbEnfant = groupe->obtenirNombreEnfants();
			for(unsigned int i = 0; i < nbEnfant; ++i)
			{
				NoeudPortail* portail = dynamic_cast<NoeudPortail*>(groupe->chercher(i));
				if(portail)
				{
					float sommeRayon = (portail->obtenirRayon())/5 + obtenirRayon();
					Vecteur3 distance(portail->obtenirPositionAbsolue()-obtenirPositionAbsolue());

					// Collision
					if(portail->isAttractionFieldActive() && distance.norme() <= sommeRayon && nbEnfant > 1 )
					{
						// Choix aléatoire du portail de sortie
						int noPortailDeSortie = 0;
						while((noPortailDeSortie = rand()%nbEnfant) == i);

						NoeudPortail* portailDeSortie = dynamic_cast<NoeudPortail*>(groupe->chercher(noPortailDeSortie));
						portailDeSortie->setIsAttractionFieldActive(false);
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
		NoeudMaillet* maillet = FacadeModele::getInstance()->obtenirMailletJoueurGauche();
		
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
					float time = aidecollision::TimeOfClosestApproach(
						anciennePos_.convertir<2>(),
						maillet->obtenirAnciennePos().convertir<2>(),
						mVelocite.convertir<2>(),
						maillet->obtenirVelocite().convertir<2>(),
						obtenirRayon(),
						maillet->obtenirRayon(),
						collisionMaillet);
					if(collisionMaillet && time <= temps)
					{
						Vecteur3 nouvellePosTOIRondelle = anciennePos_+mVelocite*time;
						Vecteur3 nouvellePosTOIMaillet = maillet->obtenirAnciennePos()+maillet->obtenirVelocite()*time;
						droiteEntreCentres = nouvellePosTOIRondelle-nouvellePosTOIMaillet;
						droiteEntreCentres.normaliser();
					}
					details.direction.normaliser();
					enfoncement_ -= details.direction*details.enfoncement;

					droiteEntreCentres.normaliser();
					Vecteur3 normale(droiteEntreCentres[VY],-droiteEntreCentres[VX]);

					Vecteur3 reflexion = calculerReflexion(mVelocite,droiteEntreCentres);
					vitesseResultante_ += reflexion;

					Vecteur3 projectionTemp = calculerProjectionDroite(maillet->obtenirVelocite(),droiteEntreCentres);
					vitesseResultante_ += projectionTemp*1.3f;

					SoundFMOD::obtenirInstance()->playEffect(effect(COLLISION_MAILLET_EFFECT1+(rand()%5)));			
					collision_ = true;
				}
			}
			
			maillet = FacadeModele::getInstance()->obtenirMailletJoueurDroit();
		}

		// Collision avec un accelerateur
		groupe = table_->obtenirGroupe(RazerGameUtilities::NOM_ACCELERATEUR);
		if(groupe)
		{
			unsigned int nbEnfant = groupe->obtenirNombreEnfants();
			for(unsigned int i=0; i<nbEnfant; ++i)
			{
				NoeudAccelerateur* accel = dynamic_cast<NoeudAccelerateur*>(groupe->chercher(i));
				if(accel)
				{
					float sommeRayon = accel->obtenirRayon()+obtenirRayon();
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
		groupe = table_->obtenirGroupe(RazerGameUtilities::NOM_MURET);
		/// Collision les murets
		if(groupe)
		{
			VisiteurCollision v(this,false);
		
			v.reinitialiser();
			groupe->acceptVisitor(v);
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
				float rebond = 1;
				int i=0;
				for(; iter != liste.end(); iter++, ++i)
				{
					NodeWallAbstract *muret = dynamic_cast<NodeWallAbstract *>((*iter));
					if (muret)
					{
						rebond *= muret->getReboundRatio();
						enfoncement_ -= details[i].direction*details[i].enfoncement*1.05f;
						detailsRes.enfoncement += details[i].enfoncement;
						detailsRes.direction += details[i].direction;
					}
				}
				detailsRes.direction.normaliser();
				Vecteur3 normale(-detailsRes.direction[VY],detailsRes.direction[VX]);

				vitesseResultante_ += calculerReflexion(mVelocite,normale)*-1*rebond;
				
				collision_ = true;
				if(mVelocite.norme2() > 200)
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
	positionRelative_ += mVelocite*temps;
	mAngle = (float)((int)(mAngle + 5*mVitesseRotation)%360);
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
    if(!table_)
    {
        if(GetTerrain())
        {
            table_ = GetTerrain()->getTable();
        }
        if(!table_)
        {
            return;
        }
    }

	positionRelative_ -= enfoncement_*1.1f;
	// Initialisation d'un vecteur pour l'intersection
	Vecteur2 intersection;
	// Inversion du vecteur de direction de la rondelle
	//////// La methode doit retourne le muret pas juste ces point pour le coef de rebond
	NodeWallAbstract* muret = table_->detectionCollisionGrandeVitesseMuret(anciennePos_.convertir<2>(),positionRelative_.convertir<2>(),intersection);
	if(muret)
	{
		Vecteur2 directeur(muret->obtenirCoin2()-muret->obtenirCoin1());
		directeur.normaliser();
		Vecteur2 normale(-directeur[VY],directeur[VX]);
		Vecteur2 direction(anciennePos_-positionRelative_);
		if(produitScalaire(normale,direction) < 0)
			normale*=-1;
		// La normale de la normale est la droite elle meme
		mVelocite = calculerReflexion(mVelocite,directeur.convertir<3>())*-1*1.10f;//muret->getReboundRatio();
		normale*= obtenirRayon();
		positionRelative_ = intersection.convertir<3>()+normale;
	}

	if(!table_->estSurTable(this) )
	{
		Partie* partie = FacadeModele::getInstance()->obtenirPartieCourante();
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
// 			FacadeModele::getInstance()->togglePause();
// 			utilitaire::afficherErreur("Pointage: "+DecodeString::toString(partie->obtenirPointsJoueurGauche() )+"\t"+DecodeString::toString(partie->obtenirPointsJoueurDroit() ) );
// 			FacadeModele::getInstance()->togglePause();
		}
		partie->miseAuJeu();
		// Rendu dans miseAuJeu de Partie
// 		positionRelative_ = positionOriginale_;
 		mVelocite.remetAZero();
		
		
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
    if(!table_)
    {
        if(GetTerrain())
        {
            table_ = GetTerrain()->getTable();
        }
        if(!table_)
        {
            return;
        }
    }

	// Ajustement de la vitesse par les portails
	NoeudAbstrait* groupe = (NoeudAbstrait*)table_->obtenirGroupe(RazerGameUtilities::NOM_PORTAIL);
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
					if(portail->isAttractionFieldActive())
					{
						Vecteur3 ajustement = distance;
						ajustement.normaliser();

						// Ajustement de la vitesse
						if(distance.norme2() != 0)
							ajustement *= 37*portail->obtenirRayon()/(distance.norme2());

						mVelocite+= ajustement;
					}
				}
				else if(!portail->isAttractionFieldActive())
				{
					portail->setIsAttractionFieldActive(true);
				}
					
			}
		}
	}
	


	// Algorithme pour calculer le vent !!!!!
	Vecteur3 directeur;
	if(positionRelative_[VX] == 0 && positionRelative_[VY] == 0)
	{
		// si la rondelle est au centre du terrain on lui donne une direction aleatoire pour commencer le mouvement
		directeur = Vecteur3((rand()&1) == 0 ? 1.0f : -1.0f, (rand()&1) == 0 ? 1.0f : -1.0f);
	}
	else
	{
		directeur = Vecteur3(2*coeffDirectVent_*sqrt(abs(positionRelative_[VX])/coeffDirectVent_)*(positionRelative_[VX] <0.0f?-1.0f:1.0f),positionRelative_[VY] <0.0f?-1.0f:1.0f);
	}
	directeur.normaliser();
	directeur *= -coeffVent_*positionRelative_[VX]*positionRelative_[VX]+puissanceVent_;
	mVelocite += directeur;
	
	///////////////////////////////////////////////


	// Algo pour rotation !!
	if(collision_)
	{
		Vecteur3 vitesseAvant( -1 * mVelocite );
		vitesseAvant.normaliser();
		Vecteur3 vitesseApres(vitesseResultante_);
		vitesseApres.normaliser();

		if((vitesseAvant[VX] < 0 && vitesseApres[VX] < 0 && vitesseAvant[VY] < 0 && vitesseApres[VY] > 0) ||
			(vitesseAvant[VX] > 0 && vitesseApres[VX] < 0 && vitesseAvant[VY] < 0 && vitesseApres[VY] < 0) ||
			(vitesseAvant[VX] > 0 && vitesseApres[VX] > 0 && vitesseAvant[VY] > 0 && vitesseApres[VY] < 0) ||
			(vitesseAvant[VX] < 0 && vitesseApres[VX] > 0 && vitesseAvant[VY] > 0 && vitesseApres[VY] > 0))
			mVitesseRotation = -1 * acos(produitScalaire(vitesseAvant, vitesseApres));
		else
			mVitesseRotation = acos(produitScalaire(vitesseAvant, vitesseApres));

		mVelocite = vitesseResultante_;
	}

	// Diminution de la vitesse par la friction
	mVelocite *= (1.0f-mCoefFriction*temps);
	// Modification de la vitesse selon les bonus accel
	mVelocite *= bonusAccelResultant_;

	// Cap de vitesse
	if(mVelocite.norme2() > 1000000)
	{
		mVelocite.normaliser();
		mVelocite*= 1000;
	}

	mVelocite[VZ] = 0;
	mVitesseRotation *= 0.99f;
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
	if(GetTerrain())
	{
		table_ = GetTerrain()->getTable();
		if(table_ != NULL)
		{
			positionOriginale_ = positionRelative_;
			// CoeffVent _ = puissanceMax/ x^2
			float x = table_->obtenirPoint(POSITION_MILIEU_DROITE)->obtenirPositionAbsolue()[VX]*0.50f;
			coeffVent_ = puissanceVent_/(x*x);


			//coeffDirectVent_ = X/Y^2
			Vecteur3 ref = rand()&1 ? table_->obtenirPoint(POSITION_HAUT_DROITE)->obtenirPositionAbsolue() : table_->obtenirPoint(POSITION_BAS_DROITE)->obtenirPositionAbsolue();
			coeffDirectVent_ = ref[VX]/ref[VY]/ref[VY];

			mCoefFriction = table_->obtenirCoefFriction()/10;
		}
		else
			throw std::logic_error("Aucune table sur le terrain de la rondelle");
	}
	else
		throw std::logic_error("Aucun terrain pour la rondelle qui tente de ce valider");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::updatePhysicBody()
{
#if BOX2D_INTEGRATED

    if(getWorld())
    {
        clearPhysicsBody();

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body

        float puckRadius = obtenirRayon();

        Vecteur3 pos = obtenirPositionAbsolue();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
        myBodyDef.angle = 0; //set the starting angle
        myBodyDef.linearDamping = 0.5f;
        myBodyDef.angularDamping = 0.1f;
        mPhysicBody = getWorld()->CreateBody(&myBodyDef);
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0); //position, relative to body position
        circleShape.m_radius = puckRadius*utilitaire::ratioWorldToBox2D; //radius

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
        myFixtureDef.density = 0.02f;
        myFixtureDef.friction = 0.1f;
        myFixtureDef.restitution = 0.95f;

        // Il s'agit ici d'une rondelle qui peut entre en collision avec un maillet, un mur, un portail ou un boost
        myFixtureDef.filter.categoryBits = CATEGORY_PUCK;
        myFixtureDef.filter.maskBits = CATEGORY_MALLET | CATEGORY_BOUNDARY | CATEGORY_PORTAL | CATEGORY_BOOST;


        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;

        /// Update Goals physics body accordingly with puck radius
        Terrain* terrain = GetTerrain();
        if(terrain)
        {
            NoeudBut* goals[2];
            terrain->getGoals(goals);

            for(int i=0; i<2; ++i)
            {
                NoeudBut* goal = goals[i];
                if(goal)
                {
                    goal->updatePuckCatcher(puckRadius);
                }
            }
        }
    }

#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
///
/// /*Description*/
///
/// @param[in] const ObjectAnimationParameters & pAnimationResult
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
{
    if(pAnimationResult.CanUpdatedPosition())
        assignerPositionRelative(pAnimationResult.mPosition);
    if(pAnimationResult.CanUpdatedAngle())
        mAngle = pAnimationResult.mAngle[VZ];
    if(pAnimationResult.CanUpdatedScale())
        echelleCourante_ = pAnimationResult.mScale;
    updateMatrice();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
