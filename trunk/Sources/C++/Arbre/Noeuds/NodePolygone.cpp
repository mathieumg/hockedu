///////////////////////////////////////////////////////////////////////////////
/// @file NodePolygone.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NodePolygone.h"
#include "NodeControlPoint.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodePolygone::NodePolygone( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodePolygone::NodePolygone( const std::string& type ):
    Super(type),ControlPointMutableAbstract(8)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodePolygone::~NodePolygone()
///
/// destructor
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodePolygone::~NodePolygone()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::ajouter( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant.
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodePolygone::ajouter( NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = dynamic_cast<NodeControlPoint*>(enfant);
    if(controlPoint)
    {
        return addControlPoint(controlPoint);
    }
    return Super::ajouter(enfant);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::detacherEnfant( const NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] const NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::detacherEnfant( const NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = const_cast<NodeControlPoint*>(dynamic_cast<const NodeControlPoint*>(enfant));
    if(controlPoint)
    {
        removeControlPoint(controlPoint);
    }
    else
    {
        Super::detacherEnfant(enfant);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodePolygone::modifierEchelle( float facteur )
///
/// /*Description*/
///
/// @param[in] float facteur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::modifierEchelle( float facteur )
{
//     Vecteur3 echelle;
//     obtenirEchelleCourante(echelle);
//     echelle[VY] *= facteur;
//     modifierEchelleCourante(echelle);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodePolygone::afficherConcret()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::afficherConcret() const
{
//     const Vecteur3& corner1 = obtenirCoin1(), corner2 = obtenirCoin2();
//     Vecteur2 vecteurEntre(corner1-corner2);
//     vecteurEntre = vecteurEntre.tournerMoinsPiSur2();
//     vecteurEntre.normaliser();
//     float halfHeight = echelleCourante_[VY]*DEFAULT_SIZE[VY]/2.f;
//     vecteurEntre *= halfHeight;
//     glPushMatrix();
//     glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);
//     // Assignation du mode d'affichage des polygones
//     glPolygonMode( GL_FRONT_AND_BACK, modePolygones_ );
//     glTranslatef(vecteurEntre[VX],vecteurEntre[VY],0);
//     NoeudAbstrait::afficherConcret();
// 
//     // Restauration
//     glPopAttrib();
//     glPopMatrix();

    DrawChild();
//     vecteurEntre.tourner(90,Vecteur3(0,0,1));
//     vecteurEntre.
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodePolygone::updateObserver( PositionSubject* pSubject )
///
/// /*Description*/
///
/// @param[in] PositionSubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::updateObserver( PositionSubject* pSubject )
{
    updatePhysicBody();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodePolygone::updatePhysicBody()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::updatePhysicBody()
{
#if BOX2D_INTEGRATED

    if(getWorld() && getNBControlPoint() > 2)
    {
        clearPhysicsBody();

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.position.Set(0, 0); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = getWorld()->CreateBody(&myBodyDef);
        b2PolygonShape shape;
        std::vector<Vecteur2> vertices,vertices2;
        vertices.resize(getNBControlPoint());
        for(unsigned int i=0; i<getNBControlPoint(); ++i)
        {
            vertices[i] = getControlPoint(i)->getPosition();
            //utilitaire::VEC3_TO_B2VEC(getControlPoint(i)->getPosition(),vertices[i]);
        }
        auto it = vertices.begin();
        auto itUp = it;
        auto upper = *it++;
        for(;it != vertices.end(); it++) {
            auto temp = *it;
            if(temp[1] > upper[1] || (temp[1] == upper[1] && temp[0] < upper[0])) 
            {
                itUp = it;
                upper = temp;
            }
        }
        vertices2.push_back(*itUp);
        vertices.erase(itUp);

        Vecteur2 d1(-1,0),d2(0,-1); 
        while(!vertices.empty())
        {
            auto upper = vertices2.back();
            auto it = vertices.begin();
            auto itUp = vertices.end();

            float curMax=0;
            float distance = 0;
            for(;it != vertices.end(); it++)
            {
                auto temp = (*it-upper);
                temp.normaliser();
                float b = produitScalaire(temp,d2);

                if(b>=0)
                {
                    float a = acos(produitScalaire(temp,d1));
                    if(a >= curMax)
                    {
                        float l = (*it-upper).norme2();
                        if(a != curMax || (l > distance))
                        {
                            d1 = temp;
                            d2 = d1.tournerPiSur2();
                            curMax = a;
                            distance = l;
                            upper = *it;
                            itUp = it;
                        }
                    }
                }
            }

            if(itUp == vertices.end())
            {
                break;
            }
            else
            {
                vertices2.push_back(*itUp);
                vertices.erase(itUp);
            }
        }
        b2Vec2* v = new b2Vec2[vertices2.size()];
        for(unsigned int i=0; i<vertices2.size(); ++i)
        {
            utilitaire::VEC3_TO_B2VEC(vertices2[i],v[i]);
        }
        if(vertices2.size() > 2)
        {
            shape.Set(v,(int)vertices2.size());


            b2FixtureDef myFixtureDef;
            myFixtureDef.shape = &shape; //this is a pointer to the shape above
            myFixtureDef.density = 1;
            myFixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
            myFixtureDef.filter.maskBits = CATEGORY_PUCK | CATEGORY_MALLET;

            mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        }
        delete[] v;
        //     mPhysicBody->SetUserData(this);
        //     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodePolygone::onAddControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodePolygone::onAddControlPoint( NodeControlPoint* point )
{
    if(Super::ajouter(point))
    {
        point->attach(this);
        updatePhysicBody();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodePolygone::onRemoveControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::onRemoveControlPoint( NodeControlPoint* point )
{
    Super::detacherEnfant(point);
    point->detach(this);
    updatePhysicBody();
}




#ifdef MIKE_BUILD
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////