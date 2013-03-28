//
//  EditorStateSelection.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "EditorStateAbstract.h"
@interface EditorStateSelection : EditorStateAbstract
-(EditorStateSelection*)init;
-(void)touchesBegan:(UITouch *)touch;
-(void)touchesMoved:(UITouch *)touch;
-(void)touchesEnded:(UITouch *)touch;
@end
