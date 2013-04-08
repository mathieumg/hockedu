//
//  EditorStateAdd.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateAdd.h"
#import "Enum_Declarations.h"
#import "EventManager.h"
@implementation EditorStateAdd
FieldModificationStrategyType currentType;
-(EditorStateAdd*) init:(FieldModificationStrategyType)type:(EventManager*)eventManager;
{
    [super init:eventManager];
    currentType = type;
    [mEventManager.mModel beginModification:currentType:CGPointMake(-50, 0)];
    return self;
}
-(void)touchesBegan:(CGPoint)coordVirt
{
    [mEventManager.mModel eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
}
-(void)touchesMoved:(CGPoint)coordVirt
{
    [mEventManager.mModel eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
}
-(void)touchesEnded:(CGPoint)coordVirt
{
    [mEventManager.mModel eventModification:FIELD_MODIFICATION_EVENT_CLICK:coordVirt];
    
}
-(void)stateEnd
{
    [mEventManager.mModel eventCancel];
}
@end
