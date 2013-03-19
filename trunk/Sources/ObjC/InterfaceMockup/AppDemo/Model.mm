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
@implementation Model

- (void)render
{
    ((Terrain*)mField)->renderField();

}
- (id)init
{
    mField = new Terrain(NULL);
    ((Terrain*)mField)->createRandomField("test");
    return self;
}

-(void)acceptSelectionVisitor:(float)positionMinX: (float)positionMinY:(float) positionMaxX:(float) positionMaxY
{
    Vecteur2 posMin = Vecteur2(positionMinX,positionMinY);
    Vecteur2 posMax = Vecteur2(positionMaxX,positionMaxY);
    VisiteurSelection visitor = VisiteurSelection(posMin,posMax);
    ((Terrain*)mField)->setTableItemsSelection(false);
    ((Terrain*)mField)->acceptVisitor(visitor);
    visitor.faireSelection();
    
}

@end
