//
//  EditorStateSelection.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateSelection.h"
#import "EventManager.h"
@implementation EditorStateSelection
BOOL touchMoved;
CGPoint firstCorner;
-(EditorStateSelection*)init:(EventManager*)eventManager

{
    [super init:eventManager];
    touchMoved = NO;
    return self;
}
-(void)touchesBegan:(CGPoint)coordVirt
{
    touchMoved = NO;
    firstCorner = coordVirt;
    
}
-(void)touchesMoved:(CGPoint)coordVirt
{
    touchMoved = YES;
    
}
-(void)touchesEnded:(CGPoint)coordVirt
{
    //UITouch *touchOnScreen = [[event allTouches] anyObject];
    //CGPoint positionCourante = [touch locationInView:theEAGLView];
    //CGPoint posVirtuelle = [self convertScreenCoordToVirtualCoord:positionCourante];
    
    int CV_X_NOW = coordVirt.x;
    int CV_Y_NOW = coordVirt.y;
    
    int CV_X_OLD;
    int CV_Y_OLD;
    if(touchMoved)
    {
        CGPoint firstCornerVirt = firstCorner;
        //firstCornerVirt = [self convertScreenCoordToVirtualCoord:firstCorner];
        CV_X_OLD = firstCornerVirt.x;
        CV_Y_OLD = firstCornerVirt.y;
    }
    else
    {
        CV_X_OLD = CV_X_NOW-2;
        CV_X_NOW += 2;
        CV_Y_OLD = CV_Y_NOW+2;
        CV_Y_NOW-=2;
    }
    // int nbNoeudsSelectionnes = (Retourne le nb de noeud selectionne si on veut, pas utilise pour le moment)
    [mEventManager.mModel acceptSelectionVisitor:CV_X_OLD:CV_Y_OLD:CV_X_NOW:CV_Y_NOW];
}
@end
