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
        cout << "Shader Compile Failed: " << e.what();
    }
     
    m_touch_tex = gl::Texture(loadImage(app::loadResource("touch_32.png")));

    m_camera.setOrtho(0, getSize().x, getSize().y, 0, -1, 1);
    m_camera_persp.setPerspective(60, getAspectRatio(), 0.01f, 10.0f);
    m_camera_persp.lookAt(Vec3f(0, 0, -5), Vec3f::zero(), Vec3f::yAxis());
    
    m_plane = gl::Vbo::createPlane(Vec2f::zero(), getSize());
    m_plane->assignLocations(m_shader);
    
    m_box = gl::Vbo::createBox(Vec3f::one());
    m_box->assignLocations(m_shader);
    
    glEnable(GL_DEPTH_TEST);
}

void DemoSketch::update()
{
    // update
}

void DemoSketch::draw(const Area &area)
{
    glViewport(0, 0, m_size.x, m_size.y);
    gl::clear(ColorA(1, 1, 0, 1));
    
    m_shader.bind();
    m_shader.uniform("u_mvp_matrix", m_camera.getProjectionMatrix() * m_camera.getModelViewMatrix());
//    m_shader.uniform("u_resolution", getSize());
//    m_shader.uniform("u_time", (float)getElapsedSeconds());
//    m_shader.uniform("u_texture_prev", 0);
    
    m_plane->draw();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    m_shader.uniform("u_mvp_matrix", m_camera_persp.getProjectionMatrix() * m_camera_persp.getModelViewMatrix() * Matrix44f::createRotation(Vec3f::yAxis(), getElapsedSeconds()));
    m_box->draw();
}