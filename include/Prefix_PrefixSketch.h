#pragma once

#include "CinderGLSketch.h"

#include "cinder/gl/GlslProg.h"
#include "Vbo.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"
#include "cinder/Perlin.h"

using namespace ci;

class Prefix_PrefixSketch : public CinderGLSketch {
public:
    
    void setup();
    void update();
    void draw(const Area &area);
    
    gl::VboRef m_vbo;
    
    gl::GlslProg m_color_shader;
    
    CameraOrtho m_camera;
    
    Perlin m_perlin;
    
};