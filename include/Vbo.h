#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"

#include <tr1/unordered_map>

namespace cinder { namespace gl {

class Vbo {
public:

    class Attribute;

    typedef std::shared_ptr< Attribute > AttributeRef;

    class Attribute : public std::enable_shared_from_this<Attribute> {
    public:

        Attribute(){}
        ~Attribute();
        
        void bufferData();
        void bind();
        void bindAndEnable();
        
        GLenum getTarget() const { return m_target; }
        GLenum getType() const { return m_type; }
        
        void   setUsage(GLenum usage){ m_usage = usage; m_data_dirty = true; }
        GLenum getUsage() const { return m_usage; }
        
        void setLocation(int location){ m_location = location; }
        int  getLocation() const { return m_location; }
        
        AttributeRef  setData(Buffer data){ m_data = data; m_data_dirty = true; return shared_from_this(); };
        AttributeRef  setData(const void* data, int data_length);
        AttributeRef  setData(const std::vector<float> &data);
        AttributeRef  setData(const std::vector<Vec2f> &data);
        AttributeRef  setData(const std::vector<Vec3f> &data);
        const Buffer& getData() const { return m_data; }
        
        const std::string& getName() const { return m_name; }
        
        int getLength() const { return m_data.getDataSize() / (m_size * getTypeByteSize(m_type)); };
        
        static AttributeRef create(const std::string &name, int size, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        static AttributeRef create(const char *name, int size, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        static AttributeRef createIndex(GLenum usage = GL_STATIC_DRAW);
        
    protected:
    
        Attribute(const std::string &name, int size, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
    
        // TODO(ryan): This is a lot of code for a little function.. might be a better way
        inline static size_t getTypeByteSize(GLenum type){
            switch(type){
                case GL_BYTE:
                case GL_UNSIGNED_BYTE:
                    return 1;
                case GL_SHORT:
                case GL_UNSIGNED_SHORT:
                    return 2;
                case GL_FLOAT:
                case GL_FIXED:
                    return 4;
            }
            return 0;
        }

        std::string m_name;

        GLuint m_buffer;
        GLenum m_target, m_usage, m_type;
        
        int m_size, m_location;
        
        Buffer m_data;
        bool   m_data_dirty;

    };

    Vbo(){};
    Vbo(GLenum type);
    ~Vbo();

    void update();
    void draw();
    AttributeRef get(const std::string &name);
    void set(AttributeRef attr);
    void assignLocations(GlslProg shader);
    
    static Vbo createPlane(const Vec2f &p1, const Vec2f &p2);
    static Vbo createBox(const Vec3f &p1, const Vec3f &p2);
    static Vbo createBox(const Vec3f &size);

protected:

    GLenum m_type;

    std::tr1::unordered_map< std::string, AttributeRef > m_attributes;

};

}} // namespace cinder::gl