//
//  Model.m
//  Hockedu
//
//  Created by Mathieu Parent on 2013-03-07.
//
//

#import "Model.h"
#include "Terrain.h"
#include "VisiteurSelection.h"
#include "NoeudAbstrait.h"
#include <Box2D/Box2D.h>

@implementation Model

- (void)render
{
    ((Terrain*)mField)->renderField();

}
- (id)init
{
    b2World* world = new b2World(b2Vec2(0,0));
    mField = new Terrain(NULL);
    ((Terrain*)mField)->createRandomField("test");
    return self;
}

-(void)acceptSelectionVisitor:(float)positionMinX: (float)positionMinY:(float) positionMaxX:(float) positionMaxY
{
    Vecteur2 posMin = Vecteur2(positionMinX,positionMinY);
    Vecteur2 posMax = Vecteur2(positionMaxX,positionMaxY);
    VisiteurSelection visitor = VisiteurSelection(posMin,posMax);
    ((Terrain*)mField)->acceptVisitor(visitor);
    visitor.faireSelection();
    
}

@end
