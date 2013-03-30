//
//  EditorStateAbstract.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateAbstract.h"

@implementation EditorStateAbstract
-(void)touchesBegan:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    // Rien, les sub classes reimplemente cette methode (virtual)
}
-(void)touchesMoved:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    // Rien, les sub classes reimplemente cette methode (virtual)
}
-(void)touchesEnded:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    // Rien, les sub classes reimplemente cette methode (virtual)
}
@end
