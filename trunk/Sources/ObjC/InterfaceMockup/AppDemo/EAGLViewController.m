//
//  EAGLViewController.m
//  AppDemo
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLViewController.h"
#import "EAGLView.h"
#define degreesToRadian(x) (M_PI * (x) / 180.0)
#import "Enum_Declarations.h"
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
@property (nonatomic, retain) EventManager* mEventManager;
@property (nonatomic, retain) EAGLView *theEAGLView;
@property (nonatomic, retain) IBOutlet UIView *mGLView;
@property (retain, nonatomic) IBOutlet UIView *mSideBarView;
@property (retain, nonatomic) IBOutlet UIView *mTopBarView;
@property (nonatomic, assign) CADisplayLink *displayLink;
@property (nonatomic, assign) BOOL wrap;
@property (nonatomic, assign) BOOL clipsToBounds;
@property (nonatomic, retain) NSMutableArray *items;
- (void) itemSelected:(PieMenuItem *)item;
- (void) setupPieMenu;
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
@synthesize mEventManager;
@synthesize carousel;
@synthesize items;
// Pie menu
@synthesize pieMenu;
@synthesize labelPieMenu;

- (void)awakeFromNib
{
    self.items = [NSMutableArray array];
    for (int i = 0; i < 100; i++)
    {
        [items addObject:[NSNumber numberWithInt:i]];
    }
    
    [self.view init];
    theEAGLView = [[EAGLView alloc] initWithFrame:CGRectMake(0.0, 0.0, self.mGLView.bounds.size.height, self.mGLView.bounds.size.width)];
    
    if (!theEAGLView)
        NSLog(@"Failed to create ES view");
    
    theEAGLView.opaque = YES;
    
    mModel = [[Model alloc]init];
    mEventManager = [[EventManager alloc]init:mModel];
    translationX = 0.0;
    translationY = 0.0;
    zoomFactor = 0.5;
    mSelectTool = true;
    mSelectionMode = true;
    mMoveTool = false;
    mCreationMode = false;
    itemToBeAdded = -1;
    imageObjectToAdd= nil;
    
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
    [mEventManager release];
    [mGLView release];
    [mSideBarView release];
    [mTopBarView release];
    carousel.delegate = nil;
    carousel.dataSource = nil;
    
    [carousel release];
    [items release];
    
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
    [super viewDidLoad];
    
    
    
    // SETUP DES GESTURES
    UIRotationGestureRecognizer *rotationGesture = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotationDetectee:)];
    //UILongPressGestureRecognizer *longPressGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressDetected:)];
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(longPressDetected:)];
    
    [rotationGesture setDelegate:self];
    //[longPressGesture setDelegate:self];
    [tapGesture setNumberOfTapsRequired:2];
    [tapGesture setDelegate:self];
    
    [mGLView addGestureRecognizer:rotationGesture];
    //[mGLView addGestureRecognizer:longPressGesture];
    [mGLView addGestureRecognizer:tapGesture];
    
    [rotationGesture release];
    //[longPressGesture release];
    [tapGesture release];
    
    // FIN SETUP DES GESTURES
    
    // Set le carousel
    carousel.type = iCarouselTypeLinear;
    // Set le pie menu, voir methode
    [self setupPieMenu];
}

- (void) itemSelected:(PieMenuItem *)item {
    
    // Fonction appelle lorsqun item du pie menu est selectionne
	NSLog(@"Item '%s' selected", [item.title UTF8String]);
	labelPieMenu.text = [NSString stringWithFormat:@"Item '%s' selected", [item.title UTF8String]];
}

- (void) setupPieMenu
{
    // SETUP DU PIE MENU
	self.pieMenu = [[[PieMenu alloc] init] autorelease];
	PieMenuItem *itemA = [[PieMenuItem alloc] initWithTitle:@"Select"
													  label:nil
													 target:self
												   selector:@selector(selectToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"cursor_24x24.png"]];
    
	PieMenuItem *itemB = [[PieMenuItem alloc] initWithTitle:@"Move"
													  label:nil
													 target:self
												   selector:@selector(moveToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"move_alt2_24x24.png"]];
	
	PieMenuItem *itemC = [[PieMenuItem alloc] initWithTitle:@"Rotate"
													  label:nil
													 target:self
												   selector:@selector(rotationToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"curved_arrow_24x18.png"]];
	
	PieMenuItem *itemD = [[PieMenuItem alloc] initWithTitle:@"Scale"
													  label:nil
													 target:self
												   selector:@selector(scaleToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"fullscreen_24x24.png"]];
	
	
	PieMenuItem *itemE = [[PieMenuItem alloc] initWithTitle:@"Duplicate"
													  label:nil
													 target:self
												   selector:@selector(duplicateToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"fork_21x24.png"]];
	
	PieMenuItem *itemF = [[PieMenuItem alloc] initWithTitle:@"Delete"
													  label:nil
													 target:self
												   selector:@selector(deleteToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"x_alt_24x24.png"]];
	
	PieMenuItem *itemG = [[PieMenuItem alloc] initWithTitle:@"Properties"
													  label:nil
													 target:self
												   selector:@selector(itemSelected:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"cog_24x24.png"]];
	
	
	//[itemA addSubItem:itemE];
	//[itemA addSubItem:itemB];
	//[itemA addSubItem:itemD];
	[pieMenu addItem:itemA];
	[pieMenu addItem:itemB];
    [pieMenu addItem:itemC];
    [pieMenu addItem:itemD];
	[pieMenu addItem:itemE];
	[pieMenu addItem:itemF];
	[pieMenu addItem:itemG];
	
	[itemA release];
	[itemB release];
	[itemC release];
	[itemD release];
	[itemE release];
	[itemF release];
	[itemG release];
}

#warning Fonction a mettre dans les settings!!!!!
- (IBAction) fingerSizeAction:(id)sender {
	UISegmentedControl* segCtl = sender;
	pieMenu.fingerSize = [segCtl selectedSegmentIndex];
}
#warning Fonction a mettre dans les settings!!!!!
- (IBAction) leftHandedAction:(id)sender {
	UISwitch *swit = (UISwitch *)sender;
	pieMenu.leftHanded = swit.on;
}

//- (UIResponder *)nextResponder {
//	if (pieMenu.on) {
//		return [pieMenu view];
//	} else {
//		return [super nextResponder];
//	}
//}

- (IBAction)longPressDetected:(UILongPressGestureRecognizer *)sender;
{
    //auto a=0;
    
    //UITouch *touch = [sender. anyObject];
	CGPoint p = [sender locationInView:self.mGLView];//[touch locationInView:self.view];
	[pieMenu showInView:self.view atPoint:p];
    //[super touchesBegan:touches withEvent:event];
    [mModel eventCancel];
    
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
    self.carousel = nil;
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

- (void)unselectAllTools
{
    // AJOUTER TOUS NOUVEAUX TOOL ICI
    //mMoveTool = false;
    //mSelectTool = false;
}
-(IBAction) selectionModeButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_SELECTION];
    // En mode selection, on ne peut pas ajouter ditem
    //itemToBeAdded = -1;
    // On ne peut pas avoir creationmode et selection mode en meme temps
    //mCreationMode = false;
    // On inverse selection mode
    //mSelectionMode = !mSelectionMode;
    //[sender setSelected:![sender isSelected]];
    
}


- (IBAction)creationModeButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_AJOUTER_PORTAIL];
    // En creation mode, on ne peut quajouter des nouveaux items, aucun tool de disponible
//    [self unselectAllTools];
//    mCreationMode = !mCreationMode;
//    mSelectionMode = false;
    //[sender setSelected:![sender isSelected]];
    
}


- (IBAction)selectToolButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_SELECTION];
    
    // Disponible uniquement si on est en selectionmode
//    if (mSelectionMode) {
//        mSelectTool = true;
//        mMoveTool = false;
//    }
    
}
- (IBAction)moveToolButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_TRANSFORMATION_DEPLACEMENT];
    // Disponible uniquement si on est en selectionmode
//    if (mSelectionMode) {
//        mSelectTool = false;
//        mMoveTool = true;
//    }
    
}
- (IBAction)rotationToolButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_TRANSFORMATION_ROTATION];
}


- (IBAction)scaleToolButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_TRANSFORMATION_ECHELLE];
}

- (IBAction)duplicateToolButtonTouched:(UIButton *)sender
{
    [mModel duplicateSelection];
}

- (IBAction)deleteToolButtonTouched:(UIButton *)sender
{
    [mModel deleteSelection];
}

- (IBAction)portalButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_AJOUTER_PORTAIL];
    // On envois la position du centre du bouton de creation, pour cacher lobjet cree en arriere
    //[mModel beginModification:FIELD_MODIFICATION_ADD_PORTAL:sender.center];
    // On met le portal comme item a ajouter au prochain clique dans la vue
    itemToBeAdded = PORTAL;
}

-(IBAction) saveAndExitButtonTouched:(UIButton *)sender
{
    // On sauvegarde la map
    [mModel saveField];
    
}

- (CGPoint)convertScreenCoordToVirtualCoord:(CGPoint)pointToConvert;
{
    int AXIS_X_MIN = ( -( LARGEUR_FENETRE / 2) + translationX ) * zoomFactor;
    int AXIS_X_MAX = ( (LARGEUR_FENETRE / 2) + translationX) * zoomFactor;
    int AXIS_Y_MIN = ( -(HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor;
    int AXIS_Y_MAX = ( (HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor;
    
    CGPoint pointVirt;
    
    pointVirt.x = pointToConvert.x / (double) LARGEUR_FENETRE * (AXIS_X_MAX-AXIS_X_MIN) + AXIS_X_MIN;
    pointVirt.y = AXIS_Y_MAX - (pointToConvert.y / (double) HAUTEUR_FENETRE * (AXIS_Y_MAX-AXIS_Y_MIN));
    
    return pointVirt;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchCoordVirt = [self convertScreenCoordToVirtualCoord:[touch locationInView:theEAGLView]];
    [mEventManager touchesBegan:touch:touchCoordVirt];
    // On prend en note le point ou le toucher a commencer
    //firstCorner = [touch locationInView:theEAGLView];
    // On reinitialise le bool disant si on drag
    //touchMoved = false;
//    if (mCreationMode)
//    {
//        //CGPoint randomPoint;
//        //randomPoint.x=-25;
//        //randomPoint.y=0;
//        CGPoint coordVirt = [self convertScreenCoordToVirtualCoord:firstCorner];
//        [mModel beginModification:FIELD_MODIFICATION_ADD_PORTAL:coordVirt];
//        if(itemToBeAdded != -1)
//        {
//            // Si on est en mode creation et quon a un item a ajouter, on assigne limage associe a l'item
//            switch (itemToBeAdded) {
//                case PORTAL:
//                    //imageObjectToAdd = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"cameraFixe.png"]];
//                    break;
//                    
//                case PUCK:
//                    //imageObjectToAdd = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"deplacer.png"]];
//                    break;
//                default:
//                    // On ajoute des portal si on ne connait pas l'item demande, par securite
//                    //imageObjectToAdd = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"cameraFixe.png"]];
//                    break;
//            }
//            // On assigne la position de l'objet a la position du touche et on ajoute le UIImageView a la vu EAGL
//            //imageObjectToAdd.center = [touch locationInView:theEAGLView];
//            //[[self view] addSubview:imageObjectToAdd];
//        }
//    }
    
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
        CGPoint touchCoordVirt = [self convertScreenCoordToVirtualCoord:[touch locationInView:theEAGLView]];
        [mEventManager touchesMoved:touch:touchCoordVirt];
        
        //CGPoint positionCourante = [touch locationInView:theEAGLView];
//        if (mCreationMode) {
//            // Si on est en mode creation et touchMoved, on update la position de limage
//            //imageObjectToAdd.center = [touch locationInView:theEAGLView];
//            CGPoint coordVirt = [self convertScreenCoordToVirtualCoord:positionCourante];
//            [mModel eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
//            
//        }
//        else if (mSelectionMode && mMoveTool)
//        {
//            CGPoint positionPrecedente = [touch previousLocationInView:theEAGLView];
//            translationX -= (positionCourante.x - positionPrecedente.x);
//            translationY += (positionCourante.y - positionPrecedente.y);
//            
//            // Set boundaries for the editing grid, currently 1000x1000, centered at 0,0.
//            /*
//             if( translationX < ( -500 / zoomFactor ) )
//             {
//             translationX = (int)( -500 / zoomFactor );
//             }
//             */
//            if( translationX < -500 )
//            {
//                translationX = -500;
//            }
//            else if( translationX > 500 )
//            {
//                translationX = 500;
//            }
//            
//            if( translationY < -500 )
//            {
//                translationY = -500;
//            }
//            else if( translationY > 500 )
//            {
//                translationY = 500;
//            }
//            
//            [self updateOrtho];
//            
//        }
//        else if (mSelectionMode && mSelectTool)
//        {
//            touchMoved = true;
//        }
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
    
    
    if ([[event allTouches] count] == 1)
    {
        UITouch *touch = [[event allTouches] anyObject];
        //CGPoint positionCourante = [touch locationInView:theEAGLView];
        CGPoint touchCoordVirt = [self convertScreenCoordToVirtualCoord:[touch locationInView:theEAGLView]];
        [mEventManager touchesEnded:touch:touchCoordVirt];
//        
//        if (mCreationMode) {
//            // Destruction de limage de lobjet qui suit la position du doigt
//            
//                // On drop lobjet
//                CGPoint coordVirt = [self convertScreenCoordToVirtualCoord:positionCourante];
//            // On drop le noeud a la position finale
//            [mModel eventModification:FIELD_MODIFICATION_EVENT_CLICK:coordVirt];
//            // On enleve le prochain noeud qui apparait pour sajouter, utilise dans c++
//            [mModel eventCancel];
//                //[imageObjectToAdd removeFromSuperview];
//                //[imageObjectToAdd release];
//        }
        
//        if(mSelectionMode && mSelectTool)
//        {
//            
//            
//            CGPoint posVirtuelle = [self convertScreenCoordToVirtualCoord:positionCourante];
//            
//            int CV_X_NOW = posVirtuelle.x;
//            int CV_Y_NOW = posVirtuelle.y;
//            
//            
//            int CV_X_OLD;
//            int CV_Y_OLD;
//            if(touchMoved)
//            {
//                CGPoint firstCornerVirt;
//                firstCornerVirt = [self convertScreenCoordToVirtualCoord:firstCorner];
//                CV_X_OLD = firstCornerVirt.x;
//                CV_Y_OLD = firstCornerVirt.y;
//            }
//            else
//            {
//                CV_X_OLD = CV_X_NOW-2;
//                CV_X_NOW += 2;
//                CV_Y_OLD = CV_Y_NOW+2;
//                CV_Y_NOW-=2;
//            }
//            int nbNoeudsSelectionnes = [mModel acceptSelectionVisitor:CV_X_OLD:CV_Y_OLD:CV_X_NOW:CV_Y_NOW];
//            if(nbNoeudsSelectionnes==1)
//            {
//                // Si on a un seul noeud selectionne, on ouvre un popovercontroller contenant les proprietes modifiables du noeud
//                
//                //UITableViewController *tableController = [[UITableViewController alloc]initWithStyle:UITableViewStylePlain];
//                
//                //UITabBarController *tabController = [[UITabBarController alloc] init];
//                
//                //UINavigationController *navController = [[UINavigationController alloc]initWithRootViewController:tableController];
//                
//                //UIPopoverController *popOverController = [[UIPopoverController alloc]initWithContentViewController:navController];
//                //navController.tabBarController = tabController;
//                
//                //[popOverController presentPopoverFromRect:CGRectMake(150, 300, 450, 300) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
//                
//            }
//        }
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
    
    //glEnableClientState(GL_VERTEX_ARRAY);
    [mModel render];
    //glDisableClientState(GL_VERTEX_ARRAY);
    
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


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

#pragma mark -
#pragma mark iCarousel methods

- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel
{
    //return the total number of items in the carousel
    return [items count];
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view
{
    UILabel *label = nil;
    
    //create new view if no view is available for recycling
    if (view == nil)
    {
        //don't do anything specific to the index within
        //this `if (view == nil) {...}` statement because the view will be
        //recycled and used with other index values later
        view = [[[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 98.0f, 92.0f)] autorelease];
        ((UIImageView *)view).image = [UIImage imageNamed:@"aide32x32.png"];
        view.contentMode = UIViewContentModeCenter;
        
        UILabel* label = [[[UILabel alloc] initWithFrame:view.bounds] autorelease];
        label.backgroundColor = [UIColor clearColor];
        label.textAlignment = NSTextAlignmentCenter;
        label.font = [label.font fontWithSize:30];
        label.textColor = [UIColor colorWithRed:0.0 green: 1.0 blue: 0.0 alpha:1.0f];
        label.tag = 1;
        [view addSubview:label];
    }
    else
    {
        //get a reference to the label in the recycled view
        label = (UILabel *)[view viewWithTag:1];
    }
    
    //set item label
    //remember to always set any properties of your carousel item
    //views outside of the `if (view == nil) {...}` check otherwise
    //you'll get weird issues with carousel item content appearing
    //in the wrong place in the carousel
    label.text = [[items objectAtIndex:index] stringValue];
    
    return view;
}

- (CGFloat)carousel:(iCarousel *)carousel valueForOption:(iCarouselOption)option withDefault:(CGFloat)value
{
    switch( option )
    {
        case iCarouselOptionSpacing:
            return value * 1.1f;
            
        case iCarouselOptionWrap:
            return YES;
        default :
            break;
    }
    return value;
}

@end
