#include "«PREFIX»Sketch.h"

#include "cinder/ImageIO.h"
#include "cinder/app/App.h"

using namespace std;

void «PREFIX»Sketch::setup()
{
    m_vbo = gl::Vbo::create(GL_LINE_STRIP);
    m_vbo->set(gl::Vbo::Attribute::create("position", 3));
    
    try{
        m_color_shader = gl::GlslProg(app::loadResource("color.vsh"), app::loadResource("color.fsh"));
    }catch(Exception &e){
        cout << "Shader Compile Failed: " << e.what();
    }

    m_camera.setOrtho(0, 1, -1, 1, -1, 1);
}

void «PREFIX»Sketch::update()
{
    float time = getElapsedSeconds() * 0.2f;

    vector<Vec3f> positions;
    for(int n = 100, i = 0; i < n; ++i){
        float t = i / (n - 1.0f);
        positions.push_back(Vec3f(t, m_perlin.fBm(t, time), 0));
    }
    
    m_vbo->get("position")->setData(positions);
}

void «PREFIX»Sketch::draw(const Area &area)
{
    gl::clear(ColorA(1, 1, 0, 1));
    
    glLineWidth(4);
    
    m_color_shader.bind();
    m_color_shader.uniform("u_mvp_matrix", m_camera.getProjectionMatrix() * m_camera.getModelViewMatrix());
    m_color_shader.uniform("u_color", Vec4f(0, 0, 0, 1));
    m_vbo->draw(m_color_shader);
}