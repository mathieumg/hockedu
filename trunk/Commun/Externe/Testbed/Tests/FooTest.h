#ifndef FOOTEST_H
#define FOOTEST_H

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#include <time.h>

class FooTest : public Test
{
public:
    //in the class itself, not inside the constructor!
    b2Body* dynamicBody;

    FooTest() {
//         b2BodyDef myBodyDef;
//         myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
//         myBodyDef.position.Set(0, 20); //set the starting position
//         myBodyDef.angle = 0; //set the starting angle
// 
//         dynamicBody = m_world->CreateBody(&myBodyDef);
//         b2Body* dynamicBody1 = m_world->CreateBody(&myBodyDef);
// 
//         b2PolygonShape boxShape;
//         boxShape.SetAsBox(1,1);
//         b2FixtureDef boxFixtureDef;
//         boxFixtureDef.shape = &boxShape;
//         boxFixtureDef.density = 1;
//         dynamicBody->CreateFixture(&boxFixtureDef);
//         dynamicBody->SetTransform( b2Vec2( 10, 20 ), 1 );
//         dynamicBody->SetTransform( b2Vec2( 10, 20 ), 45 * DEGTORAD ); //45 degrees counter-clockwise
//         dynamicBody->SetLinearVelocity( b2Vec2( -5, 5 ) ); //moving up and left 5 units per second
//         dynamicBody->SetAngularVelocity( -90 * DEGTORAD ); //90 degrees per second clockwise
// 
//         b2CircleShape circleShape;
//         circleShape.m_p.Set(-10, 0); //position, relative to body position
//         circleShape.m_radius = 1; //radius
//         b2FixtureDef myFixtureDef;
//         myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
//         dynamicBody1->CreateFixture(&myFixtureDef); //add a fixture to the body
//         myFixtureDef.density = 1;
// 
//         //set each vertex of polygon in an array
//         b2Vec2 vertices[5];
//         vertices[0].Set(-1+10,  2);
//         vertices[1].Set(-1+10,  0);
//         vertices[2].Set( 0+10, -3);
//         vertices[3].Set( 1+10,  0);
//         vertices[4].Set( 1+10,  1);
// 
//         b2PolygonShape polygonShape;
//         polygonShape.Set(vertices, 5); //pass array to the shape
// 
//         myFixtureDef.shape = &polygonShape; //change the shape of the fixture
//         myBodyDef.position.Set(0, 20); //in the middle
//         b2Body* dynamicBody2 = m_world->CreateBody(&myBodyDef);
//         dynamicBody1->CreateFixture(&myFixtureDef); //add a fixture to the body
// 
// 
//         myBodyDef.type = b2_staticBody; //this will be a static body
//         myBodyDef.position.Set(0, 10); //slightly lower position
//         b2Body* floor = m_world->CreateBody(&myBodyDef); //add body to world
//         floor->CreateFixture(&boxFixtureDef); //add fixture to body
// 
//         myBodyDef.type = b2_kinematicBody; //this will be a kinematic body
//         myBodyDef.position.Set(-18, 11); // start from left side, slightly above the static body
//         b2Body* kinematicBody = m_world->CreateBody(&myBodyDef); //add body to world
//         kinematicBody->CreateFixture(&boxFixtureDef); //add fixture to body
// 
//         kinematicBody->SetLinearVelocity( b2Vec2( 1, 0 ) ); //move right 1 unit per second
//         kinematicBody->SetAngularVelocity( 360 * DEGTORAD ); //1 turn per second counter-clockwise
// 
//         polygonShape.SetAsBox(2, 1); //a 4x2 rectangle
//         myBodyDef.position.Set(10,20); //a bit to the right
// 
//         b2Body* dynamicBody3 = m_world->CreateBody(&myBodyDef);
//         dynamicBody1->CreateFixture(&myFixtureDef); //add a fixture to the body
// 
//         myBodyDef.type = b2_staticBody; //change body type
//         myBodyDef.position.Set(0,0); //middle, bottom
// 
//         b2EdgeShape edgeShape;
//         edgeShape.Set( b2Vec2(-15,0), b2Vec2(15,0) ); //ends of the line
//         myFixtureDef.shape = &edgeShape;
//         b2Body* staticBody2 = m_world->CreateBody(&myBodyDef);
//         staticBody2->CreateFixture(&myFixtureDef); //add a fixture to the body








        //set up a dynamic body
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;
        myBodyDef.position.Set(0, 20); //middle
        b2Body* dynamicBody = m_world->CreateBody(&myBodyDef);

        //prepare a shape definition
        b2PolygonShape polygonShape;
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 1;

        //add four square shaped fixtures around the body center
        for ( int i = 0; i < 4; i++) {
            b2Vec2 pos( sinf(i*90*DEGTORAD), cosf(i*90*DEGTORAD) ); //radial placement
            polygonShape.SetAsBox(1, 1, pos, 0 ); //a 2x2 rectangle
            myFixtureDef.friction = i/4.0;
            myFixtureDef.restitution = 0.5f;
            dynamicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        }

        //make a static floor to drop things on
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0); //middle, bottom
        floor = m_world->CreateBody(&myBodyDef);  
        b2EdgeShape edgeShape;
        myFixtureDef.shape = &edgeShape;
        edgeShape.Set( b2Vec2(-15,0), b2Vec2(15,3) ); //slightly sloped  
        floor->CreateFixture(&myFixtureDef); //add a fixture to the body

    } //do nothing, no scene yet

    b2Body* floor;

    void Step(Settings* settings)
    {
        //run the default physics and rendering
        Test::Step(settings); 

        //show some text in the main screen
        m_debugDraw.DrawString(5, m_textLine, "Now we have a foo test");
        m_textLine += 15;
        static int t = clock();
        if(t + 1000 < clock())
        {
            //floor->SetTransform(floor->GetPosition(),floor->GetAngle()-1*DEGTORAD);
            t = clock();
        }

//         b2Vec2 pos = dynamicBody->GetPosition();
//         float angle = dynamicBody->GetAngle();
//         b2Vec2 vel = dynamicBody->GetLinearVelocity();
//         float angularVel = dynamicBody->GetAngularVelocity();
//         m_debugDraw.DrawString(5, m_textLine, 
//             "Position:%.3f,%.3f Angle:%.3f", pos.x, pos.y, angle * RADTODEG);
//         m_textLine += 15;
//         m_debugDraw.DrawString(5, m_textLine, 
//             "Velocity:%.3f,%.3f Angular velocity:%.3f", vel.x, vel.y, angularVel * RADTODEG);
//         m_textLine += 15;

        //dynamicBody->SetTransform( dynamicBody->GetPosition()*1.1f, dynamicBody->GetAngle()*1.1f );
    }

    static Test* Create()
    {
        return new FooTest;
    }
};

#endif