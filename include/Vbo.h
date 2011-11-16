#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"

#include <tr1/unordered_map>

namespace cinder { namespace gl {

using namespace std;

class Vbo {
public:

    class Attribute {
    public:

        Attribute(){}
        Attribute(const string &name, int size, int location = -1, GLenum usage = GL_STATIC_DRAW);
        Attribute(const char *name, int size, int location = -1, GLenum usage = GL_STATIC_DRAW);
        Attribute(const Attribute &attr);
        ~Attribute();
        
        void bufferData();
        void bind();
        void bindAndEnable();
        
        GLenum getTarget() const { return m_target; }
        
        void   setUsage(GLenum usage){ m_usage = usage; m_data_dirty = true; }
        GLenum getUsage() const { return m_usage; }
        
        void setLocation(int location){ m_location = location; }
        int  getLocation() const { return m_location; }
        
        Attribute&    setData(Buffer data){ m_data = data; m_data_dirty = true; return *this; };
        Attribute&    setData(const void* data, int data_length);
//        Attribute&    setData(const vector<int> &data);
        Attribute&    setData(const vector<float> &data);
        Attribute&    setData(const vector<Vec2f> &data);
        Attribute&    setData(const vector<Vec3f> &data);
        const Buffer& getData() const { return m_data; }
        
        const string& getName() const { return m_name; }
        
        int getLength() const { return m_data.getDataSize() / (m_size * 4); };
        
    protected:

        string m_name;

        GLuint m_buffer;
        GLenum m_target, m_usage;
        
        int m_size, m_location;
        
        Buffer m_data;
        bool   m_data_dirty;

    };
    typedef shared_ptr< Attribute > AttributeRef;

    Vbo(){};
    Vbo(GLenum type);
    ~Vbo();

    void update();
    void draw();
    void set(const Attribute &attr);
    AttributeRef get(const string &name);
    void assignLocations(GlslProg shader);
    
    static Vbo createPlane(const Vec2f &p1, const Vec2f &p2);
    static Vbo createBox(const Vec3f &p1, const Vec3f &p2);
    static Vbo createBox(const Vec3f &size);

protected:

    GLenum m_type;

    tr1::unordered_map< string, AttributeRef > m_attributes;

};

}} // namespace cinder::gl