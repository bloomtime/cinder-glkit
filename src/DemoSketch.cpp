#include "DemoSketch.h"

#include "cinder/gl/gl.h"

void DemoSketch::setup()
{
    m_needs_setup = false;
}

void DemoSketch::update()
{
    // update
}

void DemoSketch::draw(const Area &area)
{
    glViewport(0, 0, m_size.x, m_size.y);
    
    gl::clear(ColorA(1, 1, 0, 1));
}