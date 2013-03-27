//
//  Model.h
//  Hockedu
//
//  Created by Mathieu Parent on 2013-03-07.
//
//

#import <Foundation/Foundation.h>
#import "Enum_Declarations.h"
@interface Model : NSObject
{

    void* mField;
    
}

- (void)render;
- (id)init;
- (void) dealloc;
-(int)acceptSelectionVisitor:(float)positionMinX: (float)positionMinY:(float) positionMaxX:(float) positionMaxY;
-(void) saveField;
-(void) beginModification:(FieldModificationStrategyType)type:(CGPoint)coordVirt;
-(void) eventModification:(FieldModificationStrategyEventType)type:(CGPoint)coordVirt;
-(void) eventCancel;
@end