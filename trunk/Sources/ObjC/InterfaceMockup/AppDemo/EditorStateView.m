//
//  EditorStateView.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateView.h"
#import "EventManager.h"
@implementation EditorStateView
CGPoint positionPrecedente;
-(EditorStateView*)init:(EventManager*)eventManager
{
    [super init:eventManager];
    return self;
}
-(void)touchesBegan:(CGPoint)coordVirt
{
    positionPrecedente = coordVirt;
}
-(void)touchesMoved:(CGPoint)coordVirt
{
    CGPoint positionCourante = coordVirt;
    
    int translationX = (positionCourante.x - positionPrecedente.x);
    int translationY = (positionCourante.y - positionPrecedente.y);
    [mEventManager.mModel orbit:translationX :translationY];
    positionPrecedente = positionCourante;
}
-(void)touchesEnded:(CGPoint)coordVirt
{
    
}
@end
