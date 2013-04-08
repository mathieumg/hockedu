//
//  EditorStateAbstract.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "Model.h"
@class EventManager;
@interface EditorStateAbstract : NSObject
{
    EventManager* mEventManager;
}
@property (retain,nonatomic) EventManager* mEventManager;
-(EditorStateAbstract*)init:(EventManager*)eventManager;
-(void)touchesBegan:(CGPoint)coordVirt;
-(void)touchesMoved:(CGPoint)coordVirt;
-(void)touchesEnded:(CGPoint)coordVirt;
-(void)stateEnd;

@end
