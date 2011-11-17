#pragma once

#include "CinderGLSketch.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "Vbo.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"

using namespace ci;

class DemoSketch : public CinderGLSketch {
public:
    
    void setup();
    void update();
    void draw(const Area &area);
    
    gl::Fbo m_fbo_read, m_fbo_write;
    Vec2i   m_fbo_size;
    
    gl::VboRef m_plane, m_box;
    
    gl::GlslProg m_ripple_shader, m_tex_shader, m_color_shader;
    
    gl::Texture m_touch_tex;
    
    CameraOrtho m_camera;
    CameraPersp m_camera_persp;
    
    const static int PIXEL_SCALE = 2;
    
};