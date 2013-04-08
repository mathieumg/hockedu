//
//  EventManager.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EventManager.h"

@implementation EventManager

@synthesize mCurrentState;
@synthesize mModel;

-(EventManager*) init:(Model*)model
{
    mModel = model;
    mCurrentState = [[EditorStateSelection alloc]init:self]; // Etat par defaut en ouvrant l'editeur
    return self;
}

- (void)dealloc {
    [mModel release];
    [mCurrentState release];
    [super dealloc];
}

-(void)touchesBegan:(CGPoint)coordVirt
{
    if(mCurrentState)
    {
        [mCurrentState touchesBegan:coordVirt];
    }
    
}
-(void)touchesMoved:(CGPoint)coordVirt
{
    if(mCurrentState)
    {
        [mCurrentState touchesMoved:coordVirt];
    }
}
-(void)touchesEnded:(CGPoint)coordVirt
{
    if(mCurrentState)
    {
        [mCurrentState touchesEnded:coordVirt];
    }
    
}

-(void) modifyState:(EditorStateName)editorState
{
    if(mCurrentState!=nil)
    {
        [mCurrentState stateEnd];
        [mCurrentState release];
    }
    switch(editorState)
    {
        case EDITOR_STATE_TRANSFORMATION_ROTATION : mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_ROTATE:self];
            break;
        case EDITOR_STATE_TRANSFORMATION_ECHELLE : mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_SCALE:self];
            break;
        case EDITOR_STATE_TRANSFORMATION_DEPLACEMENT : mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_MOVE:self];
            break;
        case EDITOR_STATE_SELECTION : mCurrentState = [[EditorStateSelection alloc] init:self];
            break;
        case EDITOR_STATE_AJOUTER_PORTAIL : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_PORTAL:self];
            break;
        case EDITOR_STATE_AJOUTER_MURET : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_WALL:self];
            break;
        case EDITOR_STATE_AJOUTER_MAILLET : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_MALLET:self];
            break;
        case EDITOR_STATE_AJOUTER_RONDELLE : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_PUCK:self];
            break;
        case EDITOR_STATE_AJOUTER_ACCELERATEUR : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_BOOST:self];
            break;
        case EDITOR_STATE_AJOUTER_BONUS : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_BONUS:self];
            break;
        case EDITOR_STATE_MOVE_WINDOW : mCurrentState = [[EditorStateView alloc]init:self];
            break;
        case EDITOR_STATE_ZOOM_PROPORTIONNEL : mCurrentState = [[EditorStateView alloc]init:self];
            break;
        default: break;
    }
}



@end
