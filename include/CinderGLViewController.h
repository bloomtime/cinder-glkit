#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "CinderGLSketch.h"

using namespace ci;

@interface CinderGLViewController : GLKViewController {
    CinderGLSketch *m_sketch;
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setSketch:(CinderGLSketch *)sketch;

@end
