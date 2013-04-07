//
//  Model.h
//  Hockedu
//
//  Created by Mathieu Parent on 2013-03-07.
//
//

#import <Foundation/Foundation.h>
#import "Enum_Declarations.h"
#import "Model3DManager.h"
@interface Model : NSObject
{

    void* mField;
    Model3DManager* mModel3DManager;
}

- (void)render;
- (id)init;
- (void) dealloc;
-(int)acceptSelectionVisitor:(float)positionMinX :(float)positionMinY :(float)positionMaxX :(float) positionMaxY;
-(void) saveField;
-(void) beginModification:(FieldModificationStrategyType)type :(CGPoint)coordVirt;
-(void) eventModification:(FieldModificationStrategyEventType)type :(CGPoint)coordVirt;
-(void) eventCancel;
-(void) endModification;
-(void) duplicateSelection;
-(void) deleteSelection;
-(void) undo;
-(void) redo;
-(void) zoom:(float)scale;
-(void) deplacerSouris:(int)deplacementX :(int)deplacementY;
-(void) orbit:(int)deplacementX :(int)deplacementY;
-(void) resizeWindow:(int)minX :(int)minY :(int)maxX :(int)maxY;
-(void*) getProperties:(void*)fullProp;
-(void) setProperties:(void*)fullProp;
-(RazerKey) getSelectedNodesType;
@end