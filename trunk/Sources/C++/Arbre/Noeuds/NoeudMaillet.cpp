///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMaillet.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudMaillet.h"
#include "ArbreRenduINF2990.h"
#include "UsineNoeudMaillet.h"
#include "FacadeModele.h"
#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "NoeudTable.h"
#include "NoeudRondelle.h"
#include "NoeudGroupe.h"
#include "Partie.h"
#include "NoeudBut.h"
#include <iostream>
#include "JoueurVirtuel.h"
#include "Terrain.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "DebugRenderBox2D.h"
#include "GestionnaireEvenements.h"

unsigned int NoeudMaillet::mailletExistant = 0;
bool UsineNoeudMaillet::bypassLimitePourTest = false;



b2Body* mMouseBody;

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet::NoeudMaillet(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet::NoeudMaillet(const std::string& typeNoeud)
   : NoeudAbstrait(typeNoeud), vitesse_(300.0),estControleParClavier_(false), joueur_(0), mMouseJoint(NULL)
{
    NoeudMaillet::mailletExistant++;
   
#if BOX2D_INTEGRATED
    if(!mMouseBody)
    {
        b2BodyDef bodyDef;
        mMouseBody = mWorld->CreateBody(&bodyDef);
    }
#endif


    for (int i = 0; i < NB_DIR ; i++)
    {
        direction_[i] = false;
    }
    velocite_.remetAZero();
    FacadeModele::getInstance()->ajouterElementSurTable(this);

    updatePhysicBody();

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet::~NoeudMaillet()
///
/// Ce destructeur d�sallouee la liste d'affichage du cube.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet::~NoeudMaillet()
{
	FacadeModele::getInstance()->supprimerElementSurTable(this);
	NoeudMaillet::mailletExistant--;

#if BOX2D_INTEGRATED
    checkf(!mMouseJoint, "Le mouse joint a mal ete liber�");
    destroyMouseJoint();
#endif


}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::afficherConcret(  )
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::afficherConcret() const
{
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Appel � la version de la classe de base pour l'affichage des enfants.
    NoeudAbstrait::afficherConcret();

    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();

#if BOX2D_INTEGRATED && BOX2D_DEBUG
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    if (mMouseJoint)
    {
        DebugRenderBox2D* debugRender = FacadeModele::getInstance()->getDebugRenderBox2D();
        if(debugRender)
        {
            b2Vec2 p1 = mMouseJoint->GetAnchorB();
            b2Vec2 p2 = mMouseJoint->GetTarget();

            b2Color c;
            c.Set(0.0f, 1.0f, 0.0f);
            debugRender->DrawPoint(p1, 4.0f, c);
            debugRender->DrawPoint(p2, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            debugRender->DrawSegment(p1, p2, c);
        }
    }
    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();
#endif

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
void NoeudMaillet::animer( const float& temps)
{
   // Appel � la version de la classe de base pour l'animation des enfants.
   NoeudAbstrait::animer(temps);
   

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudMaillet(this);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::gestionCollision( float temps )
///
/// Application des lois de la physique
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::gestionCollision( const float& temps )
{
	// Pour linstant on ne fait rien
	return;

	NoeudRondelle* rondelle;
	// Si le maillet n'est pas sur un table, il n'y a pas de physique appliquee
	if(!GetTerrain() || !( rondelle = GetTerrain()->getRondelle() ) )
		return;
	// Reinitialisation du vecteur d'enfoncement
	enfoncement_.remetAZero();
	collisionAvecRondelle_ = false;
	
	
	// On se cr�e un visiteur de collision
	VisiteurCollision v(this,false);
	rondelle->acceptVisitor(v);

	// Si il y a collision
	if(v.collisionPresente())
	{
		// On se cr�e un conteneur de noeud pour contenir les noeud en collision
		ConteneurNoeuds liste;
		v.obtenirListeCollision(liste);
		ConteneurNoeuds::iterator iter = liste.begin();

		for(; iter != liste.end(); iter++)
		{
			if( (*iter) == rondelle)
			{
				Vecteur3 dir = rondelle->obtenirPositionAbsolue() - obtenirPositionAbsolue();
				float enf = (rondelle->obtenirRayon()+obtenirRayon())-dir.norme();
				dir.normaliser();
				enfoncement_ += dir*enf;
				collisionAvecRondelle_ = true;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void setKeyboardControlled(bool clavier)
///
/// Permet d'indiquer au maillet s'il est controle par le clavier ou la souris
///
/// @param[in] bool clavier : bool�en pour indiquer qui controle ce maillet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::setKeyboardControlled(bool clavier)
{
	estControleParClavier_ = clavier;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::modifierDirection( bool active, DirectionMaillet dir )
///
/// Indique dans quelle direction le maillet ce deplace
///
/// @param[in] bool active
/// @param[in] DirectionMaillet dir
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::modifierDirection( bool active, DirectionMaillet dir )
{
	direction_[dir] = active;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::assignerPosSouris( Vecteur3 pos )
///
/// Assignation de la position de la souris pour que le maillet puisse la suivre
///
/// @param[in] Vecteur3 pos
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::assignerPosSouris( Vecteur3 pos )
{
	posSouris_ = pos;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::majPosition( float temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::majPosition( const float& temps )
{
	// Si le maillet n'est pas sur une table, il n'y a pas de physique appliquee
	if(!GetTerrain() || !GetTerrain()->getTable())
		return;
	anciennePos_ = positionRelative_;
	positionRelative_ += velocite_*temps;
	positionRelative_[VZ] = 0;


	NoeudGroupe* groupe = GetTerrain()->getTable()->obtenirGroupe(ArbreRenduINF2990::NOM_MURET);
	NoeudBut* but = GetTerrain()->getTable()->obtenirBut(anciennePos_[VX] < 0 ? 1:2);
	VisiteurCollision v(this,false);
	groupe->acceptVisitor(v);

	if(v.collisionPresente())
	{
		ConteneurDetailsCollision details = v.obtenirConteneurDetailsCollision();
		aidecollision::DetailsCollision detailsRes;
		detailsRes.enfoncement = 0;
		detailsRes.direction.remetAZero();
		for(unsigned int i=0; i < details.size(); ++i)
		{
			detailsRes.enfoncement += details[i].enfoncement;
			detailsRes.direction += details[i].direction;
		}
		detailsRes.direction.normaliser();
		positionRelative_ += detailsRes.direction*detailsRes.enfoncement;
		Vecteur3 normale(detailsRes.direction[VY],-detailsRes.direction[VX]);
		velocite_ = calculerProjectionDroite(velocite_,normale);
	}
	v.reinitialiser();

	but->acceptVisitor(v);
	if(v.collisionPresente())
	{
		ConteneurDetailsCollision details = v.obtenirConteneurDetailsCollision();
		aidecollision::DetailsCollision detailsRes;
		detailsRes.enfoncement = 0;
		detailsRes.direction.remetAZero();
		for(unsigned int i=0; i < details.size(); ++i)
		{
			detailsRes.enfoncement += details[i].enfoncement;
			detailsRes.direction += details[i].direction;
		}
		detailsRes.direction.normaliser();
		positionRelative_ += detailsRes.direction*detailsRes.enfoncement;
		Vecteur3 normale(detailsRes.direction[VY],-detailsRes.direction[VX]);
		velocite_ = calculerProjectionDroite(velocite_,normale);
	}
	if(estAGauche_)
	{
		if(positionRelative_[VX] + obtenirRayon() > 0)
		{
			positionRelative_[VX] = -obtenirRayon();
			velocite_[VX] = 0;
		}
	}
	else
	{
		if(positionRelative_[VX] - obtenirRayon() < 0)
		{
			positionRelative_[VX] = obtenirRayon();
			velocite_[VX] = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::ajusterEnfoncement()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::ajusterEnfoncement()
{
	// Si le maillet n'est pas sur une table, il n'y a pas de physique appliquee
	if(!GetTerrain() || !GetTerrain()->getTable())
		return;
	// il faudrait regarder si le maillet a ete mis en situation ou il est en collision avec qqc


	if(!GetTerrain()->getTable()->estSurTable(this) )
	{
		positionRelative_ = anciennePos_;
	}
	else
		positionRelative_ -= enfoncement_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::ajusterVitesse( float temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::ajusterVitesse( const float& temps )
{
	Vecteur2 direction;
	if(!estControleParOrdinateur_)
	{
		if(!estControleParClavier_)
		{
			direction = posSouris_-obtenirPositionAbsolue();
			// Pour arreter le maillet s'il est pres de la souris
			if(direction.norme2() <= 10)
			{
				direction.remetAZero();
				//velocite_.remetAZero();
				velocite_*=0.5;
			}
		}
		else
		{
			for (int i = 0; i < NB_DIR ; i++)
			{
				if(direction_[i])
				{
					switch(i)
					{
					case DIR_HAUT:
						direction[VY] += 1;
						//velocite_[VY] += vitesse_;
						break;
					case DIR_BAS:
						direction[VY] -= 1;
						//velocite_[VY] -= vitesse_;
						break;
					case DIR_GAUCHE:
						direction[VX] -= 1;
						//velocite_[VX] -= vitesse_;
						break;
					case DIR_DROITE:
						direction[VX] += 1;
						//velocite_[VX] += vitesse_;
						break;
					default:
						break;
					}
				}
			}
		}
		direction.normaliser();
		direction *= obtenirVitesse();
	}
	else
	{
		direction = joueur_->obtenirDirectionAI(this);
	}
	
	velocite_ += direction.convertir<3>();
	velocite_ *= 0.50;
	//velocite_[VZ] = 0;
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    clearPhysicsBody();

    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
    Vecteur3 pos = obtenirPositionAbsolue();
    b2Vec2 posB2;
    utilitaire::VEC3_TO_B2VEC(pos,posB2);
    myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
    myBodyDef.angle = 0; //set the starting angle
    myBodyDef.fixedRotation = true;

    mPhysicBody = getWorld()->CreateBody(&myBodyDef);
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = (float32)obtenirRayon()*utilitaire::ratioWorldToBox2D; //radius

    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
    myFixtureDef.density = 0.02f;
    myFixtureDef.filter.categoryBits = CATEGORY_MALLET;
    myFixtureDef.filter.maskBits = CATEGORY_PUCK | CATEGORY_BOUNDARY;
    myFixtureDef.filter.groupIndex = 1;

    mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
    mPhysicBody->SetUserData(this);
    mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::buildMouseJoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::buildMouseJoint()
{
#if BOX2D_INTEGRATED
    if(!mMouseJoint)
    {
        if(!estControleParOrdinateur_ && !estControleParClavier_)
        {
            GestionnaireEvenements::obtenirInstance()->attach(this);
        }

        b2Body* body = getPhysicBody();
        b2MouseJointDef md;
        md.bodyA = mMouseBody;
        md.bodyB = body;
        Vecteur3 pos = obtenirPositionAbsolue();
        utilitaire::VEC3_TO_B2VEC(pos,md.target);
        md.maxForce = 10000.0f * body->GetMass();
        md.dampingRatio = 0;
        md.frequencyHz = 100;
        mMouseJoint = (b2MouseJoint*)mWorld->CreateJoint(&md);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::destroyMouseJoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::destroyMouseJoint()
{
#if BOX2D_INTEGRATED
    if(mMouseJoint)
    {
        GestionnaireEvenements::obtenirInstance()->detach(this);
        mWorld->DestroyJoint(mMouseJoint);
        mMouseJoint = NULL;
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::updateObserver( class MouseMoveSubject& pSubject )
///
/// /*Description*/
///
/// @param[in] class MouseMoveSubject & pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::updateObserver( MouseMoveSubject* pSubject )
{
#if BOX2D_INTEGRATED
    if(mMouseJoint)
    {
        Vecteur3 virtualMousePos;
        b2Vec2 virtualMousePosB2;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(pSubject->getEvent().obtenirPosition()[VX],pSubject->getEvent().obtenirPosition()[VY],virtualMousePos);
        utilitaire::VEC3_TO_B2VEC(virtualMousePos,virtualMousePosB2);
        mMouseJoint->SetTarget(virtualMousePosB2);
    }
#endif
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::preSimulationActions()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::preSimulationActions()
{
#if BOX2D_INTEGRATED  
    Vecteur2 direction(0,0);
    if(estControleParClavier_ || estControleParOrdinateur_)
    {
        if(!estControleParOrdinateur_)
        {
            if(estControleParClavier_)
            {
                for (int i = 0; i < NB_DIR ; i++)
                {
                    if(direction_[i])
                    {
                        switch(i)
                        {
                        case DIR_HAUT:
                            direction[VY] += 1;
                            break;
                        case DIR_BAS:
                            direction[VY] -= 1;
                            break;
                        case DIR_GAUCHE:
                            direction[VX] -= 1;
                            break;
                        case DIR_DROITE:
                            direction[VX] += 1;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            direction.normaliser();
            direction *= obtenirVitesse();
        }
        else
        {
            direction = joueur_->obtenirDirectionAI(this);
        }

        direction.normaliser();
        direction *= 10;
        direction += obtenirPositionAbsolue();

        b2Vec2 velocite;
        utilitaire::VEC3_TO_B2VEC(direction,velocite);
        if(mMouseJoint)
        {
            mMouseJoint->SetTarget(velocite);
        }
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
