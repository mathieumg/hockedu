//
//  EditorStateTransform.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateTransform.h"
#import "EventManager.h"
@implementation EditorStateTransform
FieldModificationStrategyType _mType;
-(EditorStateTransform*)init:(FieldModificationStrategyType)type:(EventManager*)eventManager
{
    [super init:eventManager];
    _mType = type;
    return self;
}
-(void)touchesBegan:(CGPoint)coordVirt
{
    [mEventManager.mModel beginModification:_mType:coordVirt];
}
-(void)touchesMoved:(CGPoint)coordVirt
{
    [mEventManager.mModel eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
}
-(void)touchesEnded:(CGPoint)coordVirt
{
    [mEventManager.mModel endModification];
}
@end
