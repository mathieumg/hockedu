//
//  EAGLViewController.m
//  AppDemo
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLViewController.h"
#import "EAGLView.h"
#define degreesToRadian(x) (M_PI * (x) / 180.0)

int const LARGEUR_FENETRE = 1024;
int const HAUTEUR_FENETRE = 768;
// Uniform index.
enum {
    UNIFORM_TRANSLATE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

@interface EAGLViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, retain) EAGLView *theEAGLView;
@property (nonatomic, retain) IBOutlet UIView *mGLView;
@property (retain, nonatomic) IBOutlet UIView *mSideBarView;
@property (retain, nonatomic) IBOutlet UIView *mTopBarView;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation EAGLViewController

@synthesize animating;
@synthesize theEAGLView;
@synthesize mSideBarView;
@synthesize mTopBarView;
@synthesize mGLView;
@synthesize context;
@synthesize displayLink;
@synthesize cube;

- (void)awakeFromNib
{
    [self.view init];
    theEAGLView = [[EAGLView alloc] initWithFrame:CGRectMake(0.0, 0.0, self.mGLView.bounds.size.height, self.mGLView.bounds.size.width)];
    
    if (!theEAGLView)
        NSLog(@"Failed to create ES view");
    
    theEAGLView.opaque = YES;
    
    mModel = [[Model alloc]init];
    translationX = 0.0;
    translationY = 0.0;
    zoomFactor = 0.5;
    
    [self.mGLView addSubview:theEAGLView];
    [self.mGLView addSubview:mSideBarView];
    [self.mGLView addSubview:mTopBarView];
    [self.theEAGLView setFramebuffer];
    
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
}

- (void)dealloc
{
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [mGLView release];
    [mSideBarView release];
    [mTopBarView release];
    [super dealloc];
}


//With this and the next method, we only allow the landscaperight orientation when on this view
-(BOOL)shouldAutorotate {
    return YES;
}
- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidLoad
{
    UIRotationGestureRecognizer *rotationGesture = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotationDetectee:)];
    [rotationGesture setDelegate:self];
    [theEAGLView addGestureRecognizer:rotationGesture];
    [rotationGesture release];
}


- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating) {
        [self setupView];
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)updateOrtho
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof( ( -( LARGEUR_FENETRE / 2) + translationX ) * zoomFactor, ( (LARGEUR_FENETRE / 2) + translationX) * zoomFactor, ( -(HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor, ( (HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor, -5, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

-(void) selectionModeButtonTouched:(UIButton *)sender
{
    if([sender isSelected])
    {
        [sender setSelected:NO];
        [sender setTitle:@"Selection Mode : OFF" forState:UIControlStateNormal];
        mSelectionMode = FALSE;
    }
    else
    {
        [sender setSelected:YES];
        [sender setTitle:@"Selection Mode : ON" forState:UIControlStateSelected];
        mSelectionMode = TRUE;
    }
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    touchMoved = false;
    UITouch *touch = [[event allTouches] anyObject];
    firstCorner = [touch locationInView:theEAGLView];
    NSLog(@"Position de tous les doigts venant de commencer à toucher l'écran");
    for(UITouch* touch in touches) {
        CGPoint positionCourante = [touch locationInView:theEAGLView];
        NSLog(@"x: %f y: %f", positionCourante.x, positionCourante.y);
    }
    NSLog(@"Position de tous les doigts sur l'écran");
    NSSet *allTouches = [event allTouches];
    for(UITouch* touch in allTouches) {
        CGPoint positionCourante = [touch locationInView:theEAGLView];
        NSLog(@"x: %f y: %f", positionCourante.x, positionCourante.y);
    }
    NSLog(@"\n\n");
}


-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    if ([[event allTouches] count] == 1)
    {
        if (!mSelectionMode)
        {
            UITouch *touch = [[event allTouches] anyObject];
            CGPoint positionCourante = [touch locationInView:theEAGLView];
            CGPoint positionPrecedente = [touch previousLocationInView:theEAGLView];
            translationX -= (positionCourante.x - positionPrecedente.x);
            translationY += (positionCourante.y - positionPrecedente.y);
            
            // Set boundaries for the editing grid, currently 1000x1000, centered at 0,0.
            /*
             if( translationX < ( -500 / zoomFactor ) )
             {
             translationX = (int)( -500 / zoomFactor );
             }
             */
            if( translationX < -500 )
            {
                translationX = -500;
            }
            else if( translationX > 500 )
            {
                translationX = 500;
            }
            
            if( translationY < -500 )
            {
                translationY = -500;
            }
            else if( translationY > 500 )
            {
                translationY = 500;
            }
            
            [self updateOrtho];
            
        }
        else
        {
            touchMoved = true;
        }
    }
    else if([[event allTouches] count] == 2) {
        // Poor man's pinch, perhaps implement as a UIGesture later on.
        
        // Acquire the position of the two fingers.
        NSSet *allTouches = [event allTouches];
        CGPoint positionFingers[4];
        int index = 0;
        for(UITouch* touch in allTouches) {
            positionFingers[ index++ ] = [touch locationInView:theEAGLView];
            positionFingers[ index++ ] = [touch previousLocationInView:theEAGLView];
        }
        
        float deltaX = positionFingers[2].x - positionFingers[0].x;
        float deltaY = positionFingers[2].y - positionFingers[0].y;
        
        float distance1 = sqrtf( ( pow( deltaX, 2 ) + pow( deltaY, 2 ) ) );
        
        deltaX = positionFingers[3].x - positionFingers[1].x;
        deltaY = positionFingers[3].y - positionFingers[1].y;
        
        float distance2 = sqrtf( ( pow( deltaX, 2 ) + pow( deltaY, 2 ) ) );
        
        zoomFactor += ( distance2 - distance1 ) * 0.001;
        
        if( zoomFactor < 0.2 )
        {
            zoomFactor = 0.2;
        }
        else if( zoomFactor > 5.0 )
        {
            zoomFactor = 5.0;
        }
        
        [self updateOrtho];
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    //glOrthof( ( -( LARGEUR_FENETRE / 2) + translationX ) * zoomFactor, ( (LARGEUR_FENETRE / 2) + translationX) * zoomFactor, ( -(HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor, ( (HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor, -5, 100);
    
    if ([[event allTouches] count] == 1)
    {
        
        if(mSelectionMode)
        {
            UITouch *touch = [[event allTouches] anyObject];
            CGPoint positionCourante = [touch locationInView:theEAGLView];
            
            int AXIS_X_MIN = ( -( LARGEUR_FENETRE / 2) + translationX ) * zoomFactor;
            int AXIS_X_MAX = ( (LARGEUR_FENETRE / 2) + translationX) * zoomFactor;
            int AXIS_Y_MIN = ( -(HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor;
            int AXIS_Y_MAX = ( (HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor;
            
            int CV_X_NOW = positionCourante.x / (double) LARGEUR_FENETRE * (AXIS_X_MAX-AXIS_X_MIN) + AXIS_X_MIN;
            //(clotureXmin + ( positionCourante.x / fenetreVirtuelleX )*LARGEUR_FENETRE);
            int CV_Y_NOW = AXIS_Y_MAX - (positionCourante.y / (double) HAUTEUR_FENETRE * (AXIS_Y_MAX-AXIS_Y_MIN));
            //(clotureYmax - ( positionCourante.y / fenetreVirtuelleY )*HAUTEUR_FENETRE );
            
            
            int CV_X_OLD;
            int CV_Y_OLD;
            if(touchMoved)
            {
                CV_X_OLD = firstCorner.x / (double) LARGEUR_FENETRE * (AXIS_X_MAX-AXIS_X_MIN) + AXIS_X_MIN;
                CV_Y_OLD = AXIS_Y_MAX - (firstCorner.y / (double) HAUTEUR_FENETRE * (AXIS_Y_MAX-AXIS_Y_MIN));
            }
            else
            {
                CV_X_OLD = CV_X_NOW-2;
                CV_X_NOW += 2;
                CV_Y_OLD = CV_Y_NOW+2;
                CV_Y_NOW-=2;
            }
            int nbNoeudsSelectionnes = [mModel acceptSelectionVisitor:CV_X_OLD:CV_Y_OLD:CV_X_NOW:CV_Y_NOW];
            if(nbNoeudsSelectionnes==1)
            {
                // Si on a un seul noeud selectionne, on ouvre un popovercontroller contenant les proprietes modifiables du noeud
                UITableViewController *tableController = [[UITableViewController alloc]initWithStyle:UITableViewStylePlain];
                
                UITabBarController *tabController = [[UITabBarController alloc] init];
                
                UINavigationController *navController = [[UINavigationController alloc]initWithRootViewController:tableController];
                
                UIPopoverController *popOverController = [[UIPopoverController alloc]initWithContentViewController:navController];
                //navController.tabBarController = tabController;

                [popOverController presentPopoverFromRect:CGRectMake(150, 300, 450, 300) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
                
            }
        }
    }
}

-(void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer
{
    if ([gestureRecognizer numberOfTouches] == 2)
    {
        CGPoint position = [gestureRecognizer locationInView:theEAGLView];
        NSLog(@"Centre de rotation x: %f y: %f",position.x, position.y);
    }
}


-(void)setupView
{
    glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
    
    
	CGRect rect = theEAGLView.bounds;
    
    [self updateOrtho];
    
    
	glViewport(0, 0, rect.size.width, rect.size.height);
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
	glClearColor(0.3765, 0.4039, 0.4862, 1.0); //Background color for the editing area.
    
	glGetError(); // Clear error codes
}


- (void)drawFrame
{
    [(EAGLView *)theEAGLView setFramebuffer];
    
	static GLfloat rotation = 0.0;
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw the background grid.
    glColor4f( 1.0, 1.0, 1.0, 1.0 );
    
    int x1 = translationX - LARGEUR_FENETRE / 2;
    int x2 = translationX + LARGEUR_FENETRE / 2;
    int y1 = translationY - HAUTEUR_FENETRE / 2;
    int y2 = translationY + HAUTEUR_FENETRE / 2;
    
    const int intervalle = 50;
    const int zValue = -5;
    const int size = ( ( ( LARGEUR_FENETRE + HAUTEUR_FENETRE ) / intervalle ) + 5 ) * 3 * 2;
    GLfloat vertices[ size ];
    
    int count = 0;
    // Vertical lines.
    for( int a = x1 - x1 % intervalle; a < x2; a += intervalle )
    {
        vertices[ count++ ] = a;
        vertices[ count++ ] = y1;
        vertices[ count++ ] = zValue;
        
        vertices[ count++ ] = a;
        vertices[ count++ ] = y2;
        vertices[ count++ ] = zValue;
    }
    
    // Horizontal lines.
    for( int a = y1 - y1 % intervalle; a < y2; a += intervalle )
    {
        vertices[ count++ ] = x1;
        vertices[ count++ ] = a;
        vertices[ count++ ] = zValue;
        
        vertices[ count++ ] = x2;
        vertices[ count++ ] = a;
        vertices[ count++ ] = zValue;
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, count / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    [mModel render];
    glDisableClientState(GL_VERTEX_ARRAY);
    
	static NSTimeInterval lastDrawTime;
	if (lastDrawTime)
	{
		NSTimeInterval timeSinceLastDraw = [NSDate timeIntervalSinceReferenceDate] - lastDrawTime;
		rotation+=50 * timeSinceLastDraw;
		Rotation3D rot;
		rot.x = rotation;
		rot.y = rotation;
		rot.z = rotation;
		cube.currentRotation = rot;
	}
	lastDrawTime = [NSDate timeIntervalSinceReferenceDate];
    
    [(EAGLView *)theEAGLView presentFramebuffer];
}

@end
