//
//  EditorStateTransform.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateTransform.h"

@implementation EditorStateTransform
-(EditorStateTransform*)init:(FieldModificationStrategyType)type
{
    return self;
}
-(void)touchesBegan:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model
{
    [model beginModification:FIELD_MODIFICATION_MOVE :coordVirt];
}
-(void)touchesMoved:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model
{
    [model eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
}
-(void)touchesEnded:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model
{
    [model endModification];
}
@end
