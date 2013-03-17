//
//  Model.h
//  Hockedu
//
//  Created by Mathieu Parent on 2013-03-07.
//
//

#import <Foundation/Foundation.h>
@interface Model : NSObject
{

void* mField;

}

- (void)render;
- (id)init;
-(void)acceptSelectionVisitor:(float)positionMinX: (float)positionMinY:(float) positionMaxX:(float) positionMaxY;
@end