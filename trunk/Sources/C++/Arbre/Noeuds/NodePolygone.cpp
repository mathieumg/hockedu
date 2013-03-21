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

template<typename T>
T sign(T val)
{
    if (val == 0)
    {
        return 0;
    }
    return (T)(val > 0 ? 1 : -1);
}

#if MIKE_DEBUG_
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
/// @fn bool NodeWallEdition::add( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant.
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodePolygone::add( NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = dynamic_cast<NodeControlPoint*>(enfant);
    if(controlPoint)
    {
        return addControlPoint(controlPoint);
    }
    return Super::add(enfant);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::unlinkChild( const NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] const NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::unlinkChild( const NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = const_cast<NodeControlPoint*>(dynamic_cast<const NodeControlPoint*>(enfant));
    if(controlPoint)
    {
        removeControlPoint(controlPoint);
    }
    else
    {
        Super::unlinkChild(enfant);
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
/// @fn void NodePolygone::renderReal()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::renderReal() const
{
#if WIN32
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);
    // Assignation du mode d'affichage des polygones
    glPolygonMode( GL_FRONT_AND_BACK, mModePolygones );
    glTranslatef(0,0,5);

    divideIntoTriangles();

    // Restauration
    glPopAttrib();
    glPopMatrix();
#endif

    DrawChild();
//     vecteurEntre.tourner(90,Vecteur3(0,0,1));
//     vecteurEntre.
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodePolygone::updateObserver( const  PositionSubject* pSubject )
///
/// /*Description*/
///
/// @param[in] PositionSubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodePolygone::updateObserver( const  PositionSubject* pSubject )
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
#if BOX2D_INTEGRATED && 0

    auto world = getWorld();
    if(world && getNBControlPoint() > 2)
    {
        clearPhysicsBody();

        b2BodyDef myBodyDef;
        myBodyDef.type = IsInGame() ? b2_staticBody : b2_dynamicBody;; //this will be a dynamic body
        myBodyDef.position.Set(0, 0); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = world->CreateBody(&myBodyDef);
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
            if(IsInGame())
            {
                myFixtureDef.filter.categoryBits = CATEGORY_WALL;
                myFixtureDef.filter.maskBits = CATEGORY_PUCK | CATEGORY_MALLET;
            }

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
    if(Super::add(point))
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
    Super::unlinkChild(point);
    point->detach(this);
    updatePhysicBody();
}


void NodePolygone::divideIntoTriangles() const
{
#if WIN32
    if(getNBControlPoint() < 3)
        return ;
    std::vector<Vecteur2> my_polygon;
    
    for(unsigned int i=0; i<getNBControlPoint(); ++i)
    {
        Vecteur2 pos = getControlPoint(i)->getPosition();
        bool doAdd = true;
        for(int j=0; j<my_polygon.size(); ++j)
        {
            auto a = my_polygon[j]-pos;
            if(a.norme2() < 5)
            {
                doAdd = false;
                break;
            }
        }
        if(doAdd)
        {
            my_polygon.push_back(pos);
        }
    }
    // from wiki (http://en.wikipedia.org/wiki/File:Polygon-to-monotone.png)

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(6);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    for(size_t i = 0, n = my_polygon.size(); i < n; ++ i)
        glVertex2f(my_polygon[i][VX], my_polygon[i][VY]);
    glEnd();
    glPointSize(6);
    glBegin(GL_POINTS);
    for(size_t i = 0, n = my_polygon.size(); i < n; ++ i)
        glVertex2f(my_polygon[i][VX], my_polygon[i][VY]);
    glEnd();
    // draw the original polygon

    std::vector<int> working_set;
    for(int i = 0, n = (int)my_polygon.size(); i < n; ++ i)
        working_set.push_back(i);
    _ASSERTE(working_set.size() == my_polygon.size());
    // add vertex indices to the list (could be done using iota)

    std::list<std::vector<int> > monotone_poly_list;
    // list of monotone polygons (the output)

    glPointSize(14);
    glLineWidth(4);
    // prepare to draw split points and slice lines

    for(;;) {
        std::vector<int> sorted_vertex_list;
        {
            for(int i = 0, n = (int)working_set.size(); i < n; ++ i)
                sorted_vertex_list.push_back(i);
            _ASSERTE(working_set.size() == working_set.size());
            // add vertex indices to the list (could be done using iota)

            for(;;) {
                bool b_change = false;

                for(int i = 1, n = (int)sorted_vertex_list.size(); i < n; ++ i) {
                    int a = sorted_vertex_list[i - 1];
                    int b = sorted_vertex_list[i];
                    if(my_polygon[working_set[a]][VY] < my_polygon[working_set[b]][VY]) {
                        std::swap(sorted_vertex_list[i - 1], sorted_vertex_list[i]);
                        b_change = true;
                    }
                }

                if(!b_change)
                    break;
            }
            // sort vertex indices by the y coordinate
            // (note this is using bubblesort to maintain portability
            // but it should be done using a better sorting method)
        }
        // build sorted vertex list

        bool b_change = false;
        for(size_t i = 0, n = sorted_vertex_list.size(), m = working_set.size(); i < n; ++ i) {
            int n_ith = sorted_vertex_list[i];
            Vecteur2 ith = my_polygon[working_set[n_ith]];
            Vecteur2 prev = my_polygon[working_set[(n_ith + m - 1) % m]];
            Vecteur2 next = my_polygon[working_set[(n_ith + 1) % m]];
            // get point in the list, and get it's neighbours
            // (neighbours are not in sorted list ordering
            // but in the original polygon order)

            float sidePrev = sign(ith[VY] - prev[VY]);
            float sideNext = sign(ith[VY] - next[VY]);
            // calculate which side they lie on
            // (sign function gives -1 for negative and 1 for positive argument)

            if(sidePrev * sideNext >= 0) { // if they are both on the same side
                glColor3f(1, 0, 0);
                glBegin(GL_POINTS);
                glVertex2f(ith[VX], ith[VY]);
                glEnd();
                // marks points whose neighbours are both on the same side (split points)

                int n_next = -1;
                if(sidePrev + sideNext > 0) {
                    if(i > 0)
                        n_next = sorted_vertex_list[i - 1];
                    // get the next vertex above it
                } else {
                    if(i + 1 < n)
                        n_next = sorted_vertex_list[i + 1];
                    // get the next vertex below it
                }
                // this is kind of simplistic, one needs to check if
                // a line between n_ith and n_next doesn't exit the polygon
                // (but that doesn't happen in the example)

                if(n_next != -1) {
                    glColor3f(0, 1, 0);
                    glBegin(GL_POINTS);
                    glVertex2f(my_polygon[working_set[n_next]][VX], my_polygon[working_set[n_next]][VY]);
                    glEnd();
                    glBegin(GL_LINES);
                    glVertex2f(ith[VX], ith[VY]);
                    glVertex2f(my_polygon[working_set[n_next]][VX], my_polygon[working_set[n_next]][VY]);
                    glEnd();

                    std::vector<int> poly, remove_list;

                    int n_last = n_ith;
                    if(n_last > n_next)
                        std::swap(n_last, n_next);
                    int idx = n_next;
                    poly.push_back(working_set[idx]); // add n_next
                    for(idx = (idx + 1) % m; idx != n_last; idx = (idx + 1) % m) {
                        poly.push_back(working_set[idx]);
                        // add it to the polygon

                        remove_list.push_back(idx);
                        // mark this vertex to be erased from the working set
                    }
                    poly.push_back(working_set[idx]); // add n_ith
                    // build a new monotone polygon by cutting the original one

                    std::sort(remove_list.begin(), remove_list.end());
                    for(size_t i = remove_list.size(); i > 0; -- i) {
                        int n_which = remove_list[i - 1];
                        working_set.erase(working_set.begin() + n_which);
                    }
                    // sort indices of vertices to be removed and remove them
                    // from the working set (have to do it in reverse order)

                    monotone_poly_list.push_back(poly);
                    // add it to the list

                    b_change = true;

                    break;
                    // the polygon was sliced, restart the algorithm, regenerate sorted list and slice again
                }
            }
        }

        if(!b_change)
            break;
        // no moves
    }

    if(!working_set.empty())
        monotone_poly_list.push_back(working_set);
    // use the remaining vertices (which the algorithm was unable to slice) as the last polygon

    std::list<std::vector<int> >::const_iterator p_mono_poly = monotone_poly_list.begin();
    for(; p_mono_poly != monotone_poly_list.end(); ++ p_mono_poly) {
        const std::vector<int> &r_mono_poly = *p_mono_poly;

        glLineWidth(2);
        glColor3f(0, 0, 1);
        glBegin(GL_LINE_LOOP);
        for(size_t i = 0, n = r_mono_poly.size(); i < n; ++ i)
            glVertex2f(my_polygon[r_mono_poly[i]][VX], my_polygon[r_mono_poly[i]][VY]);
        glEnd();
        glPointSize(2);
        glBegin(GL_POINTS);
        for(size_t i = 0, n = r_mono_poly.size(); i < n; ++ i)
            glVertex2f(my_polygon[r_mono_poly[i]][VX], my_polygon[r_mono_poly[i]][VY]);
        glEnd();
        // draw the sliced part of the polygon

        int n_top = 0;
        for(int i = 0, n = (int)r_mono_poly.size(); i < n; ++ i) {
            if(my_polygon[r_mono_poly[i]][VY] < my_polygon[r_mono_poly[n_top]][VY])
                n_top = i;
        }
        // find the top-most point

        glLineWidth(1);
        glColor3f(0, 1, 0);
        glBegin(GL_LINE_STRIP);
        glVertex2f(my_polygon[r_mono_poly[n_top]][VX], my_polygon[r_mono_poly[n_top]][VY]);
        for(int i = 1, n = (int)r_mono_poly.size(); i <= n; ++ i) {
            int n_which = (n_top + ((i & 1)? n - i / 2 : i / 2)) % n;
            glVertex2f(my_polygon[r_mono_poly[n_which]][VX], my_polygon[r_mono_poly[n_which]][VY]);
        }
        glEnd();
        // draw as if triangle strip
    }
#endif
}



#if MIKE_DEBUG_
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////