//
//  EditorStateView.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateView.h"

@implementation EditorStateView
CGPoint positionPrecedente;
-(EditorStateView*)init
{
    return self;
}
-(void)touchesBegan:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model
{
    positionPrecedente = coordVirt;
}
-(void)touchesMoved:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model
{
    CGPoint positionCourante = coordVirt;
    
    int translationX = (positionCourante.x - positionPrecedente.x);
    int translationY = (positionCourante.y - positionPrecedente.y);
    [model orbit:translationX :translationY];
    positionPrecedente = positionCourante;
}
-(void)touchesEnded:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model
{
    
}
@end
