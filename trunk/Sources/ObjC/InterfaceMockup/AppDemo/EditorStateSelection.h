//
//  EditorStateSelection.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "EditorStateAbstract.h"
#import "Model.h"
@interface EditorStateSelection : EditorStateAbstract
-(EditorStateSelection*)init:(EventManager*)eventManager;
-(void)touchesBegan:(CGPoint)coordVirt;
-(void)touchesMoved:(CGPoint)coordVirt;
-(void)touchesEnded:(CGPoint)coordVirt;
-(void)stateEnd;
@end
