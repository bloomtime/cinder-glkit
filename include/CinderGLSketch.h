#pragma once

#include "cinder/Vector.h"
#include "cinder/Area.h"

#include <ostream>

namespace cinder {

class CinderGLSketch {
public:

    CinderGLSketch()
    : m_needs_setup(true){};
    
    virtual ~CinderGLSketch(){};

    virtual void setup(){};
    virtual void tearDown(){};
    virtual void update(){};
    virtual void draw(const Area &area){};
    
    const Vec2i& getSize() const { return m_size; }
    void         setSize(const Vec2i &size){ m_size = size; }
    
    std::ostream& console(){ return std::cout; }
    
    bool m_needs_setup;

protected:

    Vec2i m_size;

};

} // namespace cinder