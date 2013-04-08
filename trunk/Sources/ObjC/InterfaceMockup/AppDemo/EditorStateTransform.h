//
//  EditorStateTransform.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "EditorStateAbstract.h"
#import "Enum_Declarations.h"
#import "Model.h"
@interface EditorStateTransform : EditorStateAbstract
-(EditorStateTransform*)init:(FieldModificationStrategyType)type:(EventManager*)eventManager;
-(void)touchesBegan:(CGPoint)coordVirt;
-(void)touchesMoved:(CGPoint)coordVirt;
-(void)touchesEnded:(CGPoint)coordVirt;
@end
