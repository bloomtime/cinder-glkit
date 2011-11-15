#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"

#include <tr1/unordered_map>

namespace cinder { namespace gl {

using namespace std;

class Vbo {
public:

    class Attribute {
    public:

        Attribute() : m_buffer(0){}
        Attribute(const string &name, int size, Buffer data, int location = -1);
        Attribute(const char *name, int size, Buffer data, int location = -1);
        Attribute(const Attribute &attr);
        ~Attribute();
        
        void bufferData(GLenum usage);
        void bind();
        void bindAndEnable();
        
        void   setTarget(GLenum target){ m_target = target; }
        GLenum getTarget() const { return m_target; }
        
        void setLocation(int location){ m_location = location; }
        int  getLocation() const { return m_location; }
        
        const string& getName() const { return m_name; }
        const Buffer& getData() const { return m_data; }
        
        int getLength() const { return m_data.getDataSize() / m_size; };
        
    protected:

        string m_name;

        GLuint     m_buffer;
        GLenum     m_target;
        GLsizeiptr m_size;
        
        int m_location;
        
        Buffer m_data;

    };
    typedef shared_ptr< Attribute > AttributeRef;

    Vbo(){};
    Vbo(GLenum type, GLenum usage);
    ~Vbo();

    void update();
    void draw();
    void set(const Attribute &attr);
    
    static Vbo createPlane(const Vec2f &pnt1, const Vec2f &pnt2);

protected:

    GLenum m_type, m_usage;

    tr1::unordered_map< string, AttributeRef > m_attributes;

};

}} // namespace cinder::gl