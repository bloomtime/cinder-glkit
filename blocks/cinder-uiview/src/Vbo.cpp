#include "Vbo.h"

namespace cinder { namespace gl {

using namespace std;


Vbo::Vbo(GLuint type)
: mType(type)
{
}
Vbo::~Vbo()
{
}

void Vbo::set(AttributeRef attr)
{
    mAttributes[attr->getName()] = attr;
}
Vbo::AttributeRef Vbo::get(const string &name)
{
    auto iter = mAttributes.find(name);
    if(iter != mAttributes.end())
        return iter->second;
    return AttributeRef();
}

void Vbo::assignLocations(GlslProg shader)
{
    for(auto pair : mAttributes){
        pair.second->setLocation(shader.getAttribLocation(pair.first));
    }
}

void Vbo::draw()
{
    int length = numeric_limits<int>::max();
    for(auto pair : mAttributes){
        AttributeRef attr = pair.second;
        if(attr->getTarget() == GL_ARRAY_BUFFER && attr->getLocation() >= 0){
            attr->bindAndEnable();
            length = min(length, attr->getLength());
        }
    }
    if(mAttributes.count("index")){
        AttributeRef index = mAttributes["index"];
        index->bind();
        glDrawElements(mType, index->getData().getDataSize() / 2, index->getType(), 0); // TODO(ryan): This assumes the indices are 16bit ints
    }
    else{
        glDrawArrays(mType, 0, length);
    }
}
void Vbo::draw(GlslProg shader)
{
    assignLocations(shader);
    shader.bind();
    draw();
}


Vbo::Attribute::Attribute(const string &name, int size, GLenum type, GLenum usage)
: mBuffer(0)
, mName(name)
, mSize(size)
, mType(type)
, mUsage(usage)
, mLocation(-1)
, mDataDirty(false)
{
    mTarget = mName == "index" ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
}
Vbo::Attribute::~Attribute()
{
    if(mBuffer){
        glDeleteBuffers(1, &mBuffer);
        mBuffer = 0;
    }
}

Vbo::AttributeRef Vbo::Attribute::create(const string &name, int size, GLenum type, GLenum usage)
{
    return AttributeRef(new Attribute(name, size, type, usage));
}
Vbo::AttributeRef Vbo::Attribute::create(const char *name, int size, GLenum type, GLenum usage)
{
    return AttributeRef(new Attribute(string(name), size, type, usage));
}
Vbo::AttributeRef Vbo::Attribute::createIndex(GLenum usage)
{
    return AttributeRef(new Attribute(string("index"), 1, GL_UNSIGNED_SHORT, usage));
}

Vbo::AttributeRef Vbo::Attribute::setData(const void* data, int data_length)
{
    Buffer buf(data_length);
    buf.copyFrom(data, data_length);
    return setData(buf);
}
Vbo::AttributeRef Vbo::Attribute::setData(const vector<float> &data)
{
    return setData(&data[0], sizeof(float) * data.size());
}
Vbo::AttributeRef Vbo::Attribute::setData(const vector<Vec2f> &data)
{
    return setData(&data[0], sizeof(float) * 2 * data.size());
}
Vbo::AttributeRef Vbo::Attribute::setData(const vector<Vec3f> &data)
{
    return setData(&data[0], sizeof(float) * 3 * data.size());
}

void Vbo::Attribute::bufferData()
{
    if(!mBuffer)
        glGenBuffers(1, &mBuffer);
    glBindBuffer(mTarget, mBuffer);
    glBufferData(mTarget, mData.getDataSize(), mData.getData(), mUsage);
    mDataDirty = false;
}

void Vbo::Attribute::bind()
{
    if(mDataDirty)
        bufferData();
    glBindBuffer(mTarget, mBuffer);
}

void Vbo::Attribute::bindAndEnable()
{
    bind();
    glVertexAttribPointer(mLocation, mSize, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(mLocation);
}


VboRef Vbo::create(GLenum type)
{
    return VboRef(new Vbo(type));
}

VboRef Vbo::createPlane(const Vec2f &p1, const Vec2f &p2)
{
    float positions[] = { p1.x, p1.y, 0, p1.x, p2.y, 0, p2.x, p1.y, 0, p2.x, p2.y, 0 };
    float texcoords[] = { 0, 0, 0, 1, 1, 0, 1, 1 };
    
    VboRef vbo = Vbo::create(GL_TRIANGLE_STRIP);
    vbo->set(Attribute::create("position", 3)->setData(positions, sizeof(float) * 12));
    vbo->set(Attribute::create("texcoord", 2)->setData(texcoords, sizeof(float) * 8));
    
    return vbo;
}
VboRef Vbo::createPlane(const Vec2f &size)
{
    return createPlane(size * -0.5f, size * 0.5f);
}

VboRef Vbo::createBox(const Vec3f &p1, const Vec3f &p2)
{
    GLfloat positions[] = { p2.x,p2.y,p2.z, p2.x,p1.y,p2.z, p2.x,p1.y,p1.z, p2.x,p2.y,p1.z,   // +X
                            p2.x,p2.y,p2.z, p2.x,p2.y,p1.z, p1.x,p2.y,p1.z, p1.x,p2.y,p2.z,   // +Y
                            p2.x,p2.y,p2.z, p1.x,p2.y,p2.z, p1.x,p1.y,p2.z, p2.x,p1.y,p2.z,   // +Z
                            p1.x,p2.y,p2.z, p1.x,p2.y,p1.z, p1.x,p1.y,p1.z, p1.x,p1.y,p2.z,   // -X
                            p1.x,p1.y,p1.z, p2.x,p1.y,p1.z, p2.x,p1.y,p2.z, p1.x,p1.y,p2.z,   // -Y
                            p2.x,p1.y,p1.z, p1.x,p1.y,p1.z, p1.x,p2.y,p1.z, p2.x,p2.y,p1.z }; // -Z
    
    GLfloat normals[] = { 1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,
                          0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,
                          0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,
                         -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
                          0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,
                          0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1 };
    
    GLfloat texcoords[] = { 0,1, 1,1, 1,0, 0,0,
                            1,1, 1,0, 0,0, 0,1,
                            0,1, 1,1, 1,0, 0,0,
                            1,1, 1,0, 0,0, 0,1,
                            1,0, 0,0, 0,1, 1,1,
                            1,0, 0,0, 0,1, 1,1 };
    
    GLushort indices[] = { 0, 1, 2, 0, 2, 3,
                           4, 5, 6, 4, 6, 7,
                           8, 9,10, 8,10,11,
                          12,13,14,12,14,15,
                          16,17,18,16,18,19,
                          20,21,22,20,22,23 };
    
    VboRef vbo = Vbo::create(GL_TRIANGLES);
    vbo->set(Attribute::create("position", 3)->setData(positions, sizeof(GLfloat) * 72));
    vbo->set(Attribute::create("normal", 3)->setData(normals, sizeof(GLfloat) * 72));
    vbo->set(Attribute::create("texcoord", 2)->setData(texcoords, sizeof(GLfloat) * 48));
    vbo->set(Attribute::createIndex()->setData(indices, sizeof(GLushort) * 36));

    return vbo;
}
VboRef Vbo::createBox(const Vec3f &size)
{
    return createBox(size * -0.5f, size * 0.5f);
}

VboRef Vbo::createWireBox(const Vec3f &p1, const Vec3f &p2)
{
    GLfloat positions[] = { p1.x,p1.y,p1.z, p2.x,p1.y,p1.z, p2.x,p2.y,p1.z, p1.x,p2.y,p1.z,
                            p1.x,p1.y,p2.z, p2.x,p1.y,p2.z, p2.x,p2.y,p2.z, p1.x,p2.y,p2.z };
    
    GLushort indices[] = { 0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 3,7, 2,6, 1,5 };
    
    VboRef vbo = Vbo::create(GL_LINES);
    vbo->set(Attribute::create("position", 3)->setData(positions, sizeof(GLfloat) * 24));
    vbo->set(Attribute::createIndex()->setData(indices, sizeof(GLushort) * 24));
    
    return vbo;
}
VboRef Vbo::createWireBox(const Vec3f &size)
{
    return createWireBox(size * -0.5f, size * 0.5f);
}


}} // namespace cinder::gl