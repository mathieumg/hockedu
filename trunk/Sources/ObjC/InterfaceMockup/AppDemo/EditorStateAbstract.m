//
//  EditorStateAbstract.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EditorStateAbstract.h"
#import "EventManager.h"
@implementation EditorStateAbstract
@synthesize mEventManager;
-(EditorStateAbstract*)init:(EventManager*)eventManager
{
    mEventManager = eventManager;
    return self;
}
-(void)touchesBegan:(CGPoint)coordVirt;
{
    // Rien, les sub classes reimplemente cette methode (virtual)
}
-(void)touchesMoved:(CGPoint)coordVirt;
{
    // Rien, les sub classes reimplemente cette methode (virtual)
}
-(void)touchesEnded:(CGPoint)coordVirt;
{
    // Rien, les sub classes reimplemente cette methode (virtual)
}
-(void)stateEnd
{
}
@end
