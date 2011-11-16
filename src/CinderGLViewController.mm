#import "CinderGLViewController.h"

#include <vector>

#include "cinder/Area.h"
#include "cinder/Vector.h"
#include "cinder/app/TouchEvent.h"

using namespace ci;
using namespace ci::app;

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
    
    if(m_sketch)
        [self setSketch: m_sketch];
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
    
    if(self.view){
        [EAGLContext setCurrentContext: self.context];
        
        Vec2i size(self.view.bounds.size.width, self.view.bounds.size.height);
        
        m_sketch->setSize(size);
        if(m_sketch->m_needs_setup){
            m_sketch->setup();
            m_sketch->m_needs_setup = false;
        }
    }
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
        m_sketch->draw(Area(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height));
    }
}

#pragma mark - touch methods

- (uint32_t)addTouchToMap:(UITouch*)touch
{
	uint32_t candidateId = 0;
	bool found = true;
	while( found ) {
		candidateId++;
		found = false;
		for( std::map<UITouch*,uint32_t>::const_iterator mapIt = mTouchIdMap.begin(); mapIt != mTouchIdMap.end(); ++mapIt ) {
			if( mapIt->second == candidateId ) {
				found = true;
				break;
			}
		}
	}
	
	mTouchIdMap.insert( std::make_pair( touch, candidateId ) );
	
	return candidateId;
}

- (void)removeTouchFromMap:(UITouch*)touch
{
	std::map<UITouch*,uint32_t>::iterator found( mTouchIdMap.find( touch ) );
	if( found == mTouchIdMap.end() )
		;//std::cout << "Couldn' find touch in map?" << std::endl;
	else
		mTouchIdMap.erase( found );
}

- (uint32_t)findTouchInMap:(UITouch*)touch
{
	std::map<UITouch*,uint32_t>::const_iterator found( mTouchIdMap.find( touch ) );
	if( found == mTouchIdMap.end() ) {
		;//std::cout << "Couldn' find touch in map?" << std::endl;
		return 0;
	}
	else
		return found->second;
}

- (void)updateActiveTouches
{
	static float contentScale = [self.view respondsToSelector:NSSelectorFromString(@"contentScaleFactor")] ? self.view.contentScaleFactor : 1;
    
	std::vector<TouchEvent::Touch> activeTouches;
	for( std::map<UITouch*,uint32_t>::const_iterator touchIt = mTouchIdMap.begin(); touchIt != mTouchIdMap.end(); ++touchIt ) {
		CGPoint pt = [touchIt->first locationInView:self.view];
		CGPoint prevPt = [touchIt->first previousLocationInView:self.view];
		activeTouches.push_back( TouchEvent::Touch( Vec2f( pt.x, pt.y ) * contentScale, Vec2f( prevPt.x, prevPt.y ) * contentScale, touchIt->second, [touchIt->first timestamp], (__bridge_retained void*)touchIt->first ) );
	}
	m_sketch->setActiveTouches( activeTouches );
}

- (std::vector<ci::app::TouchEvent::Touch>)convertTouches:(NSSet*)touches andRemoveFromMap:(BOOL)remove
{
    static float contentScale = [self.view respondsToSelector:NSSelectorFromString(@"contentScaleFactor")] ? self.view.contentScaleFactor : 1;
    
    std::vector<TouchEvent::Touch> touchList;
    for( UITouch *touch in touches ) {
        CGPoint pt = [touch locationInView:self.view];
        CGPoint prevPt = [touch previousLocationInView:self.view];
        touchList.push_back( TouchEvent::Touch( Vec2f( pt.x, pt.y ) * contentScale, Vec2f( prevPt.x, prevPt.y ) * contentScale, [self addTouchToMap:touch], [touch timestamp], (__bridge_retained void*)touch ) );
        if (remove) {
            [self removeTouchFromMap:touch];
        }
    }
    return touchList;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event
{	
    if(m_sketch){
        std::vector<TouchEvent::Touch> touchList = [self convertTouches:touches andRemoveFromMap:NO];
        [self updateActiveTouches];
        if( ! touchList.empty() )
            m_sketch->touchesBegan( TouchEvent( touchList ) );
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    if(m_sketch){
        std::vector<TouchEvent::Touch> touchList = [self convertTouches:touches andRemoveFromMap:NO];
        [self updateActiveTouches];
        if( ! touchList.empty() )
            m_sketch->touchesMoved( TouchEvent( touchList ) );
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    if(m_sketch){
        std::vector<TouchEvent::Touch> touchList = [self convertTouches:touches andRemoveFromMap:YES];        
        [self updateActiveTouches];
        if( ! touchList.empty() )
            m_sketch->touchesEnded( TouchEvent( touchList ) );
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
	[self touchesEnded:touches withEvent:event];
}

@end
