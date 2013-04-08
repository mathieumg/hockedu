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
    if([mEventManager.mModel eventModification:FIELD_MODIFICATION_EVENT_CLICK:coordVirt]==0)
    {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"You can't add outside of the edition zone and on top of other objects." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
        [alert show];
    }
    
}
-(void)stateEnd
{
    [mEventManager.mModel eventCancel];
}
@end
