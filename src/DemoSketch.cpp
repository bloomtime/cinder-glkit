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
    fbo_format.setColorInternalFormat(GL_RGBA);
    m_fbo_write = gl::Fbo(m_fbo_size.x, m_fbo_size.y, fbo_format);
    m_fbo_read  = gl::Fbo(m_fbo_size.x, m_fbo_size.y, fbo_format);
    
    m_fbo_read.bindFramebuffer();
    gl::clear(ColorA::black());
    m_fbo_read.unbindFramebuffer();
    
    try{
        m_ripple_shader = gl::GlslProg(app::loadResource("ripple.vsh"), app::loadResource("ripple.fsh"));
        m_tex_shader = gl::GlslProg(app::loadResource("texture.vsh"), app::loadResource("texture.fsh"));
        m_color_shader = gl::GlslProg(app::loadResource("color.vsh"), app::loadResource("color.fsh"));
    }catch(exception &e){
        cout << "Shader Compile Failed: " << e.what();
    }
     
    m_touch_tex = gl::Texture(loadImage(app::loadResource("touch_32.png")));

    m_camera.setOrtho(0, 1, 0, 1, -1, 1);
    m_camera_persp.setPerspective(60, getAspectRatio(), 0.01f, 10.0f);
    m_camera_persp.lookAt(Vec3f(0, 0, -3), Vec3f::zero(), Vec3f::yAxis());

    m_plane = gl::Vbo::createPlane(Vec2f::zero(), Vec2f::one());
    m_box = gl::Vbo::createWireBox(Vec3f::one());
    
    glEnable(GL_DEPTH_TEST);
}

void DemoSketch::update()
{
    // update
}

void DemoSketch::draw(const Area &area)
{
    // Render to backbuffer
    {
        gl::SaveFramebufferBinding save_fbo;
        
        m_fbo_write.bindFramebuffer();
        m_fbo_read.bindTexture();
        
        glViewport(0, 0, m_fbo_write.getWidth(), m_fbo_write.getHeight());
        
        float time = getElapsedSeconds();
        
        m_ripple_shader.bind();
        m_ripple_shader.uniform("u_mvp_matrix", m_camera.getProjectionMatrix() * m_camera.getModelViewMatrix());
        m_ripple_shader.uniform("u_time", time);
        m_ripple_shader.uniform("u_resolution", Vec2f(m_fbo_write.getSize()));
        m_ripple_shader.uniform("u_texture_prev", 0);
        m_ripple_shader.uniform("u_mouse", Vec2f(0.1, 0.1));
        m_plane->draw(m_ripple_shader);
        
        {
            gl::BoolState save(GL_BLEND);
            gl::enableAlphaBlending();
        
            Vec3f axis(sinf(time * 0.1f), cosf(time * 0.1f), 0);
            
            m_color_shader.bind();
            m_color_shader.uniform("u_mvp_matrix", m_camera_persp.getProjectionMatrix() *
                                                   m_camera_persp.getModelViewMatrix() *
                                                   Matrix44f::createRotation(axis.normalized(), time * 0.5f));
            m_color_shader.uniform("u_color", Vec4f(1,1,1,0.25));
            m_box->draw(m_color_shader);
        }
    }
    
    // Swap FBOs
    gl::Fbo tmp = m_fbo_write;
    m_fbo_write = m_fbo_read;
    m_fbo_read = tmp;

    // Draw to screen
    glViewport(0, 0, getSize().x, getSize().y);
    gl::clear(ColorA(1, 1, 0, 1));
    
    m_fbo_read.bindTexture();
    
    m_tex_shader.bind();
    m_tex_shader.uniform("u_mvp_matrix", m_camera.getProjectionMatrix() * m_camera.getModelViewMatrix());
    m_tex_shader.uniform("u_texture", 0);
    m_plane->draw(m_tex_shader);
}