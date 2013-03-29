//
//  Model3DManager.h
//  Hockedu
//
//  Created by Michael Ferris on 2013-03-26.
//
//

#import <Foundation/Foundation.h>
#import "Enum_Declarations.h"


@interface Model3DManager : NSObject
{
    NSMutableDictionary* mModels;
}

- (id)init;
- (void)AddObject:(NSString*)name :(RazerKey)key :(void*)expectedSize;
- (bool)renderObject:(RazerKey)type;

@end