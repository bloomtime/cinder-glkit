#pragma once

#include "cinder/Vector.h"
#include "cinder/Area.h"
#include "cinder/Timer.h"
#include "cinder/app/TouchEvent.h"

#include <ostream>

namespace cinder {

using namespace std;

class CinderGLSketch {
public:

    CinderGLSketch()
    : m_needs_setup(true)
    , m_timer(true)
    {};
    
    virtual ~CinderGLSketch(){};

    virtual void setup(){};
    virtual void tearDown(){};
    virtual void update(){};
    virtual void draw(const Area &area){};
    
    const Vec2i& getSize() const { return m_size; }
    void         setSize(const Vec2i &size){ m_size = size; }
    
    double getElapsedSeconds() const { return m_timer.getSeconds(); }

    virtual void touchesBegan(app::TouchEvent event){}
    virtual void touchesMoved(app::TouchEvent event){}
    virtual void touchesEnded(app::TouchEvent event){}
    
    const vector<app::TouchEvent::Touch>& getActiveTouches() { return m_active_touches; }
    void setActiveTouches(vector<app::TouchEvent::Touch> touches) { m_active_touches = touches; }
    
    ostream& console(){ return cout; }
    
    bool m_needs_setup;

protected:

    Vec2i m_size;
    Timer m_timer;
    vector<app::TouchEvent::Touch> m_active_touches;
};

} // namespace cinder