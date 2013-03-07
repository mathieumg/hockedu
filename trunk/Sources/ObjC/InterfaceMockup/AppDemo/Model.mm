//
//  Model.m
//  Hockedu
//
//  Created by Mathieu Parent on 2013-03-07.
//
//

#import "Model.h"
#include "Terrain.h"
@implementation Model

- (void)render
{
    ((Terrain*)mField)->renderField();

}
- (id)init
{
    mField = new Terrain(NULL);
    ((Terrain*)mField)->creerTerrainParDefaut("test");
    return self;
}

@end
