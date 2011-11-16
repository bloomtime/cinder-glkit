#import "CinderGLViewController.h"

#include "cinder/Area.h"
#include "cinder/Vector.h"

using namespace ci;


@implementation CinderGLViewController

@synthesize context = _context;


- (id)init
{
    self = [super init];
    if(self){
        self->m_sketch = NULL;
    }
    return self;
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES2];

    if(!self.context){
        NSLog(@"Failed to create ES2 context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    if(m_sketch){
        m_sketch->tearDown();
    }
    
    if([EAGLContext currentContext] == self.context){
        [EAGLContext setCurrentContext: nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    else
        return YES;
}


- (void)setSketch:(CinderGLSketch *)sketch
{
    m_sketch = sketch;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    if(m_sketch){
        m_sketch->update();
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    [EAGLContext setCurrentContext: self.context];
    
    if(m_sketch){
        GLKView *view = (GLKView *)self.view;
        
        m_sketch->setSize(Vec2i(view.drawableWidth, view.drawableHeight));
        
        if(m_sketch->m_needs_setup){
            m_sketch->setup();
            m_sketch->m_needs_setup = false;
        }
        
        m_sketch->draw(Area(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height));
    }
}

@end
