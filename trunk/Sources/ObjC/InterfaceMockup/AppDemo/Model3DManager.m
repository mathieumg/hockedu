//
//  Model3DManager.m
//  Hockedu
//
//  Created by Michael Ferris on 2013-03-26.
//
//

#import "Model3DManager.h"

#import "OpenGLWaveFrontObject.h"

@interface Model3DManager ()
@property (nonatomic, retain) NSMutableDictionary *mModels;
@end

@implementation Model3DManager

@synthesize mModels;

-(void)AddObject:(NSString*)name :(RazerKey)key :(void*)expectedSize
{
    NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:@"obj"];
	OpenGLWaveFrontObject *theObject = [[OpenGLWaveFrontObject alloc] initWithPath:path];
	Vertex3D position = Vertex3DMake(0.0, 0.0, 0.0);
	theObject.currentPosition = position;
    [theObject calculateScale: *(Vertex3D*)expectedSize];
    
    [mModels setObject:theObject forKey:[NSNumber numberWithInt:key]];

	[theObject release];
}


-(id)init
{
    self.mModels = [NSMutableDictionary dictionary];
    Vertex3D expectedSize = Vertex3DMake(10, 10, 2);
  /*  //[self AddObject:@"portail":RAZER_KEY_PORTAL:&expectedSize];
    
    expectedSize = Vertex3DMake(14, 14, 20);
    //[self AddObject:@"accelerateur":RAZER_KEY_BOOST:&expectedSize];
    
    expectedSize = Vertex3DMake(30, 10, 5);
    //[self AddObject:@"but_milieu":RAZER_KEY_GOAL:&expectedSize];
    
    expectedSize = Vertex3DMake(15, 15, 1);
    //[self AddObject:@"EmptyBonus":RAZER_KEY_BONUS:&expectedSize];
    
    expectedSize = Vertex3DMake(20, 20, 20);
    [self AddObject:@"maillet":RAZER_KEY_MALLET:&expectedSize];
    
    expectedSize = Vertex3DMake(1, 5, 15);
    [self AddObject:@"muret":RAZER_KEY_WALL:&expectedSize];
    
    expectedSize = Vertex3DMake(16, 16, 2);
    [self AddObject:@"rondelle":RAZER_KEY_PUCK:&expectedSize];
    
    expectedSize = Vertex3DMake(10, 10, 4);
    [self AddObject:@"control_point":RAZER_KEY_CONTROL_POINT:&expectedSize];
    
    expectedSize = Vertex3DMake(20, 20, 20);
    //[self AddObject:@"point":RAZER_KEY_TABLE_CONTROL_POINT:&expectedSize];
*/
    return self;
}


- (bool)renderObject:(RazerKey)type
{
    return false;
    OpenGLWaveFrontObject *theObject = [mModels objectForKey:[NSNumber numberWithInt:type]];
    if(theObject != nil)
    {
        [theObject drawSelf];
        return true;
    }
    return false;
}


@end
