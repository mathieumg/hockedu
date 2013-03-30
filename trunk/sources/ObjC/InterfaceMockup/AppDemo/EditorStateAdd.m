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
-(EditorStateAdd*) init:(FieldModificationStrategyType)type
{
    return self;
}
-(void)touchesBegan:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    [model beginModification:FIELD_MODIFICATION_ADD_PORTAL:coordVirt];
}
-(void)touchesMoved:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    
}
-(void)touchesEnded:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    
}
@end
