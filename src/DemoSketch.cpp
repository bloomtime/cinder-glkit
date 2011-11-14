#include "DemoSketch.h"

#include "cinder/ImageIO.h"
#include "cinder/app/App.h"

void DemoSketch::setup()
{
    // create framebuffers
    m_fbo_size = getSize() / PIXEL_SCALE;
    
    gl::Fbo::Format fbo_format;
    fbo_format.enableDepthBuffer(false);
    fbo_format.enableMipmapping(false);
    fbo_format.setMinFilter(GL_NEAREST);
    fbo_format.setMagFilter(GL_NEAREST);
    m_fbo_write = gl::Fbo(m_fbo_size.x, m_fbo_size.y, fbo_format);
    m_fbo_read  = gl::Fbo(m_fbo_size.x, m_fbo_size.y, fbo_format);
    
    m_fbo_read.bindFramebuffer();
    gl::clear(ColorA::black());
    m_fbo_read.unbindFramebuffer();
    
    try{
        m_shader = gl::GlslProg(app::loadResource("ripple.vsh"), app::loadResource("ripple.fsh"));
    }catch(exception &e){
        std::cout << e.what();
    }
     
    m_touch_tex = gl::Texture(loadImage(app::loadResource("touch_32.png")));

    m_camera.setOrtho(0, getSize().x, getSize().y, 0, -1, 1);
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