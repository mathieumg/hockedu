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

extern float const LARGEUR_FENETRE;
extern float const HAUTEUR_FENETRE;

@interface EAGLViewController : UIViewController <UIGestureRecognizerDelegate> {
    EAGLContext *context;
    GLuint program;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    
    OpenGLWaveFrontObject *cube;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property (nonatomic, retain) OpenGLWaveFrontObject *cube;

- (void)startAnimation;
- (void)stopAnimation;
- (void)setupView;
- (void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer;

@end
