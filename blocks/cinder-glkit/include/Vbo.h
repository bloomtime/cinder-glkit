#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"

#include <tr1/unordered_map>

namespace cinder { namespace gl {

class Vbo;
typedef std::shared_ptr< Vbo > VboRef;

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
        
        GLenum getTarget() const { return mTarget; }
        GLenum getType() const { return mType; }
        
        void   setUsage(GLenum usage){ mUsage = usage; mDataDirty = true; }
        GLenum getUsage() const { return mUsage; }
        
        void setLocation(int location){ mLocation = location; }
        int  getLocation() const { return mLocation; }
        
        AttributeRef  setData(Buffer data){ mData = data; mDataDirty = true; return shared_from_this(); };
        AttributeRef  setData(const void* data, int data_length);
        AttributeRef  setData(const std::vector<float> &data);
        AttributeRef  setData(const std::vector<Vec2f> &data);
        AttributeRef  setData(const std::vector<Vec3f> &data);
        AttributeRef  setData(const std::vector<Color> &data);
        AttributeRef  setData(const std::vector<ColorA> &data);
        const Buffer& getData() const { return mData; }
        
        const std::string& getName() const { return mName; }
        
        int getLength() const { return mData.getDataSize() / (mSize * getTypeByteSize(mType)); };
        
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
            return 1;
        }

        std::string mName;

        GLuint mBuffer;
        GLenum mTarget, mUsage, mType;
        
        int mSize, mLocation;
        
        Buffer mData;
        bool   mDataDirty;

    };

    ~Vbo();

    void update();
    void draw();
    void draw(GlslProg shader);
    AttributeRef get(const std::string &name);
    void set(AttributeRef attr);
    void assignLocations(GlslProg shader);
    
    static VboRef create(GLenum type);
    static VboRef createPlane(const Vec2f &p1, const Vec2f &p2);
    static VboRef createPlane(const Vec2f &size);
    static VboRef createBox(const Vec3f &p1, const Vec3f &p2);
    static VboRef createBox(const Vec3f &size);
    static VboRef createWireBox(const Vec3f &p1, const Vec3f &p2);
    static VboRef createWireBox(const Vec3f &size);

protected:

    Vbo(GLenum type);

    GLenum mType;

    std::tr1::unordered_map< std::string, AttributeRef > mAttributes;

};

}} // namespace cinder::gl