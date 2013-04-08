//
//  EventManager.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "Enum_Declarations.h"
#import "EditorStateAbstract.h"
#import "EditorStateAdd.h"
#import "EditorStateSelection.h"
#import "EditorStateTransform.h"
#import "EditorStateView.h"
#import "Model.h"
@class EAGLViewController;
@interface EventManager : NSObject
{
    EditorStateAbstract *mCurrentState;
    @public Model *mModel;
    EAGLViewController* mViewController;
}
@property (nonatomic, retain) EditorStateAbstract *mCurrentState;
@property (nonatomic, retain) Model *mModel;
@property (nonatomic, retain) EAGLViewController* mViewController;
typedef enum {
	EDITOR_STATE_MOVE_WINDOW
	,EDITOR_STATE_ZOOM_PROPORTIONNEL
	,EDITOR_STATE_TRANSFORMATION_ROTATION
	,EDITOR_STATE_TRANSFORMATION_ECHELLE
	,EDITOR_STATE_TRANSFORMATION_DEPLACEMENT
	,EDITOR_STATE_SELECTION
	,EDITOR_STATE_AJOUTER_PORTAIL
	,EDITOR_STATE_AJOUTER_MURET
	,EDITOR_STATE_AJOUTER_MAILLET
	,EDITOR_STATE_AJOUTER_RONDELLE
    ,EDITOR_STATE_AJOUTER_ACCELERATEUR
    ,EDITOR_STATE_AJOUTER_BONUS
    ,EDITOR_STATE_ORBIT
    ,EDITOR_STATE_AUCUN
	,NUMBER_OF_STATE
	,EDITOR_STATE_INCONNU
} EditorStateName;

-(EventManager*) init:(Model*)model:(EAGLViewController*)viewController;
-(void) modifyState:(EditorStateName)editorState;
-(void)touchesBegan:(CGPoint)coordVirt;
-(void)touchesMoved:(CGPoint)coordVirt;
-(void)touchesEnded:(CGPoint)coordVirt;

@end
