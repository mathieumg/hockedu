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
@property (nonatomic, retain) NSMutableArray *mModels;
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
    
    [mModels setObject:theObject atIndexedSubscript:key];
	[theObject release];
}


-(id)init
{    
    self.mModels = [NSMutableArray arrayWithCapacity:NB_RAZER_KEYS];
    for(int i=0; i<NB_RAZER_KEYS; ++i)
    {
        [mModels addObject:[NSNull null]];
    }
    
    Vertex3D expectedSize = Vertex3DMake(20, 20, 1);
    [self AddObject:@"portal":RAZER_KEY_PORTAL:&expectedSize];
    
    expectedSize = Vertex3DMake(14, 14, 2);
    [self AddObject:@"boost":RAZER_KEY_BOOST:&expectedSize];
    
    expectedSize = Vertex3DMake(30, 10, 5);
    [self AddObject:@"but":RAZER_KEY_GOAL:&expectedSize];
    
    expectedSize = Vertex3DMake(20, 20, 20);
    [self AddObject:@"maillet":RAZER_KEY_MALLET:&expectedSize];
    
    expectedSize = Vertex3DMake(1, 5, 15);
    [self AddObject:@"muret":RAZER_KEY_WALL:&expectedSize];
    
    expectedSize = Vertex3DMake(16, 16, 2);
    [self AddObject:@"rondelle":RAZER_KEY_PUCK:&expectedSize];
    
    expectedSize = Vertex3DMake(10, 10, 4);
    [self AddObject:@"control_point":RAZER_KEY_CONTROL_POINT:&expectedSize];
    
    expectedSize = Vertex3DMake(20, 20, 6);
    [self AddObject:@"control_point":RAZER_KEY_TABLE_CONTROL_POINT:&expectedSize];

    return self;
}


- (bool)renderObject:(RazerKey)type
{
    id theObject = [mModels objectAtIndex:type];
    
    bool ret = false;
    if(theObject != nil && theObject != [NSNull null])
    {
        OpenGLWaveFrontObject * model = theObject;
        [model drawSelf];
        ret = true;
    }
    
    //
    return ret;
}


@end
