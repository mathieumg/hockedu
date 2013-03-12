//
//  EAGLViewController.h
//  AppDemo
//

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "OpenGLWaveFrontObject.h"
#import "Model.h"
extern int const LARGEUR_FENETRE;
extern int const HAUTEUR_FENETRE;

@class EAGLView;

@interface EAGLViewController : UIViewController <UIGestureRecognizerDelegate> {
    EAGLContext *context;
    EAGLView* theEAGLView;
    GLuint program;
    IBOutlet UIView *mGLView;
    IBOutlet UIView *mSideBarView;
    IBOutlet UIView *mTopBarView;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    
    //OpenGLWaveFrontObject *cube;
    Model* mModel;
    float translationX;
    float translationY;
    float zoomFactor;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property (nonatomic, retain) OpenGLWaveFrontObject *cube;

- (void)startAnimation;
- (void)stopAnimation;
- (void)setupView;
- (void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer;

@end
