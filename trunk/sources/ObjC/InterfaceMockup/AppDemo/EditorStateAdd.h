//
//  EditorStateAdd.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "EditorStateAbstract.h"
#import "Enum_Declarations.h"
@interface EditorStateAdd : EditorStateAbstract
-(EditorStateAdd*) init:(FieldModificationStrategyType)type;
-(void)touchesBegan:(UITouch *)touch;
-(void)touchesMoved:(UITouch *)touch;
-(void)touchesEnded:(UITouch *)touch;
@end
