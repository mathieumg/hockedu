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
//@interface FullPropertiesApple : NSObject{
//@public
typedef struct{
    float mFriction;
    float mZoneEditionX;
    float mZoneEditionY;
    float mScale;
    float mAcceleration;
    float mPositionX;
    float mPositionY;
    float mAttraction;
    float mAngle;
    float mRebound;
    float mMinBonusSpawnTime;
    float mMaxBonusSpawnTime;
    float mRinkRebound1;
    float mRinkRebound2;
    float mRinkRebound3;
    float mRinkRebound4;
    float mRinkRebound5;
    float mRinkRebound6;
    float mRinkRebound7;
    float mRinkRebound8;
    //BonusProperties* mBonusProperties;
    int mPropertyFlagAssignment;
    
}FullPropertiesApple;
//@property (retain, nonatomic) float mFriction;
//@end

@interface Model : NSObject
{

    void* mField;
    Model3DManager* mModel3DManager;
    

}

- (void)render;
- (id)init;
- (void) dealloc;
-(int)acceptSelectionVisitor:(float)positionMinX :(float)positionMinY :(float)positionMaxX :(float) positionMaxY;
+(void) loadField:(NSString*) stringToParse;
-(void) saveField;
-(int) beginModification:(FieldModificationStrategyType)type :(CGPoint)coordVirt;
-(int) eventModification:(FieldModificationStrategyEventType)type :(CGPoint)coordVirt;
-(void) eventCancel;
-(int) endModification;
-(void) duplicateSelection;
-(void) deleteSelection;
-(void) undo;
-(void) redo;
-(void) zoom:(float)scale;
-(void) deplacerSouris:(int)deplacementX :(int)deplacementY;
-(void) createCameraFixed;
-(void) createCameraOrbit;
-(void) createCameraFree;
-(void) orbit:(int)deplacementX :(int)deplacementY;
-(void) resizeWindow:(int)minX :(int)minY :(int)maxX :(int)maxY;
-(void) getProperties:(FullPropertiesApple*)prop;
-(void) setProperties:(FullPropertiesApple*)prop;
-(RazerKey) getSelectedNodesType;
bool RenderNodeCallback(RazerKey key);
void EditionEventCallback(EditionEventCodes pEvent);
-(void) setMapFields:(NSString*)pMapName : (NSString*) pMapDescription : (int) pMapPublic;
@end