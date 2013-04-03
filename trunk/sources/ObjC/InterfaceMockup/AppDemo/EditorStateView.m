//
//  EditorStateView.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateView.h"

@implementation EditorStateView
CGPoint oldPoint;
-(EditorStateView*)init
{
    return self;
}
-(void)touchesBegan:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    //oldPoint = [touch location]
}
-(void)touchesMoved:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
//    CGPoint positionPrecedente = [touch previousLocationInView:theEAGLView];
//    translationX -= (positionCourante.x - positionPrecedente.x);
//    translationY += (positionCourante.y - positionPrecedente.y);
//    // Set boundaries for the editing grid, currently 1000x1000, centered at 0,0.
//    
//    if( translationX < ( -500 / zoomFactor ) )
//    {
//        translationX = (int)( -500 / zoomFactor );
//    }
//    
//    if( translationX < -500 )
//    {
//        translationX = -500;
//    }
//    else if( translationX > 500 )
//    {
//        translationX = 500;
//    }
//    
//    if( translationY < -500 )
//    {
//        translationY = -500;
//    }
//    else if( translationY > 500 )
//    {
//        translationY = 500;
//    }
//    
//    [self updateOrtho];
}
-(void)touchesEnded:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model
{
    
}
@end
