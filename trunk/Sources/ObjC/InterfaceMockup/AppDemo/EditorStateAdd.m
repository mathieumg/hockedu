//
//  EditorStateAdd.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateAdd.h"
#import "Enum_Declarations.h"
@implementation EditorStateAdd
FieldModificationStrategyType currentType;
-(EditorStateAdd*) init:(FieldModificationStrategyType)type
{
    currentType = type;
    return self;
}
-(void)touchesBegan:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    [model beginModification:currentType:coordVirt];
}
-(void)touchesMoved:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    [model eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
}
-(void)touchesEnded:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    [model eventModification:FIELD_MODIFICATION_EVENT_CLICK:coordVirt];
    [model eventCancel];
}
@end
