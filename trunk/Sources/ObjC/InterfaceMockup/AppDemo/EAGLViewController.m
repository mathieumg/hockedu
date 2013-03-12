//
//  EAGLViewController.m
//  AppDemo
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLViewController.h"
#import "EAGLView.h"
#define degreesToRadian(x) (M_PI * (x) / 180.0)

float const LARGEUR_FENETRE = 1024;
float const HAUTEUR_FENETRE = 768;
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

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
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
        UITouch *touch = [[event allTouches] anyObject];
        CGPoint positionCourante = [touch locationInView:theEAGLView];
        CGPoint positionPrecedente = [touch previousLocationInView:theEAGLView];
        /*
        cube.currentPosition = Vertex3DMake(cube.currentPosition.x + (((positionCourante.x - positionPrecedente.x) / theEAGLView.bounds.size.width) * LARGEUR_FENETRE),
                                            cube.currentPosition.y - (((positionCourante.y - positionPrecedente.y) / theEAGLView.bounds.size.height) * HAUTEUR_FENETRE), 
                                            cube.currentPosition.z);
        */
        
        translationX -= (positionCourante.x - positionPrecedente.x);
        translationY += (positionCourante.y - positionPrecedente.y);
        
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
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(-( LARGEUR_FENETRE / 2) + translationX , (LARGEUR_FENETRE / 2) + translationX, -(HAUTEUR_FENETRE / 2) + translationY, (HAUTEUR_FENETRE / 2) + translationY, 0, 100);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    
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
    
    glOrthof(-(LARGEUR_FENETRE / 2), (LARGEUR_FENETRE / 2), -(HAUTEUR_FENETRE / 2), (HAUTEUR_FENETRE / 2), 0, 100);
    
	glViewport(0, 0, rect.size.width, rect.size.height);  
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity(); 
	glClearColor(0.3765, 0.4039, 0.4862, 1.0); //Background color for the editing area.
		
	glGetError(); // Clear error codes
	
    /*
	NSString *path = [[NSBundle mainBundle] pathForResource:@"cube" ofType:@"obj"];
	OpenGLWaveFrontObject *theObject = [[OpenGLWaveFrontObject alloc] initWithPath:path];
	Vertex3D position = Vertex3DMake(0.0, 0.0, -50.0);
	theObject.currentPosition = position;
	self.cube = theObject;
	//[theObject release];
    */
}


- (void)drawFrame
{
    [(EAGLView *)theEAGLView setFramebuffer];
    
	static GLfloat rotation = 0.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
    
    // Draw the background grid.
    glColor4f( 1.0, 1.0, 1.0, 1.0 );

    
    int x1 = translationX - LARGEUR_FENETRE / 2.0;
    int x2 = translationX + LARGEUR_FENETRE / 2.0;
    int y1 = translationY - HAUTEUR_FENETRE / 2.0;
    int y2 = translationY + HAUTEUR_FENETRE / 2.0;
    
    const int intervalle = 50;
    const int size = ( ( ( LARGEUR_FENETRE + HAUTEUR_FENETRE ) / intervalle ) + 5 ) * 3 * 2;
    GLfloat vertices[ size ];
    
    int count = 0;
    for( int a = x1 - x1 % intervalle; a < x2; a += intervalle )
    {
        vertices[ count++ ] = a;
        vertices[ count++ ] = y1;
        vertices[ count++ ] = -5;
        
        vertices[ count++ ] = a;
        vertices[ count++ ] = y2;
        vertices[ count++ ] = -5;
    }
    
    for( int a = y1 - y1 % intervalle; a < y2; a += intervalle )
    {
        vertices[ count++ ] = x1;
        vertices[ count++ ] = a;
        vertices[ count++ ] = -5;
        
        vertices[ count++ ] = x2;
        vertices[ count++ ] = a;
        vertices[ count++ ] = -5;
    }
    
    /*
    for( int count = 0, i = 0; i < 16; i++ ) {
        // Vertical lines.
        vertices[count++] = 400 - i * 50;
        vertices[count++] = -400;
        vertices[count++] = -5;
        
        vertices[count++] = 400 - i * 50;
        vertices[count++] = 400;
        vertices[count++] = -5;
        
        // Horizontal lines.
        vertices[count++] = -400;
        vertices[count++] = 400 - i * 50;
        vertices[count++] = -5;
        
        vertices[count++] = 400;
        vertices[count++] = 400 - i * 50;
        vertices[count++] = -5;
    }
    */
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, count / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //[cube drawSelf];
    glEnableClientState(GL_VERTEX_ARRAY);
    [mModel render];
    glDisableClientState(GL_VERTEX_ARRAY);
    
    // Dessiner les shit ici
    //Vertex3D    vertex1 = Vertex3DMake(0.0, 50.0, -50.0);
    //Vertex3D    vertex2 = Vertex3DMake(50.0, 0.0, -50.0);
    //Vertex3D    vertex3 = Vertex3DMake(-50.0, 0.0, -50.0);
    //Triangle3D  triangle = Triangle3DMake(vertex1, vertex2, vertex3);
    
    /*
    glLoadIdentity();
    glClearColor(0.7, 0.7, 0.7, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, &triangle);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glDisableClientState(GL_VERTEX_ARRAY);
    */
    /*glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();
    glColor(0.0f,1.0f,0.0f);
    glBegin(GL_LINE_LOOP);
    
    static const float jump = 3.1415926535/10.f;
    const float radius = 50;
    for (float i=0; i < 3.1415926535; i+=jump)
    {
        glVertex2f(cos(i)*radius,sin(i)*radius);
    }
    
    glEnd();*/
    
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
